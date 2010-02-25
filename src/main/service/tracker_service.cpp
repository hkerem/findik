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

#include "tracker_service.hpp"

#include "services.hpp"

namespace findik
{
	namespace service
	{
		tracker_service::tracker_service()
		{}

		tracker_service::~tracker_service()
		{}

		bool tracker_service::accepting_connection(findik::io::connection_ptr connection_)
		{
			boost::mutex::scoped_lock tracker_map_lock_(tracker_map_mutex_);

			unsigned long ip_ = connection_->local_endpoint().to_v4().to_ulong();
			std::set<findik::io::connection *> & ip_set_ = tracker_map_[ip_];
			ip_set_.insert(connection_.get());

			if ( ip_set_.size() > FI_CONFIG.server_max_concurrent_connections_per_user() )
			{
				return false;
			}

			std::size_t total_ = 0;
			std::map<unsigned long, std::set<findik::io::connection *> >::iterator it;

			for (it = tracker_map_.begin();
				it != tracker_map_.end(); it++)
			{
				total_ += (*it).second.size();
			}

			if ( total_ > FI_CONFIG.server_max_concurrent_connections() )
			{
				return false;
			}

			return true;
		}

		void tracker_service::closing_connection(findik::io::connection_ptr connection_)
		{
			closing_connection(
				connection_->local_endpoint().to_v4().to_ulong(),
				connection_.get()
				);
		}

		void tracker_service::closing_connection(unsigned long ip_, findik::io::connection * connection_)
		{
			boost::mutex::scoped_lock tracker_map_lock_(tracker_map_mutex_);

			std::map<unsigned long, std::set<findik::io::connection *> >::iterator it;
			it = tracker_map_.find(ip_);

			if ( it != tracker_map_.end() )
			{
				std::set<findik::io::connection *> & ip_set_ = (*it).second;

				ip_set_.erase(connection_);

				if (ip_set_.empty())
				{
					tracker_map_.erase(it);
				}
			}
		}
	}
}

