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

#ifndef FINDIK_SERVICE_TRACKER_SERVICE_HPP
#define FINDIK_SERVICE_TRACKER_SERVICE_HPP

#include <boost/noncopyable.hpp>
#include <map>
#include <set>

#include <boost/thread/mutex.hpp>

#include "connection.hpp"

namespace findik
{
	namespace service
	{
		/*!
		Service for tracking active connections.
                \extends boost::noncopyable this class has designed to be not copyable.
		*/
		class tracker_service :
                        private boost::noncopyable
		{
		public:
			/*!
			Default constructor.
			*/
			tracker_service();

			/*!
			Destructor.
			*/
			~tracker_service();

			/*!
			Starts to track connection. If max connection limit reached returns false.
			\param connection_ to track. 
			\returns false if max connection limit has been reached, else true.
			*/
			bool accepting_connection(findik::io::connection_ptr connection_);

			/*!
			Stops tracking connection.
			\param connection_ to stop tracking. 
			*/
			void closing_connection(findik::io::connection_ptr connection_);

			/*!
			Stops tracking connection.
			\param connection_address_ address of connection to stop tracking.
			*/
			void closing_connection(unsigned long ip_, findik::io::connection * connection_);

		protected:

			/*!
			Mutex to use when operating on tracker_map.
			*/
			boost::mutex tracker_map_mutex_;

			/*!
			Map to store connections in an order.
			*/
			std::map<unsigned long, std::set<findik::io::connection *> > tracker_map_;

		};
	}
}

#endif

