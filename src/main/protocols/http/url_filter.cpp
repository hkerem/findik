/*
  Copyright (C) 2008 Burak OGUZ (barfan) <findikmail@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include "url_filter.hpp"

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			// initialization of logger
			log4cxx::LoggerPtr url_filter::debug_logger_(log4cxx::Logger::getLogger("findik.protocols.http.url_filter"));	

			std::string url_filter::filter_code_ = "content_url";

			boost::tuple<bool, findik::filter::filter_reason_ptr> 
					url_filter::filter(findik::io::connection_ptr connection_, unsigned int param) 
			{
				LOG4CXX_DEBUG(debug_logger_, "URL filter entered"); // log for filter entrance
				
				// get request object from current data
				request_ptr req = boost::static_pointer_cast<request>(connection_->current_data());
				std::string url = req->request_uri();
				if(url.substr(0,5) == "https")
					url = url.substr(8);
				else if(url.substr(0,4) == "http")
					url = url.substr(7);
				// check whether hostname exists in domain blacklist
				if(!FI_SERVICES->db_srv().urlQuery(url, param)){
					boost::shared_ptr<http_filter_result_generator> reply_(new http_filter_result_generator(filter_code_, false, response::forbidden, true, "URL blocked : " + req->request_uri(), req->request_uri(), connection_, req));
                                        return boost::make_tuple(false, findik::filter::filter_reason::create_reason(reply_->reply_str(), reply_->log_str()));
				} 

				findik::filter::filter_reason_ptr frp_;
                                return boost::make_tuple(true, frp_);
			}

                        bool url_filter::is_applicable(findik::io::connection_ptr connection_)
			{
				// set this filter to be used in request only
				return connection_->proto() == findik::io::http && connection_->current_data()->is_local();	
			}
		}
	}
}

