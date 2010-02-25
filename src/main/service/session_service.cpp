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

#include "session_service.hpp"
#include "services.hpp"

namespace findik
{
	namespace service
	{
		session_service::session_service()
		{}

		session_service::~session_service()
		{}

		findik::io::session_ptr session_service::get_session(findik::io::connection_ptr connection_)
		{
			boost::mutex::scoped_lock session_queue_lock(session_queue_mutex_);

			std::deque<findik::io::session_ptr>::iterator it = session_queue_.begin();
			while (it != session_queue_.end())
			{
				if ( (*it)->proto() == connection_->proto() &&
					session_manager_map_[connection_->proto()]->is_associated(*it, connection_))
				{
					findik::io::session_ptr associated_session = *it;
					session_queue_.erase(it);
					session_queue_.push_front(associated_session);
					associated_session->add(connection_);
					return associated_session;
				}
				*it++;
			}

			findik::io::session_ptr new_session(new findik::io::session(connection_->proto()));
			if (session_queue_.size() == FI_CONFIG.server_max_session())
				session_queue_.pop_back();
			session_queue_.push_front(new_session);
			new_session->add(connection_);
			return new_session;
		}

		void session_service::register_session_manager(findik::io::protocol proto, 
				findik::parser::abstract_session_manager_ptr session_manager)
		{
			session_manager_map_[proto] = session_manager;
		}
	}
}

