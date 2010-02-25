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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "response.hpp"
#include "service_container.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "zlib_util.hpp"

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			response::response(bool is_https) :
				content_encoding_(indeterminate),
				is_chunked_(boost::indeterminate)
			{
				is_local_ = false;
				is_https_ = is_https;
			}

			response::~response()
			{}

			bool response::is_chunked()
			{
				if ( boost::logic::indeterminate(is_chunked_) )
				{
					BOOST_FOREACH( header h, get_headers() )
						if (h.name == "Transfer-Encoding" && h.value == "chunked")
						{
							is_chunked_ = true;
							return is_chunked_;
						}
					is_chunked_ = false;
				}

				return is_chunked_;
			}

			const std::string & response::content_type()
			{
				if (content_type_.empty())
					BOOST_FOREACH( header h, get_headers() )
						if (h.name == "Content-Type") {
							std::size_t pos_ = h.value.find_first_of(';');
							if (pos_ == std::string::npos) {
								content_type_ = h.value;
								break;
							}

							content_type_ = h.value.substr(0,pos_);
							pos_ = h.value.find("charset=");

							if (pos_ == std::string::npos)
								break;

							content_charset_ = h.value.substr(pos_ + 8);
						}

				return content_type_;
			}

			const std::string & response::content_charset()
			{
				if (content_charset_.empty())
					content_type();
				return content_charset_;
			}

			const std::string & response::magic_mime_type()
			{
#ifdef HAVE_MAGIC
				if (magic_mime_type_.empty())
				{
					if (content_hr().size() > 0)
					{
						FI_SERVICES->util_srv().magic_num().get_magic_number(content_hr(),magic_mime_type_);
					}
					else
					{
						magic_mime_type_ = "application/x-empty";
					}
				}
				
				return magic_mime_type_;
#endif
				return content_type();
			}
			response::content_encoding_type response::content_encoding()
			{
				if (content_encoding_ == indeterminate) {
					bool not_found_ = true;
					BOOST_FOREACH( header h, get_headers() )
						if (h.name == "Content-Encoding") {
							if (h.value == "")
								content_encoding_ = none;
							else if (h.value == "gzip")
								content_encoding_ = gzip;
							else if (h.value == "deflate")
								content_encoding_ = deflate;
							else
								content_encoding_ = other;
							not_found_ = false;
							break;
						}
						if (not_found_)
						content_encoding_ = none;
				}

				return content_encoding_;
			}

			void response::into_buffer(boost::asio::streambuf & sbuf)
			{
				std::ostream response_stream(&sbuf);

				response_stream << "HTTP/" << http_version_major << "."
					<< http_version_minor << " " << status_code << " "
					<< status_line << "\r\n";

				BOOST_FOREACH( header h, get_headers() )
					response_stream << h.name << ": " << h.value << "\r\n";

				response_stream << "\r\n";

				if (has_content())
					response_stream.write(&(content()[0]), content().size());

				clear_content();
			}

			const std::vector<char> & response::content_hr()
			{
				switch (content_encoding())
				{
				case none:
					return content_unchunked();
				case gzip:
				case deflate:
					if (content_hr_.empty()) {
						if (findik::util::zlib_inflate(content_unchunked(), content_hr_) != Z_OK)
							content_hr_.clear();
						content_hr_.push_back('\0');
					}
					return content_hr_;
				default:
					return content_unchunked();
				}
			}

			const std::vector<char> & response::content_unchunked()
			{
				if (is_chunked())
					return content_unchunked_;
				else
					return content();
			}

			void response::push_chunked_to_content(char input)
			{
				push_to_content(input); // inherited method

				content_unchunked_.push_back(input);
			}

			void response::wait_for_eof()
			{
				is_expecting_eof_ = true;
			}

		}
	}
}

