/*
  Copyright (C) 2008 H. Kerem Cevahir (shelta) <findikmail@gmail.com>

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

#include "reply_service.hpp"
#include "service_container.hpp"
#include <time.h>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

namespace findik
{
	namespace service
	{
		reply_service::reply_service()
		{}

		reply_service::~reply_service()
		{}

		void reply_service::start()
		{
			stock_replies_[findik::io::http][FC_BAD_LOCAL] = "Bad request!";
			stock_replies_[findik::io::http][FC_BAD_REMOTE] = "Bad response!";

			// std::string reply_file_name;
			// FI_SERVICES->config_srv().getConfigValue_String("findik.reply.reply_file", reply_file_name);
			
			{
				std::string line;
				std::ifstream file (FI_CONFIG.reply_reply_file().c_str());
				if (file.is_open())
				{
					while (!file.eof())
					{
						getline (file,line);
						reply_html_.append(line);
					}
					file.close();
				}
			}
			{
				std::string line;
				std::ifstream file (FI_CONFIG.reply_ssl_reply_file().c_str());
				if (file.is_open())
				{
					while (!file.eof())
					{
						getline (file,line);
						ssl_reply_html_.append(line);
					}
					file.close();
				}
			}
		}

		void reply_service::reply(boost::asio::streambuf & sbuf,
				findik::io::protocol proto, unsigned int code)
		{
			std::ostream os(&sbuf);
			os << stock_replies_[proto][code];
		}

		void reply_service::reply(boost::asio::streambuf & sbuf,
				findik::io::protocol proto, findik::filter::filter_reason_ptr reason)
		{
			// TODO: reply generator chain .
			std::ostream os(&sbuf);
			os << reason->reply_str();
		}

		void reply_service::reply(boost::asio::streambuf & sbuf,
				findik::io::protocol proto, findik::authenticator::authentication_result_ptr reason)
		{
			// TODO: reply generator chain .
			std::ostream os(&sbuf);
			std::string resp;

                        if(reason->protocol() == findik::io::http)
                        {
                                std::ostringstream stm;
                                std::string reply_str_(reply_html_);
                                resp = "HTTP/1.1 ";
                                if(reason->return_code() == 101) 
				{
                                        resp = resp + "407 Proxy Authentication Required\r\n";
					resp = resp + "Proxy-Authenticate: Negotiate\r\n";	
					resp = resp + "Date: "+generateGMTDate()+"\r\n";	
					resp = resp + "Content-Length : 0\r\n";	
					resp = resp + "Proxy-Connection: close  \r\n";	
					resp = resp + "\r\n";
                                }
				else if (reason->return_code() == 102) 
				{
                                        resp = resp + "407 Proxy Authentication Required\r\n";
                                        resp = resp + "Proxy-Authenticate: Basic realm=\"FINDIK\"\r\n";
                                        resp = resp + "Date: "+generateGMTDate()+"\r\n";
                                        resp = resp + "Content-Length : 0\r\n";
                                        resp = resp + "Proxy-Connection: close  \r\n";
					resp = resp + "\r\n";
                                }
				else if (reason->return_code() == FC_SSL_TMPACCEPT)
				{
					std::string reply_str_(ssl_reply_html_);
#ifdef HAVE_PCRE
					FI_SERVICES->util_srv().pcre().global_replace("@@domain@@", reason->result_str() , reply_str_);	
					FI_SERVICES->util_srv().pcre().global_replace("@@reason@@", boost::lexical_cast<std::string>(reason->code()), reply_str_);
#endif
                                        resp = resp + "200 OK\r\n";
                                        resp = resp + "Content-Type: text/html; charset=UTF-8\r\n";
                                        resp = resp + "Content-Length: " + boost::lexical_cast<std::string>(reply_str_.size()) + "\r\n";
					resp = resp + "Connection: close\r\n";
					resp = resp + "\r\n";
					resp = resp + reply_str_;
                                }
				else
				{
                                        resp = resp + "404 Not Found\r\n";
					resp = resp + "\r\n";
                                }
			}

			os << resp;
		}

		const std::string & reply_service::reply_html()
		{
			return reply_html_;
		}

		const std::string & reply_service::ssl_reply_html()
		{
			return ssl_reply_html_;
		}

		std::string reply_service::generateGMTDate()
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

