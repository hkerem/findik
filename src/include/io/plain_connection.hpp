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

#ifndef FINDIK_IO_PLAIN_CONNECTION_HPP
#define FINDIK_IO_PLAIN_CONNECTION_HPP

#include <boost/noncopyable.hpp>

#include "connection.hpp"

namespace findik
{
	namespace io
	{
		/*!
		Plain connection class for handling data connections and proxying data transmission between them.
		\extends boost::noncopyable because of containing i/o members, copying this class is not safe.
		\extends findik::io::connection 
		@author H. Kerem Cevahir (shelta)
		*/
		class plain_connection :
			private boost::noncopyable,
			public connection
		{
		public:
			/*!
			Constructor.
			\param proto protocol of connection.
			*/
			explicit plain_connection( protocol proto );

			/*!
			Socket for the local connection.
			\returns local socket
			*/
			boost::asio::ip::tcp::socket::lowest_layer_type & local_socket();

		protected:
			/*!
			Debug logger for server class.
			*/
                        static log4cxx::LoggerPtr debug_logger;

			/*!
			Socket for the local connection.
			*/
			boost::asio::ip::tcp::socket local_socket_;

			/*!
			Socket for the remote connection.
			*/
			boost::asio::ip::tcp::socket remote_socket_;

			/*!
			Socket for the remote connection.
			\returns remote socket
			*/
			boost::asio::ip::tcp::socket::lowest_layer_type & remote_socket();

			/*!
			Register to ASIO service to read from local socket.
			Internal IO operations.
			*/
			void register_for_local_read_io();

			/*!
			Register to ASIO service to write local_write_buffer_ to local socket.
			Internal IO operations.
			*/
			void register_for_local_write_io();

			/*!
			Register to ASIO service to write specified data to local socket.
			Internal IO operations.
			\param data_ fron iterator of data.
			\param size_ number of bytes to write
			*/
			void register_for_local_write_io(char * data_, std::size_t size_);

			/*!
			Register to ASIO service to write remote_write_buffer_ to remote socket.
			Internal IO operations.
			*/
			void register_for_remote_write_io();

			/*!
			Register to ASIO service to write specified data to remote socket.
			Internal IO operations.
			\param data_ fron iterator of data.
			\param size_ number of bytes to write
			*/
			void register_for_remote_write_io(char * data_, std::size_t size_);

			/*!
			Register to ASIO service to read from remote socket.
			Internal IO operations.
			*/
			void register_for_remote_read_io();

			/*!
			Shutdown local communication.  
			*/
			void shutdown_local();

			/*!
			Shutdown remote communication.  
			*/
			void shutdown_remote();

			/*!
			This method forms the bridge between transport layer communication and application layer communication.
			For remote.
			*/
			void start_remote();

			/*!
			This method forms the bridge between transport layer communication and application layer communication.
			For local.
			*/
			void start_local();

			/*!
			Must implement. N/A for plain connection. Empty method.
			\param err error code if there is an error
			*/
			void handle_handshake_local(const boost::system::error_code& err);

			/*!
			Must implement. N/A for plain connection. Empty method.
			\param err error code if there is an error
			*/
			void handle_handshake_remote(const boost::system::error_code& err);

			/*!
			Must implement. N/A for plain connection. Empty method.
			\param err error code if there is an error
			*/
			void handle_shutdown_local(const boost::system::error_code& err);

			/*!
			Must implement. N/A for plain connection. Empty method.
			\param err error code if there is an error
			*/
			void handle_shutdown_remote(const boost::system::error_code& err);
		};
	}
}

#endif

