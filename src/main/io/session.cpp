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

#include "session.hpp"
#include "services.hpp"

namespace findik
{
	namespace io
	{
		session::session(protocol proto) :
			proto_(proto)
		{}

		session::~session()
		{}

		protocol session::proto()
		{
			return proto_;
		}

		void session::add(connection_ptr connection_)
		{
			boost::mutex::scoped_lock connection_queue_lock(connection_queue_mutex_);
			
			if (connection_queue_.size() == FI_CONFIG.server_max_connection_per_session()) 
				connection_queue_.pop_back();

			connection_queue_.push_front(connection_);
		}

		const std::deque<connection_ptr> & session::connection_queue()
		{
			return connection_queue_;
		}
	}
}

