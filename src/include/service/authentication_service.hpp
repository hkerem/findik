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

#ifndef FINDIK_SERVICE_AUTHENTICATOR_SERVICE_HPP
#define FINDIK_SERVICE_AUTHENTICATOR_SERVICE_HPP

#include <boost/tuple/tuple.hpp>
#include <boost/noncopyable.hpp>

#include <list>

#include "connection.hpp"
#include "abstract_authenticator.hpp"
#include "authentication_result.hpp"

namespace findik
{
	namespace service
	{
		/*!
		Manageble container for authenticators. 
                \extends boost::noncopyable this class has designed to be not copyable.
		*/
		class authentication_service :
                        private boost::noncopyable
		{
		public:
			/*!
			Default constructor.
			*/
			authentication_service();

			/*!
			Destructor.
			*/
			~authentication_service();

			/*!
			Registers a authenticator for authentication service in order to authenticate with that.
			\param authenticator_ authenticator to register
			*/
			void register_authenticator(findik::authenticator::abstract_authenticator_ptr authenticator_);

			/*!
			Examines connection by using authenticators in authenticator_list_.
			\param connection_ connection to be inspected.
			\returns a tuple containing whether connection should be authenticated or not and authentication result.
			*/
			boost::tuple<bool, findik::authenticator::authentication_result_ptr> 
				authenticate(findik::io::connection_ptr connection_);

		protected:
			/*!
			List to store authenticators in an order. When authenticate operation has been requested
			authenticators in this list will be started to be executed in order.
			*/
			std::list<findik::authenticator::abstract_authenticator_ptr> authenticator_list_;
		};
	}
}

#endif

