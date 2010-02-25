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

#ifndef FINDIK_PROTOCOLS_HTTP_REQUEST_HPP
#define FINDIK_PROTOCOLS_HTTP_REQUEST_HPP

#include <boost/asio.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "data.hpp"

#include <string>

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			/*!
			Local data implementation for HTTP protocol.
			\extends boost::enable_shared_from_this<request> to use boost shared pointers.
			\extends findik::protocols::http::data because this is a data for http protocol.
			@author H. Kerem Cevahir (shelta)
			*/
			class request :
				public boost::enable_shared_from_this<request>,
				public findik::protocols::http::data
			{
			public:
				/*!
				Default constructor.
				*/
				request(bool is_https = false);

				/*!
				Default destructor.
				*/
				~request();

				/*!
				Method of request.
				*/
				enum request_method {
					get,
					post,
					head,
					put,
					delete_,
					trace,
					options,
					connect
				} method;

				/*!
				Uri in HTTP request command line.
				*/
				std::string uri;

				/*!
				Raw data to sent other side.
				\param sbuf streambuffer to insert raw data into.
				*/
				void into_buffer(boost::asio::streambuf & sbuf);

				/*!
				Gets full uri of request. This methods craft full uri from HTTP request command line and "Host:" header.

				\return full uri of request.
				*/
				const std::string & request_uri();

				/*!
                                Gets hostname of request. 

                                \return full uri of request.
                                */
                                const std::string & request_host();

				/*!
				Gets request path. This methods gets request path from HTTP request command line.

				\return full uri of request.
				*/
				const std::string & request_path();

				/*!
				request_path() and request_uri() methods caches result of the first calculation
				and returns the same value for every repeated call.
				This method clears cache for those methods and enforces them to recalculate.
				*/
				void reset_uri_and_path();

			protected:
				
				/*!
				Request path.
				*/
				std::string request_path_;

				/*!
				Request uri.
				*/
				std::string request_uri_;

				/*!
                                Request hostname.
                                */
                                std::string request_host_;
				
				/*!
				Whether request is sent to a proxy from client.
				\returns whether request is sent to a proxy.
				*/
				bool is_proxy_request();
			};
			
			typedef boost::shared_ptr<request> request_ptr;
		}
	}
}

#endif

