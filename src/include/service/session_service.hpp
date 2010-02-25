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

#ifndef FINDIK_SERVICE_SESSION_SERVICE_HPP
#define FINDIK_SERVICE_SESSION_SERVICE_HPP

#include <boost/noncopyable.hpp>
#include <deque>

#include <boost/thread/mutex.hpp>

#include "session.hpp"
#include "connection.hpp"
#include "abstract_session_manager.hpp"

namespace findik
{
	namespace service
	{
		/*!
		Service for attaching connections to sessions.
                \extends boost::noncopyable this class has designed to be not copyable.
		*/
		class session_service :
                        private boost::noncopyable
		{
		public:
			/*!
			Default constructor.
			*/
			session_service();

			/*!
			Destructor.
			*/
			~session_service();

			/*!
			Executes related algoritm for specified connection and associate with a session object.
			\param connection_ to associate session object. 
			\returns associated session.
			*/
			findik::io::session_ptr get_session(findik::io::connection_ptr connection_);

			/*!
			Registers a session manager for session service in order to parse specified protocol.
			\param proto protocol to use session manager
			\param session_manager to register
			*/
			void register_session_manager(findik::io::protocol proto, 
					findik::parser::abstract_session_manager_ptr session_manager);

		protected:
			/*!
			Queue to store previous sessions.
			When a new connection established, with a protocol specific method, new connection will be 
			attached to a session.
			*/
			std::deque<findik::io::session_ptr> session_queue_;

			/*!
			Mutex to use when operating on session queue.
			*/
			boost::mutex session_queue_mutex_;

			/*!
			Map to store session managers in an order. When session association operation has been requested
			appropriate session manager will be fetched from this map.
			*/
			std::map<findik::io::protocol, findik::parser::abstract_session_manager_ptr> session_manager_map_;

		};
	}
}

#endif

