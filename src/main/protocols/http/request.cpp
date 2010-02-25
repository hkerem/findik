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

#include "request.hpp"
#include "service_container.hpp"

#include <boost/foreach.hpp>

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			request::request(bool is_https) :
				request_path_(""),
				request_uri_(""),
				request_host_("")
			{
				is_local_ = true;
				is_https_ = is_https;
			}

			request::~request()
			{}

			void request::into_buffer(boost::asio::streambuf & sbuf)
			{
				std::ostream request_stream(&sbuf);

				if (method == get)
					request_stream << "GET";
				else if (method == post)
					request_stream << "POST";
				else if (method == head)
					request_stream << "HEAD";
				else if (method == put)
					request_stream << "PUT";
				else if (method == delete_)
					request_stream << "DELETE";
				else if (method == trace)
					request_stream << "TRACE";
				else if (method == options)
					request_stream << "OPTIONS";
				else if (method == connect)
					request_stream << "CONNECT";

				request_stream	<< " ";

				if ( !is_https() && is_proxy_request() )
				{
					request_stream << request_uri();
				}
				else
				{
					request_stream << request_path();
				}

				request_stream << " HTTP/" << http_version_major << "." << http_version_minor << "\r\n";


				BOOST_FOREACH( header h, get_headers() )
					request_stream << h.name << ": " << h.value << "\r\n";

				request_stream << "\r\n";

				if (has_content())
					request_stream.write(&(content()[0]), content().size());

				clear_content();
			}

			const std::string & request::request_uri()
			{
				if (request_uri_ == "")
					request_path();

				return request_uri_;
			}

			const std::string & request::request_path()
			{
				if (request_path_ == "") 
				{
					std::string prefix("");

					if (is_https())
						prefix.append("https://");
					else
						prefix.append("http://");

					if (uri.find(prefix) == 0)
					{
						request_uri_ = uri;
						request_path_ = uri.substr( 7 + uri.substr(7).find("/"));
					} 
					else
					{
						BOOST_FOREACH( header h, get_headers() )
							if ( h.name == "Host")
								request_uri_ = prefix + h.value + uri;
						request_path_ = uri;
					}
				}

				return request_path_;
			}

			const std::string & request::request_host()
                        {
				if(request_host_ == "")
					BOOST_FOREACH( header h, get_headers() )
        	                        	if ( h.name == "Host")
                	                        	request_host_ = h.value;

				return request_host_;
                        }

			void request::reset_uri_and_path()
			{
				request_path_ = "";
				request_uri_ = "";
			}

			bool request::is_proxy_request()
			{
				if (is_https())
					return uri.find("https://") == 0;
				else
					return uri.find("http://") == 0;
			}

		}
	}
}

