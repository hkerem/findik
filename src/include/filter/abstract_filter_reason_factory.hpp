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

#ifndef FINDIK_LOGGER_ABSTRACT_FILTER_REASON_FACTORY_HPP
#define FINDIK_LOGGER_ABSTRACT_FILTER_REASON_FACTORY_HPP

#define FC_ACL_DENY 3001
#define FC_ACL_NA 3002

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "connection.hpp"
#include "filter_reason.hpp"

namespace findik
{
	namespace filter 
	{
		/*!
		Abstract factory object to inherit in order to implement protocal specific factories to create filter_reason objects for findik.
		\extends boost::enable_shared_from_this<abstract_filter_reason_factory> to use boost shared pointers.
		@author H. Kerem Cevahir (shelta)
		*/
		class abstract_filter_reason_factory :
			public boost::enable_shared_from_this<abstract_filter_reason_factory>
		{
		public:

			/*!
			Creates a filter_reason object for given connection and code.
			\param connection_ connection
			\param code reason code
			*/
			virtual filter_reason_ptr create_filter_reason(findik::io::connection_ptr connection_, unsigned int code = 0) = 0;

		protected:

		};
		
		typedef boost::shared_ptr<abstract_filter_reason_factory> abstract_filter_reason_factory_ptr;
	}
}

#endif

