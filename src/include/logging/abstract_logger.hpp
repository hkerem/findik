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

#ifndef FINDIK_LOGGER_ABSTRACT_LOGGER_HPP
#define FINDIK_LOGGER_ABSTRACT_LOGGER_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "filter_reason.hpp"
#include "service_chain_element.hpp"
#include "connection.hpp"
#include "log.hpp"
#include "protocol.hpp"

namespace findik
{
	namespace logger
	{
		/*!
		Abstract logger object to inherit in order to implement loggers for findik.
		\extends boost::enable_shared_from_this<abstract_logger> to use boost shared pointers.
		\extends findik::service::service_chain_element to be used in service chains.
		@author Burak OGUZ (barfan)
		*/
		class abstract_logger :
			public boost::enable_shared_from_this<abstract_logger>,
			public findik::service::service_chain_element
		{
		public:

			/*!
			Logger to_string method
			\return string representation of log 
			*/
			virtual const std::string & to_string() = 0;

		protected:

		};
		
		typedef boost::shared_ptr<abstract_logger> abstract_logger_ptr;
	}
}

#endif

