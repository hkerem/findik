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

#ifndef FINDIK_PROTOCOLS_HTTP_CONTENT_FILTER_GROUP_HPP
#define FINDIK_PROTOCOLS_HTTP_CONTENT_FILTER_GROUP_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "filter_group.hpp"

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			
			/*!
                        Content filter group.
                        \extends boost::enable_shared_from_this<data> to use boost shared pointers.
                        \extends findik::filter::abstract_filter because this is a filter for FINDIK HTTP subsystem.
                        @author H. Kerem Cevahir (shelta)
                        */
			class content_filter_group:
				public boost::enable_shared_from_this<content_filter_group>,
				public findik::filter::filter_group
                        {
			
			public:

				/*!
				Default c-tor.
				*/
				content_filter_group();
				
				/*!
				Default d-tor.
				*/
				~content_filter_group();

				class initializer
                                {
                                protected:
                                        /*!
                                        Default constructor.
                                        Construction of this object will register a content_mime_filter instance to filter_service.
                                        */
                                        initializer();

                                        /*!
                                        Instance to create initializer once.
                                        */
                                        static initializer instance;
                                };

			
			protected:
				static log4cxx::LoggerPtr debug_logger_;		
				static std::string filter_code_;	
			};

			typedef boost::shared_ptr<content_filter_group> content_filter_group_ptr;

		}
	}
}

#endif

