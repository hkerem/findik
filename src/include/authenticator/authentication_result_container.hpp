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

#ifndef FINDIK_AUTHENTICATOR_AUTHENTICATOR_REASON_HPP
#define FINDIK_AUTHENTICATOR_AUTHENTICATOR_REASON_HPP

#include <set>
#include <string>
#include "authentication_result.hpp"

#include <boost/thread/mutex.hpp>

namespace findik
{
	namespace authenticator
	{
		/*!
		Container class to extend in order to provide container functionality for authentication result objects.
		\extends boost::enable_shared_from_this<authentication_result_container> to use boost shared pointers.
		@author H. Kerem Cevahir (shelta)
		*/
		class authentication_result_container
		{
		public:

			/*!
			Stores specified authentication result in the container.
			\param authentication to store.
			*/
			void store_authentication(authentication_result_ptr authentication);

			/*!
			Searches container for given parameters.
			\returns true if found a match, else false
			*/
			bool is_already_authenticated(unsigned int code, std::string result_str);

		protected:
			/*!
			Set to store authentication result objects for container.
			*/
			std::set<authentication_result_ptr> authentication_result_set_;

			/*!
			Mutex to use for authentication_result_set_ operations.
			*/
			boost::mutex authentication_result_set_mutex_;
			
		};
	}
}

#endif

