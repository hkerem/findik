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

#ifndef FINDIK_PROTOCOLS_HTTP_FILTER_REASON_FACTORY_HPP
#define FINDIK_PROTOCOLS_HTTP_FILTER_REASON_FACTORY_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "abstract_filter_reason_factory.hpp"

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			
			/*!
                        Filter logger for HTTP protocol.
                        \extends boost::enable_shared_from_this<data> to use boost shared pointers.
                        \extends findik::filter::abstract_filter because this is a filter for FINDIK HTTP subsystem.
                        @author H. Kerem Cevahir (shelta)
                        */
			class http_filter_reason_factory :
				public boost::enable_shared_from_this<http_filter_reason_factory>,
				public findik::filter::abstract_filter_reason_factory
                        {
			
			public:
				/*!
				Default c-tor.
				*/
				http_filter_reason_factory();

				/*!
				Default d-tor.
				*/
				~http_filter_reason_factory();

				/*!
				Creates a filter_reason object for given connection and code.
				\param connection_ connection
				\param code reason code
				*/
				findik::filter::filter_reason_ptr create_filter_reason(
					findik::io::connection_ptr connection_, unsigned int code = 0);

				class initializer
                                {
                                protected:
                                        /*!
                                        Default constructor.
                                        Construction of this object will register a http_filter_reason_factory instance to filter_service.
                                        */
                                        initializer();

                                        /*!
                                        Instance to create initializer once.
                                        */
                                        static initializer instance;
                                };
			};

			typedef boost::shared_ptr<http_filter_reason_factory> http_filter_reason_factory_ptr;

		}
	}
}

#endif

