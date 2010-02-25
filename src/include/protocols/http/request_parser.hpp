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

#ifndef FINDIK_PROTOCOLS_HTTP_REQUEST_PARSER_HPP
#define FINDIK_PROTOCOLS_HTTP_REQUEST_PARSER_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/thread/mutex.hpp>

#include "abstract_local_parser.hpp"
#include "abstract_stateful_parser.hpp"
#include "connection.hpp"
#include "request.hpp"

#include <map>
#include <string>

#define FC_TMPSTR 2005

#define FIR_TMPSTR_OF(conn) conn->str_map[FC_TMPSTR]
#define FI_TMPSTR_OF(conn) FIR_TMPSTR_OF(conn)

namespace findik
{
	namespace protocols
	{
		namespace http
		{

			/*!
			Request parser for HTTP protocol.
			\extends boost::enable_shared_from_this<request_parser> to use boost shared pointers.
			\extends findik::parser::abstract_stateful_parser to be used in parser chains.
			@author H. Kerem Cevahir (shelta)
			*/
			class request_parser :
				public boost::enable_shared_from_this<request_parser>,
				public findik::parser::abstract_local_parser
			{
			public:

				class initializer
				{
				protected:
					/*!
					Default constructor.
					Construction of this object will register a request_parser instance to parser_service.
					*/
					initializer();

					/*!
					Instance to create initializer once.
					*/
					static initializer instance;
				};

				/*!
				Parse some data. The tribool return value is true when a complete request
				has been parsed, false if the data is invalid, indeterminate when more
				data is required. The InputIterator return value indicates how much of the
				input has been consumed.

				\param connection_ connection object to populate data onject into with parsing results.
				\param begin start point of parser
				\param end stop point of parser

				\returns a tuple which contains parse result and begin point. Parse result will be true if input is a proper protocol data and parsed successfully, false if input is not a proper protocol data, indeterminate if parser needs more data to decide whether input is proper.
				*/
				boost::tuple<boost::tribool, char*> parse(
						findik::io::connection_ptr connection_,
						char* begin, char* end
					);

				/*!
				Scans all headers, then updates remote_hostname parameter of connection.
				\param connection_ to inspect.
				\param hostname_ to update.
				*/
				void update_hostname_of(findik::io::connection_ptr connection_, std::string & hostname_);

				/*!
				Scans all headers, then updates remote_port parameter of connection.
				\param connection_ to inspect.
				\param port_ to update.
				*/
				void update_port_of(findik::io::connection_ptr connection_, unsigned int & port_);

			protected:
				/*!
				Debug logger for server class.
				*/
				static log4cxx::LoggerPtr debug_logger;

				/*!
				Defined parser states.
				*/
				enum states {
					method_start = 40,
					method = 41,
					uri_start = 10,
					uri = 11,
					http_version_h = 20,
					http_version_t_1 = 21,
					http_version_t_2 = 22,
					http_version_p = 23,
					http_version_slash = 24,
					http_version_major_start = 25,
					http_version_major = 26,
					http_version_minor_start= 27,
					http_version_minor = 28,
					expecting_newline_1 = 500,
					header_line_start = 30,
					header_lws = 31,
					header_name = 32,
					space_before_header_value = 33,
					header_value = 34,
					expecting_newline_2 = 501,
					expecting_newline_3 = 503,
					content = 600
				};
				
				/*!
				Handle the next character of input.
				
				\param req request object to populate
				\return true if input is a proper HTTP request and parsed successfully, false if input is not a proper HTTP request, indeterminate if parser needs more data to decide whether input is proper.
				*/
				boost::tribool consume(findik::io::connection_ptr connection_, char input);

			};
			
			typedef boost::shared_ptr<request_parser> request_parser_ptr;
		}
	}
}

#endif

