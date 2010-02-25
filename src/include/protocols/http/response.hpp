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

#ifndef FINDIK_PROTOCOLS_HTTP_RESPONSE_HPP
#define FINDIK_PROTOCOLS_HTTP_RESPONSE_HPP

#include <boost/asio.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/logic/tribool.hpp>

#include "data.hpp"

#include <string>
#include <vector>

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			/*!
			Remote data implementation for HTTP protocol.
			\extends boost::enable_shared_from_this<response> to use boost shared pointers.
			\extends findik::protocols::http::data because this is a data for http protocol.
			@author H. Kerem Cevahir (shelta)
			*/
			class response :
				public boost::enable_shared_from_this<response>,
				public findik::protocols::http::data
			{
			public:
				/*!
				Default constructor.
				*/
				response(bool is_https = false);

				/*!
				Default destructor.
				*/
				~response();

				/*!
				Status code of response.
				*/
				enum status_type
				{
					// Informational
					continue_ = 100,
					switching_protocols = 101,
					// success
					ok = 200,
					created = 201,
					accepted = 202,
					no_content = 204,
					reset_content = 205,
					partial_content = 206,
					multiple_choices = 300,
					moved_permanently = 301,
					// redirection
					found = 302,
					see_other = 303,
					not_modified = 304,
					use_proxy = 305,
					switch_proxy = 306,
					temporary_redirect = 307,
					// client error
					bad_request = 400,
					unauthorized = 401,
					payment_required = 402,
					forbidden = 403,
					not_found = 404,
					method_not_allowed = 405,
					not_acceptable = 406,
					proxy_authentication_required = 407,
					request_timeout = 408,
					conflicy = 409,
					gone = 410,
					length_required = 411,
					precondition_failed = 412,
					request_entity_too_large = 413,
					request_uri_too_long = 414,
					unsupported_media_type = 415,
					requested_range_not_satifiable = 416,
					expectation_failed = 417,
					// server error
					internal_server_error = 500,
					not_implemented = 501,
					bad_gateway = 502,
					service_unavailable = 503,
					gateway_timeout = 504,
					http_version_not_supported = 505,
					// http extensions
					processing_webdab = 102,
					request_uri_too_long_ext = 122,
					multi_status_webdav = 207,
					i_am_a_teapot = 418,
					unprocessable_entity_webdav = 422,
					locked_webdav = 423,
					failed_dependency_webdav = 424,
					unordered_collection = 425,
					upgrade_required = 426,
					retry_with = 449,
					blocked = 450,
					variant_also_negotiates = 506,
					insufficient_storage_webdav = 507,
					bandwidth_limit_exceeded = 509,
					not_extended = 510
				} status_code;

				/*!
				HTTP status line associated with response status code.
				*/
				std::string status_line;

				/*!
				Content encoding types for response.
				*/
				enum content_encoding_type {
					gzip,
					deflate,
					none,
					other,
					indeterminate
				};

				/*!
				Checks if response content is chunked.
				\return true if content is chunked, else false.ç
				*/
				bool is_chunked();

				/*!
				Gets content type of response data. 

				\return type of response content if "Content-Type" key has been found in headers, else "".
				*/
				const std::string & content_type();
				
				/*!
				Gets charset of response content. Only applicable for text mimetypes.
				
				\return charset of response content if content_type() is text and "charset" identifier has been specified, else "".
				*/
				const std::string & content_charset();

				/*!
				Gets content encoding of response data.

				\return encoding of response content if "Content-Encoding" key has been found in headers, else indeterminate.
				*/
				content_encoding_type content_encoding();

				/*!
				Gets magic number based mime type of the data
                                \return value of magic mime type value
                                */
				const std::string & magic_mime_type();

				/*!
				Detects whether response is chunked and whether response is deflated, then processes raw
				content and generates new data into a new vector.
				\returns human readable content.
				*/
				const std::vector<char> & content_hr();

				/*!
				Raw data to sent other side.
				\param sbuf streambuffer to insert raw data into.
				*/
				void into_buffer(boost::asio::streambuf & sbuf);

				/*!
				A wrapper to inherited push_to_content() method. 
				This pushes character to both raw content vector and unchunked content vector.
				\param input char to be pushed.
				*/
				void push_chunked_to_content(char input);

				/*!
				In HTTP/1.0 server could respond with no content-length related header. 
				In this situation terimination of data will be done by socket close from server side.
				Calling this method will set is_expecting_eof_ variable to true.
				*/
				void wait_for_eof();

			protected:

				/*!
				Type of response content.
				*/
				std::string content_type_;

				/*!
                                Magic number based mime type of response content.
                                */
                                std::string magic_mime_type_;


				/*!
				Charset of response content.
				*/
				std::string content_charset_;

				/*!
				Whether repsonse content is chunked.
				*/
				boost::tribool is_chunked_;

				/*!
				Encoding of response content.
				*/
				content_encoding_type content_encoding_;

				/*!
				Human readable content.
				Populated when content_hr() called.
				*/
				std::vector<char> content_hr_;

				/*!
				Unchunkked content.
				Populated when content_unchunkked() called.
				*/
				std::vector<char> content_unchunked_;

				/*!
				Detects whether response is chunked and whether response is deflated, then processes raw
				content and generates new data into a new vector.
				\returns human readable content.
				*/
				const std::vector<char> & content_unchunked();

			};
			
			typedef boost::shared_ptr<response> response_ptr;
		}
	}
}

#endif

