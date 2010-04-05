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

#include "http_filter_result_generator.hpp"
#include <boost/lexical_cast.hpp>
#include <time.h>

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			http_filter_result_generator::http_filter_result_generator(
				const std::string & filter_code, 
				bool filter_result, 
				unsigned int return_code, 
				bool is_close_connection, 
				const std::string & reply_str, 
				const std::string & filter_reason, 
				findik::io::connection_ptr connection_, 
				request_ptr request_, 
				response_ptr response_)
			{
				proto_ = connection_->proto();
				filter_code_ = filter_code;
				filter_result_ = filter_result;
				return_code_ = return_code;
				is_close_connection_ = is_close_connection;
				reply_str_ = reply_str;
				local_endpoint_ = connection_->local_endpoint().to_string();
				domain_name_  = request_->request_host();
				url_ = request_->request_uri();
				request_size_ = request_->content_size();
				response_size_ = response_->content_size(); 	
				filter_reason_ = filter_reason;
				log_str_ = boost::shared_ptr<http_filter_logger>(new http_filter_logger(filter_code_, filter_result_,filter_reason_, connection_, request_, response_ ))->to_string();
				
				return_resp_ = "";
			}

			http_filter_result_generator::http_filter_result_generator(
				const std::string & filter_code, 
				bool filter_result, 
				unsigned int return_code, 
				bool is_close_connection, 
				const std::string & reply_str, 
				const std::string & filter_reason, 
				findik::io::connection_ptr connection_, 
				request_ptr request_) 
                        {
                                proto_ = connection_->proto();
                                filter_code_ = filter_code;
                                filter_result_ = filter_result;
				return_code_ = return_code;
                                is_close_connection_ = is_close_connection;
                                reply_str_ = reply_str;
				local_endpoint_ = connection_->local_endpoint().to_string();
				domain_name_  = request_->request_host();
				url_ = request_->request_uri();
				request_size_ = request_->content_size();
				response_size_ = 0;
                                filter_reason_ = filter_reason;
				log_str_ = boost::shared_ptr<http_filter_logger>(new http_filter_logger(filter_code_, filter_result_, filter_reason_, connection_, request_ ))->to_string();
                        }

			const std::string & http_filter_result_generator::log_str()
			{
				return log_str_;
			}
			
			const std::string & http_filter_result_generator::reply_str()
			{
				if ( return_resp_ == "" )
				{

					return_resp_ = "HTTP/1.1 ";
					if(return_code_ == 403) {
						return_resp_ = return_resp_ + "403 Forbidden\r\n";
					} else {
						return_resp_ = return_resp_ + "404 Not Found\r\n";
					}

					std::string reply_html_;

                                        if (filter_code_ == "btk_filter")
                                        {
                                                FI_SERVICES->db_srv().btkWarningPage(reply_html_);
                                        }
                                        else
                                        {
                                                reply_html_ = FI_SERVICES->reply_srv().reply_html();
                                                time_t rawtime;
                                                time(&rawtime);
#ifdef HAVE_PCRE
                                                FI_SERVICES->util_srv().pcre().global_replace("@@date@@", ctime(&rawtime), reply_html_);
                                                FI_SERVICES->util_srv().pcre().global_replace("@@reason@@", reply_str_ , reply_html_);
#endif 
                                        }
					return_resp_ = return_resp_ + "Content-Type: text/html; charset=UTF-8\r\n";
					return_resp_ = return_resp_ + "Content-Length: " + 
							boost::lexical_cast<std::string>(reply_html_.size()) + "\r\n";
					if(is_close_connection_) 
					{
						return_resp_ = return_resp_ + "Connection : close\r\n";
					}
					else
					{
						return_resp_ = return_resp_ + "Connection : keep-alive\r\n";
					}
					return_resp_ = return_resp_ + "\r\n";
					return_resp_ = return_resp_ + reply_html_;
				}

				return return_resp_;
			}

			const std::string http_filter_result_generator::generateGMTDate()
			{
				time_t rawtime;
				tm * ptm;
				char buf[80];

				time(&rawtime);
				ptm = gmtime ( &rawtime );

				strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", ptm);
				return std::string(buf);
			}
		}
	}
}

