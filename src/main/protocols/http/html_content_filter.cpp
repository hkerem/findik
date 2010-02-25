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

#include "html_content_filter.hpp"
#ifdef HAVE_PCRE

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			// initialization of logger
			log4cxx::LoggerPtr html_content_filter::debug_logger_(log4cxx::Logger::getLogger("findik.protocols.http.html_content_filter"));	
			std::string html_content_filter::filter_code_ = "content_html";	

			boost::tuple<bool, findik::filter::filter_reason_ptr> 
					html_content_filter::filter(findik::io::connection_ptr connection_, unsigned int param) 
			{
				LOG4CXX_DEBUG(debug_logger_, "HTML content filter entered"); // log for filter entrance
				response_ptr resp = boost::static_pointer_cast<response>(connection_->current_data());
				request_ptr req = last_request_of(connection_);
				std::string content_type;
				if((resp->magic_mime_type() == "text/html") || (resp->content_type() == "text/html") || (resp->magic_mime_type().compare(0,10,"text/plain") == 0)) 
				{
					if (FI_SERVICES->util_srv().pcre().matches_predefined( &(resp->content_hr())[0] , param) )
					{
						boost::shared_ptr<http_filter_result_generator> reply_(new http_filter_result_generator(filter_code_, false, response::forbidden, true, "Content blocked for URL : "  + req->request_uri(), "content", connection_, req, resp));
						return boost::make_tuple(false, findik::filter::filter_reason::create_reason(reply_->reply_str(), reply_->log_str()));
					}
				}
		
				findik::filter::filter_reason_ptr frp_;
                                return boost::make_tuple(true, frp_);
			}

			bool html_content_filter::is_applicable(findik::io::connection_ptr connection_)
			{
				// set this filter to be used in request only
				return connection_->proto() == findik::io::http
					&& connection_->current_data()->is_remote()
					&& connection_->current_data()->has_content();	
			}
		}
	}
}
#endif /* HAVE_PCRE */
