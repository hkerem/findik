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

#ifndef FINDIK_IO_SESSION_HPP
#define FINDIK_IO_SESSION_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <deque>

#include <boost/thread/mutex.hpp>

#include "protocol.hpp"
#include "connection.hpp"
#include "authentication_result_container.hpp"

namespace findik
{
	namespace io
	{
		/*!
		Generic session class in order to store previous connections for one local endpoint.
		\extends boost::enable_shared_from_this<session> to use boost shared pointers.
		\extends findik::authenticator::authentication_result_container to be able to store findik authentication result objects.
		@author H. Kerem Cevahir (shelta)
		*/
		class session :
			public boost::enable_shared_from_this<session>,
			public findik::authenticator::authentication_result_container 
		{
		public:
			/*!
			Default constructor.
			*/
			session(findik::io::protocol proto);

			/*!
			Destructor.
			*/
			~session();

			/*!
			Protocol of session implementation.
			\returns protocol of session implementation.
			*/
			protocol proto();

			/*!
			To store connection in session.	
			\param connection_
			*/
			void add(connection_ptr connection_);

			/*!
			Queue to store previous connections for this session.
			\returns connection queue.
			*/
			const std::deque<connection_ptr> & connection_queue();

		protected:
			/*!
			Protocol of session implementation.
			*/
			protocol proto_;

			/*!
			Queue to store previous connections for this session.
			*/
			std::deque<connection_ptr> connection_queue_;

			/*!
			Mutex to use when operating on connection queue.
			*/
			boost::mutex connection_queue_mutex_;
		};
		
		typedef boost::shared_ptr<session> session_ptr;
	}
}

#endif

