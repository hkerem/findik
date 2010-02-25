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

#ifndef FINDIK_IO_SERVER_HPP
#define FINDIK_IO_SERVER_HPP

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/asio.hpp>

#include <string>

#include "protocol.hpp"
#include "connection.hpp"

#include "log.hpp"

namespace findik
{
	namespace io
	{
		/*!
		Generic server class for accepting connections and generating generalized connection objects.
		\extends boost::noncopyable because of containing i/o members, copying this class is not safe.
		\extends boost::enable_shared_from_this<server> to use boost shared pointers.
		@author H. Kerem Cevahir (shelta)
		*/
		class server :
			private boost::noncopyable,
			public boost::enable_shared_from_this<server>
		{
		public:
			/*!
			Constructor.
			\param proto protocol of server
			\param listen_address local address server will listen to.
			\param listen_port port of local_address server will bind to.
			\param whether server will run over ssl.
			*/
			explicit server( protocol proto, 
				std::string listen_address = "0.0.0.0",
				unsigned int listen_port = 0,
				bool is_ssl = false
				);

			/*!
			Destructor.
			*/
			~server();

		protected:
			/*!
			Debug logger for server class.
			*/
                        static log4cxx::LoggerPtr debug_logger;

			/*!
			Address string which server will bind to.
			*/
			std::string listen_address_;

			/*!
			Port which server will bind to.
			*/
			unsigned int listen_port_;

			/*!
			Protocol of the server.
			*/
			protocol protocol_;

			/*!
			Whether server socket is running over ssl.
			*/
			bool is_ssl_;
			
			/*!
			Acceptor used to listen for incoming connections.
			*/
			boost::asio::ip::tcp::acceptor acceptor_;

			/*!
			Handle completion of an asynchronous accept operation.
			\param e error code of accept operation if there is an error.
			*/
			void handle_accept(const boost::system::error_code& e);

			/*!
			Creates a new connection instance for same protocol with server, then
			registers it to async_accept service of IO.
			*/
			void create_new_connection_and_register();

			/*!
			Pointer to accepted new connection.
			*/
			connection_ptr new_connection_;
		};
		
		typedef boost::shared_ptr<server> server_ptr;
	}
}

#endif

