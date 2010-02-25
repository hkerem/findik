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

#include "authentication_result_container.hpp"

namespace findik
{
	namespace authenticator
	{
		void authentication_result_container::store_authentication(authentication_result_ptr authentication)
		{
			boost::mutex::scoped_lock authentication_result_set_mutex_lock(authentication_result_set_mutex_);

			authentication_result_set_.insert(authentication);
		}

		bool authentication_result_container::is_already_authenticated(unsigned int code, std::string result_str)
		{
			boost::mutex::scoped_lock authentication_result_set_mutex_lock(authentication_result_set_mutex_);

			std::set<authentication_result_ptr>::iterator it = authentication_result_set_.begin();

			while ( it != authentication_result_set_.end() )
			{
				if ( (*it)->code() == code && (*it)->result_str() == result_str )
					return true;
				*it++;
			}

			return false;
		}

	}
}

