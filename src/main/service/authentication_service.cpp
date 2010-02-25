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

#include "authentication_service.hpp"

#include <boost/tuple/tuple.hpp>

namespace findik
{
	namespace service
	{
		authentication_service::authentication_service()
		{}

		authentication_service::~authentication_service()
		{}

		void authentication_service::register_authenticator(findik::authenticator::abstract_authenticator_ptr authenticator_)
		{
			authenticator_list_.push_back(authenticator_);
		}

		boost::tuple<bool, findik::authenticator::authentication_result_ptr> 
			authentication_service::authenticate(findik::io::connection_ptr connection_)
		{
			std::list<findik::authenticator::abstract_authenticator_ptr>::iterator it;

			for ( it=authenticator_list_.begin();
				it != authenticator_list_.end(); it++ )

				if ( (*it)->proto() == connection_->proto() )
				{
					boost::tuple<bool, findik::authenticator::authentication_result_ptr> result = 
						(*it)->authenticate(connection_);
					
					if (!boost::get<0>(result))
						return result;
				}

			//TODO: may be return score for greylisting
			findik::authenticator::authentication_result_ptr arp;
			return boost::make_tuple(true, arp);
		}
	}
}

