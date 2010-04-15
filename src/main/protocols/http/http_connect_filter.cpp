/*
  Copyright (C) 2010 H. Kerem Cevahir (shelta) <findikmail@gmail.com>

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

#include "http_connect_filter.hpp"

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			// initialization of logger
			log4cxx::LoggerPtr http_connect_filter::debug_logger_(log4cxx::Logger::getLogger("findik.protocols.http.http_connect_filter"));	

			std::string http_connect_filter::filter_code_ = "http_connect";

			boost::tuple<bool, findik::filter::filter_reason_ptr> 
					http_connect_filter::filter(findik::io::connection_ptr connection_, unsigned int param) 
			{
				LOG4CXX_DEBUG(debug_logger_, "HTTP Connect filter entered"); // log for filter entrance
				
				// get request object from current data
				request_ptr req = boost::static_pointer_cast<request>(connection_->current_data());

				// check whether method is connect
				if(req->method == request::connect){
					boost::shared_ptr<http_filter_result_generator> reply_(
						new http_filter_result_generator(
							filter_code_, false, 
							response::forbidden, true, 
							"HTTP Connet request denied.", 
							req->request_host(), connection_, req
						));	
					return boost::make_tuple(false, findik::filter::filter_reason::create_reason(reply_->reply_str(), reply_->log_str()));	
				} 

				findik::filter::filter_reason_ptr frp_;
                                return boost::make_tuple(true, frp_);
			}

                        bool http_connect_filter::is_applicable(findik::io::connection_ptr connection_)
			{
				// set this filter to be used in request only
				return connection_->proto() == findik::io::http && connection_->current_data()->is_local();	
			}
		}
	}
}
