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

#include "request_parser.hpp"

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include <string>
#include <cctype>

#include "parser_util.hpp"
#include "service_container.hpp"
#include "protocol.hpp"

#include "http_parser_constants.hpp"

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			log4cxx::LoggerPtr request_parser::debug_logger(log4cxx::Logger::getLogger("findik.protocols.http.request_parser"));

			request_parser::initializer::initializer()
			{
				request_parser_ptr p(new request_parser());

				FI_SERVICES->parser_srv().register_local_parser(findik::io::http, p);
			}

			request_parser::initializer request_parser::initializer::instance;

			boost::tuple<boost::tribool, char*> request_parser::parse(
					findik::io::connection_ptr connection_,
					char* begin, char* end
				)
			{
				if (connection_->current_data().get() != 0) // if stream, do not parse data just increment size and return
				{
					request_ptr req = boost::static_pointer_cast<request>(connection_->current_data());
					if (req->is_stream())
					{
						req->add_to_stream_content_size((end - begin)/sizeof(char));

						boost::tribool result = boost::indeterminate;
						if ( !( req->content_size() < req->content_length() ) )
							result = true;

						return boost::make_tuple(result, end);
					}
				}

				while (begin != end)
				{
					boost::tribool result = consume(connection_, *begin++);
					if (result || !result)
						return boost::make_tuple(result, begin);
				}

				boost::tribool result = boost::indeterminate;
				return boost::make_tuple(result, begin);
			}

			boost::tribool request_parser::consume(findik::io::connection_ptr connection_, char input)
			{
				if (connection_->current_data().get() == 0)
				{
//					LOG4CXX_DEBUG(debug_logger, "Creating new state entry for connection.");
					FI_LSTATE_OF(connection_) = method_start;
					request_ptr p(new request(connection_->is_secure()));
					connection_->update_current_data(p);
				}

				request_ptr req = boost::static_pointer_cast<request>(connection_->current_data());

				switch (FIR_LSTATE_OF(connection_))
				{
					case method_start:
						if (!is_char(input) || is_ctl(input) || is_tspecial(input))
						{
							return false;
						}
						else
						{
							FI_LSTATE_OF(connection_) = method;
							FI_TMPSTR_OF(connection_).clear();
							FIR_TMPSTR_OF(connection_).push_back(toupper(input));
							return boost::indeterminate;
						}
					case method:
						if (input == ' ')
						{
							if (FIR_TMPSTR_OF(connection_) == "GET")
								req->method = request::get;

							else if (FIR_TMPSTR_OF(connection_) == "POST")
								req->method = request::post;

							else if (FIR_TMPSTR_OF(connection_) == "HEAD")
								req->method = request::head;

							else if (FIR_TMPSTR_OF(connection_) == "PUT")
								req->method = request::put;

							else if (FIR_TMPSTR_OF(connection_) == "DELETE")
								req->method = request::delete_;

							else if (FIR_TMPSTR_OF(connection_) == "TRACE")
								req->method = request::trace;

							else if (FIR_TMPSTR_OF(connection_) == "OPTIONS")
								req->method = request::options;

							else if (FIR_TMPSTR_OF(connection_) == "CONNECT")
							{
								if (! FI_CONFIG.server_http_run_with_squid())
								{
									LOG4CXX_ERROR(debug_logger, "FINDIK does not support HTTP CONNECT without squid.");
									return false;
								}
								req->method = request::connect;
							}
							else
								return false;

							FIR_TMPSTR_OF(connection_).clear();

							FI_LSTATE_OF(connection_) = uri;
							return boost::indeterminate;
						}
						else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
						{
							return false;
						}
						else
						{
							FI_TMPSTR_OF(connection_).push_back(toupper(input));
							FI_CHECK_SSTR( FIR_TMPSTR_OF(connection_) );
							return boost::indeterminate;
						}
					case uri_start:
						if (is_uri_char(input))
						{
							FI_LSTATE_OF(connection_) = uri;
							req->uri.push_back(input);
							return boost::indeterminate;
						}
						else
							return false;
					case uri:
						if (input == ' ')
						{
							FI_LSTATE_OF(connection_) = http_version_h;
							return boost::indeterminate;
						}
						else if (is_uri_char(input))
						{
							req->uri.push_back(input);
							FI_CHECK_LSTR(req->uri);
							return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case http_version_h:
						if (input == 'H')
						{
							FI_LSTATE_OF(connection_) = http_version_t_1;
							return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case http_version_t_1:
						if (input == 'T')
						{
							FI_LSTATE_OF(connection_) = http_version_t_2;
							return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case http_version_t_2:
						if (input == 'T')
						{
							FI_LSTATE_OF(connection_) = http_version_p;
							return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case http_version_p:
						if (input == 'P')
						{
							FI_LSTATE_OF(connection_) = http_version_slash;
							return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case http_version_slash:
						if (input == '/')
						{
							req->http_version_major = 0;
							req->http_version_minor = 0;
							FI_LSTATE_OF(connection_) = http_version_major_start;
							return boost::indeterminate;
						}
						else
						{
							return false;
					}
					case http_version_major_start:
						if (is_digit(input))
						{
							req->http_version_major = 
								req->http_version_major * 10 + input - '0';
							FI_LSTATE_OF(connection_) = http_version_major;
							return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case http_version_major:
						if (input == '.')
						{
							FI_LSTATE_OF(connection_) = http_version_minor_start;
							return boost::indeterminate;
						}
						else if (is_digit(input))
						{
							req->http_version_major = 
								req->http_version_major * 10 + input - '0';
							FI_CHECK_SINT(req->http_version_major);
						return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case http_version_minor_start:
						if (is_digit(input))
						{
							req->http_version_minor = 
								req->http_version_minor * 10 + input - '0';
							FI_LSTATE_OF(connection_) = http_version_minor;
							return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case http_version_minor:
						if (input == '\r')
						{
							FI_LSTATE_OF(connection_) = expecting_newline_1;
							return boost::indeterminate;
						}
						else if (is_digit(input))
						{
							req->http_version_minor = 
								req->http_version_minor * 10 + input - '0';
							FI_CHECK_SINT(req->http_version_minor);
							return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case expecting_newline_1:
						if (input == '\n')
						{
							FI_LSTATE_OF(connection_) = header_line_start;
							return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case header_line_start:
						if (input == '\r')
						{
							FI_LSTATE_OF(connection_) = expecting_newline_3;
							return boost::indeterminate;
						}
						else if (!req->get_headers().empty() 
								&& (input == ' ' || input == '\t'))
						{
							FI_LSTATE_OF(connection_) = header_lws;
							return boost::indeterminate;
						}
						else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
						{
							return false;
						}
						else
						{
							req->add_blank_header();
							FI_CHECK_VCTR(req->get_headers())
							req->last_header().name.push_back(input);
							FI_LSTATE_OF(connection_) = header_name;
							return boost::indeterminate;
						}
					case header_lws:
						if (input == '\r')
						{
							FI_LSTATE_OF(connection_) = expecting_newline_2;
							return boost::indeterminate;
						}
						else if (input == ' ' || input == '\t')
						{
							return boost::indeterminate;
						}
						else if (is_ctl(input))
						{
							return false;
						}
						else
						{
							FI_LSTATE_OF(connection_) = header_value;
							req->last_header().value.push_back(input);
							return boost::indeterminate;
						}
					case header_name:
						if (input == ':')
						{
							trim_spaces(req->last_header().name);
							FI_LSTATE_OF(connection_) = space_before_header_value;
							return boost::indeterminate;
						}
						else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
						{
							return false;
						}
						else
						{
							req->last_header().name.push_back(input);
							FI_CHECK_MSTR(req->last_header().name);
							return boost::indeterminate;
						}
					case space_before_header_value:
						if (input == ' ')
						{
							FI_LSTATE_OF(connection_) = header_value;
							return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case header_value:
						if (input == '\r')
						{
							trim_spaces(req->last_header().value);
							FI_LSTATE_OF(connection_) = expecting_newline_2;
							return boost::indeterminate;
						}
						else if (is_ctl(input))
						{
							return false;
						}
						else
						{
							req->last_header().value.push_back(input);
							if (req->last_header().name == "Cookie")
							{
								FI_CHECK_HSTR(req->last_header().value);
							}
							else
							{
								FI_CHECK_LSTR(req->last_header().value);
							}
							return boost::indeterminate;
						}
					case expecting_newline_2:
						if (input == '\n')
						{
							FI_LSTATE_OF(connection_) = header_line_start;
							return boost::indeterminate;
						}
						else
						{
							return false;
						}
					case expecting_newline_3:
						if (input != '\n')
							return false;
						else if (req->method == request::post) 
						{
							if (req->content_length() == 0)
							{
								return true;
							}
							else
							{
								if (req->content_length() > 
									FI_CONFIG.server_http_max_object_size())
									req->mark_as_stream();

								FI_LSTATE_OF(connection_) = content;
								return boost::indeterminate;
							}
						}
						else
						{
							return true;
						}
					case content:
						req->push_to_content(input);
						if (req->content_size() < req->content_length())
							return boost::indeterminate;
						else
							return true;
					default:
						return false;
				}
			}

			void request_parser::update_hostname_of(findik::io::connection_ptr connection_, std::string & hostname_)
			{
				if (connection_->current_data().get() == 0)
					return;

				if (connection_->current_data()->is_remote())
					return;

				if ( !connection_->is_secure() && FI_CONFIG.server_http_run_with_squid() )
				{
					hostname_ = FI_CONFIG.server_http_squid_host();
				}
				else
				{
					request_ptr req = boost::static_pointer_cast<request>(connection_->current_data());

					BOOST_FOREACH( header h, req->get_headers() )
						if (h.name == "Host")
						{
							std::size_t pos_ = h.value.find_first_of(":");

							if (pos_ == std::string::npos)
								hostname_ = h.value;
							else
								hostname_ = h.value.substr(0, pos_);
						}
				}
			}

			void request_parser::update_port_of(findik::io::connection_ptr connection_, unsigned int & port_)
			{
				if (connection_->current_data().get() == 0)
					return;

				if (connection_->current_data()->is_remote())
					return;

				if ( !connection_->is_secure() && FI_CONFIG.server_http_run_with_squid() )
				{
					port_ = FI_CONFIG.server_http_squid_port();
				}
				else
				{
					request_ptr req = boost::static_pointer_cast<request>(connection_->current_data());

					BOOST_FOREACH( header h, req->get_headers() )
						if (h.name == "Host")
						{
							std::size_t pos_ = h.value.find_first_of(":");

							if (pos_ == std::string::npos)
							{
								if ( connection_->is_secure() )
									port_ = 443;
								else
									port_ = 80;
							}
							else
							{
								port_ = boost::lexical_cast< unsigned int >(
										h.value.substr(pos_ + 1)
									);
							}
						}
				}
			}

		}
	}
} 

