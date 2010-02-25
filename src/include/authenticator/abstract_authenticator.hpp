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

#ifndef FINDIK_AUTHENTICATOR_ABSTRACT_AUTHENTICATOR_HPP
#define FINDIK_AUTHENTICATOR_ABSTRACT_AUTHENTICATOR_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/tuple/tuple.hpp>

#include "authentication_result.hpp"
#include "service_chain_element.hpp"
#include "connection.hpp"
#include "log.hpp"

#include "protocol.hpp"

namespace findik
{
	namespace authenticator
	{
		/*!
		Abstract authenticator object to inherit in order to implement authenticators for findik.
		\extends boost::enable_shared_from_this<abstract_authenticator> to use boost shared pointers.
		\extends findik::service::service_chain_element to be used in service chains.
		@author H. Kerem Cevahir (shelta)
		*/
		class abstract_authenticator :
			public boost::enable_shared_from_this<abstract_authenticator>,
			public findik::service::service_chain_element
		{
		public:
                        /*!
                        Examines new_data of connection by using authenticators in authenticator_list_.
			If method returns true for decision, service will execute other authenticators too,
			otherwise it will stop execution and return false with reason of this operation.
                        \param connection_ connection contains new data to be inspected.
                        \returns a tuple containing whether content should be authenticated or not and the authentication object.
                        */
                        virtual boost::tuple<bool, findik::authenticator::authentication_result_ptr> authenticate(findik::io::connection_ptr connection_) = 0;

			/*!
			Authentication service will match authenticators with connections according to their protocols. 
			To determine protocol of authenticator, authentication service will use this method.
			\returns protocol of this authenticator.
			*/
			virtual findik::io::protocol proto() = 0;

		protected:

		};
		
		typedef boost::shared_ptr<abstract_authenticator> abstract_authenticator_ptr;
	}
}

#endif

