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

#include "http_filter_logger.hpp"

namespace findik
{
	namespace protocols
	{
		namespace http
		{

			std::string http_filter_logger::attr_separator_ = " ";
			std::string http_filter_logger::key_value_connector_ = ":";
			std::string http_filter_logger::proto_identifier_ = "protocol";
			std::string http_filter_logger::filter_code_identifier_ = "filter";
			std::string http_filter_logger::filter_result_identifier_ = "result";
			std::string http_filter_logger::local_endpoint_identifier_ = "local_addr";
			std::string http_filter_logger::domain_name_identifier_ = "domain";
			std::string http_filter_logger::url_identifier_ = "url";
//			std::string http_filter_logger::mime_type_identifier_ = "type";
			std::string http_filter_logger::request_size_identifier_ = "req_size";
			std::string http_filter_logger::response_size_identifier_ = "resp_size";
			std::string http_filter_logger::response_code_identifier_ = "resp_code";
			std::string http_filter_logger::filter_reason_identifier_ = "filter_reason";

			http_filter_logger::http_filter_logger(
				const std::string & filter_code, 
				bool filter_result, 
				const std::string & filter_reason, 
				findik::io::connection_ptr connection_, 
				request_ptr request_, 
				response_ptr response_)
			{
				proto_ = connection_->proto();
				filter_code_ = filter_code;
				filter_result_ = filter_result;
				local_endpoint_ = connection_->local_endpoint().to_string();	
				domain_name_  = request_->request_host();
				url_ = request_->request_uri();
				request_size_ = request_->content_size();
//				mime_type_ = response_->magic_mime_type();
				response_size_ = response_->content_size(); 	
				response_code_ = boost::lexical_cast<std::string>(response_->status_code); 	
				filter_reason_ = filter_reason;
				log_str_ = "";
			}

			http_filter_logger::http_filter_logger(
				const std::string & filter_code, 
				bool filter_result, 
				const std::string & filter_reason, 
				findik::io::connection_ptr connection_, 
				request_ptr request_)
                        {
                                proto_ = connection_->proto();
                                filter_code_ = filter_code;
                                filter_result_ = filter_result;
				local_endpoint_ = connection_->local_endpoint().to_string();
				domain_name_  = request_->request_host();
				url_ = request_->request_uri();
				request_size_ = request_->content_size();
//				mime_type_ = "";
				response_size_ = 0;
				response_code_ = "403";
                                filter_reason_ = filter_reason;
				log_str_ = "";
                        }

			const std::string & http_filter_logger::to_string()
			{
				if (log_str_ == "")
				{
					log_str_ += proto_identifier_ + key_value_connector_ + boost::lexical_cast<std::string>(proto_) + attr_separator_;
					if(filter_result_) {
						log_str_ += filter_result_identifier_ + key_value_connector_ + "p" + attr_separator_;
						
					}
					else {
						log_str_ += filter_code_identifier_ + key_value_connector_ + boost::lexical_cast<std::string>(filter_code_) + attr_separator_;			
						log_str_ += filter_result_identifier_ + key_value_connector_ + "f" + attr_separator_;
					}

					log_str_ += local_endpoint_identifier_ + key_value_connector_ + local_endpoint_ + attr_separator_;
					log_str_ += domain_name_identifier_ + key_value_connector_ + domain_name_ + attr_separator_;
					log_str_ += url_identifier_ + key_value_connector_ + url_ + attr_separator_;
//					log_str_ += mime_type_identifier_ + key_value_connector_ + mime_type_ + attr_separator_;
					log_str_ += request_size_identifier_ + key_value_connector_ + boost::lexical_cast<std::string>(request_size_) + attr_separator_;
					log_str_ += response_size_identifier_ + key_value_connector_ + boost::lexical_cast<std::string>(response_size_) + attr_separator_;
					log_str_ += response_code_identifier_ + key_value_connector_ + response_code_ + attr_separator_;
					if(!filter_result_)
						log_str_ += filter_reason_identifier_ + key_value_connector_ + filter_reason_ + attr_separator_;
				}
				
				return log_str_;
			}
		}
	}
}

