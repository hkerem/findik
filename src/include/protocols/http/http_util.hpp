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

#ifndef FINDIK_PROTOCOLS_HTTP_HTTP_UTIL_HPP
#define FINDIK_PROTOCOLS_HTTP_HTTP_UTIL_HPP

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "service_container.hpp"
#include "request.hpp"
#include "response.hpp"

namespace findik {
	namespace protocols {
		namespace http {


			/*!
                        Iterates over data_queue of connection and returns most recent local data.
                        \param connection_
                        \returns most recent local data.
                        */		
			static request_ptr last_request_of(findik::io::connection_ptr connection_)
                        {
                                request_ptr req;

				if (connection_->current_data().get() != 0 && connection_->current_data()->is_local())
				{
					req = boost::static_pointer_cast<request>(connection_->current_data());
				}
				else
				{
					BOOST_REVERSE_FOREACH(findik::io::abstract_data_ptr data_, connection_->data_queue())
						if (data_->is_local())
						{
							req = boost::static_pointer_cast<request>(data_);
						}
				}

                                return req;
                        }

			/*!
                        Iterates over data_queue of connection and returns most recent remote data.
                        \param connection_
                        \returns most recent remote data.
                        */		
			static response_ptr last_response_of(findik::io::connection_ptr connection_)
                        {
                                response_ptr resp;

				if (connection_->current_data().get() != 0 && connection_->current_data()->is_remote())
				{
					resp = boost::static_pointer_cast<response>(connection_->current_data());
				}
				else
				{
					BOOST_REVERSE_FOREACH(findik::io::abstract_data_ptr data_, connection_->data_queue())
						if (data_->is_remote())
						{
							resp = boost::static_pointer_cast<response>(data_);
						}
				}

                                return resp;
                        }
			
		}
	}
}

#endif

