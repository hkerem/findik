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

#ifndef FINDIK_IO_CONNECTION_HPP
#define FINDIK_IO_CONNECTION_HPP

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/thread/mutex.hpp>

#include <boost/asio.hpp>

#include <boost/logic/tribool.hpp>

#include <deque>
#include <map>

#include "log.hpp"

#include "protocol.hpp"
#include "abstract_data.hpp"
#include "authentication_result_container.hpp"

namespace findik
{
	namespace io
	{
		/*!
		Generic connection class for handling data connections and proxying data transmission between them.
		\extends boost::enable_shared_from_this<connection> to use boost shared pointers.
		\extends findik::authenticator::authentication_result_container to be able to store findik authentication result objects.
		@author H. Kerem Cevahir (shelta)
		*/
		class connection :
			public boost::enable_shared_from_this<connection>,
			public findik::authenticator::authentication_result_container
		{
		public:
			/*!
			Constructor.
			\param proto protocol of connection.
			*/
			explicit connection( protocol proto );

			/*!
			Default d-tor.
			*/
			~connection();

			/*!
			Start processing connection.
			After constructor has been called connection instance will wait till this 
			method has been called. After this method called connection will register 
			itself into services and start processing. 
			*/
			void start_processing();

			/*!
			Protocol of this connection.
			\returns protocol of this connection.
			*/
			enum protocol proto();

			/*!
			Pointer to recieved (remote or local) new data.
			\returns currently proccessed data.
			*/
			abstract_data_ptr current_data();

			/*!
			Overwrites new_data pointer with given.
			\param data_ new value to replace new_data_ pointer.
			*/
			void update_current_data(abstract_data_ptr data_);

			/*!
			Socket for the local connection.
			\returns local socket
			*/
			virtual boost::asio::ip::tcp::socket::lowest_layer_type & local_socket() = 0;

			/*!
			Endpoint of local connection.
			\returns local endpoint
			*/
			const boost::asio::ip::address & local_endpoint();

			/*!
			Remote hostname.
			\returns remote hostname to connect.
			*/
			const std::string & remote_hostname();

			/*!
			Remote port.
			\returns remote port to connect.
			*/
			unsigned int remote_port();

			/*!
			Whether connection is keep alive.
			\returns whether connection is keepalive supported.
			*/
			bool is_keepalive();

			/*!
			Data queue of connection.
			\returns data queue of connection.
			*/
			const std::deque<abstract_data_ptr> & data_queue();

			/*!
			Handle completion of a local handshake.
			\param err error code if there is an error
			*/
			virtual void handle_handshake_local(const boost::system::error_code& err) = 0;

			/*!
			Handle completion of a remote handshake.
			\param err error code if there is an error
			*/
			virtual void handle_handshake_remote(const boost::system::error_code& err) = 0;

			/*!
			Handle completion of a local shutdown.
			\param err error code if there is an error
			*/
			virtual void handle_shutdown_local(const boost::system::error_code& err) = 0;

			/*!
			Handle completion of a remote shutdown.
			\param err error code if there is an error
			*/
			virtual void handle_shutdown_remote(const boost::system::error_code& err) = 0;

			/*!
			Handle completion of a local read operation.

			\param e error code if there is an error
			\param bytes_transferred number of transferred bytes
			*/
			void handle_read_local(const boost::system::error_code& err,
				std::size_t bytes_transferred);

			/*!
			Handle completion of a remote write operation.
			
			\param e error code if there is an error
			*/
			void handle_write_remote(const boost::system::error_code& err);

			/*!
			Handle completion of a remote write operation.

			\param err error code if there is an error
			\param bytes_transferred number of transferred bytes
			*/
			void handle_read_remote(const boost::system::error_code& err,
				std::size_t bytes_transferred);

			/*!
			Handle completion of a local write operation.

			\param err error code if there is an error
			*/
			void handle_write_local(const boost::system::error_code& err);

			/*!
			Whether connection is secure.
			
			\return whether connection is secure.
			*/
			bool is_secure();

			/*!
			Set is_tunnel to true.
			*/
			void mark_as_tunnel();

			/*!
			Ends connection.
			Generally called by timers;
			*/
			void close();

			/*!
			String map
			*/
			std::map<unsigned int, std::string> str_map;

			/*!
			UInt map
			*/
			std::map<unsigned int, unsigned int> uint_map;

		protected:
			/*!
			Debug logger for server class.
			*/
                        static log4cxx::LoggerPtr debug_logger;

			/*!
			Protocol of this connection.
			*/
			protocol proto_;

			/*!
			Strand to ensure the connection's handlers are not called concurrently.
			*/
			boost::asio::io_service::strand strand_;

			/*!
			Handle completion of a hostname resolve operation of.

			\param err error code if there is an error
			\param endpoint_iterator an object to iterate over resolve results.
			*/
			void handle_resolve_remote(const boost::system::error_code& err,
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

			/*!
			Handle completion of a connect operation to a remote endpoint.

			\param err error code if there is an error
			\param endpoint_iterator an object to iterate over resolve results. This will be used when io subsystem cannot connect to current endpoint.
			*/
			void handle_connect_remote(const boost::system::error_code& err,
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

			/*!
			This method forms the bridge between transport layer communication and application layer communication.
			For remote.
			*/
			virtual void start_remote() = 0;

			/*!
			This method forms the bridge between transport layer communication and application layer communication.
			For local.
			*/
			virtual void start_local() = 0;

			/*!
			Previously recieved datas. With a max size.
			*/
			std::deque<abstract_data_ptr> data_queue_;

			/*!
			Pointer to recieved (remote or local) new data.
			*/
			abstract_data_ptr new_data_;

			/*!
			Prepare socket for operation. 
			This methods set some socket options: NIO, TCP_NO_DELAY, NO_LINGER.
			\param socket ASIO socket to set options.
			*/
			void prepare_socket(boost::asio::ip::tcp::socket::lowest_layer_type & socket);

			/*!
			Remaining data in local buffer.
			Will be used in pipeling requests.
			*/
			std::size_t local_buffer_remaining_;

			/*!
			Resume point in local buffer.
			Will be used in pipeling requests.
			*/
			char * local_buffer_resume_point_;

			/*!
			Local read buffer.
			*/
			boost::array<char, 16*1024> local_read_buffer_;

			/*!
			Remote read buffer.
			*/
			boost::array<char, 64*1024> remote_read_buffer_;

			/*!
			Local write buffer.
			*/
			boost::asio::streambuf local_write_buffer_;

			/*!
			Remote write buffer.
			*/
			boost::asio::streambuf remote_write_buffer_;

			/*!
			Register to ASIO service to read from local socket.
			*/
			void register_for_local_read();

			/*!
			Register to ASIO service to read from local socket.
			Internal IO operations.
			*/
			virtual void register_for_local_read_io() = 0;

			/*!
			Register to ASIO service to resolve a hostname.
			\param hostname hostname to resolve.
			*/
			void register_for_resolve(const std::string & hostname, unsigned int port);

			/*!
			Remote hostname.
			*/
			std::string remote_hostname_;

			/*!
			Remote port.
			*/
			unsigned int remote_port_;

			/*!
			Register to ASIO service to write local_write_buffer_ to local socket.
			*/
			void register_for_local_write();

			/*!
			Register to ASIO service to write local_write_buffer_ to local socket.
			Internal IO operations.
			*/
			virtual void register_for_local_write_io() = 0;

			/*!
			Register to ASIO service to write specified data to local socket.
			\param data_ fron iterator of data.
			\param size_ number of bytes to write
			*/
			void register_for_local_write(char * data_, std::size_t size_);

			/*!
			Register to ASIO service to write specified data to local socket.
			Internal IO operations.
			\param data_ fron iterator of data.
			\param size_ number of bytes to write
			*/
			virtual void register_for_local_write_io(char * data_, std::size_t size_) = 0;

			/*!
			Register to ASIO service to write specified data to remote socket.
			\param data_ fron iterator of data.
			\param size_ number of bytes to write
			*/
			void register_for_remote_write(char * data_, std::size_t size_);

			/*!
			Register to ASIO service to write specified data to remote socket.
			Internal IO operations.
			\param data_ fron iterator of data.
			\param size_ number of bytes to write
			*/
			virtual void register_for_remote_write_io(char * data_, std::size_t size_) = 0;

			/*!
			Socket for the remote connection.
			\returns remote socket
			*/
			virtual boost::asio::ip::tcp::socket::lowest_layer_type & remote_socket() = 0;

			/*!
			Register to ASIO service to connect an endpoint.
			\param endpoint endpoint to connect.
			*/
			void register_for_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator);

			/*!
			Register to ASIO service to write remote_write_buffer_ to remote socket.
			*/
			void register_for_remote_write();

			/*!
			Register to ASIO service to write remote_write_buffer_ to remote socket.
			Internal IO operations.
			*/
			virtual void register_for_remote_write_io() = 0;

			/*!
			Register to ASIO service to read from remote socket.
			*/
			void register_for_remote_read();

			/*!
			Register to ASIO service to read from remote socket.
			Internal IO operations.
			*/
			virtual void register_for_remote_read_io() = 0;

			/*!
			Timer to keep track of keepalive timeout.
			*/
			boost::asio::deadline_timer keepalive_timer_;

			/*!
			Register to keepalive_timer_ to timeout.
			*/
			void register_for_keepalive_timeout();

			/*!
			Cancel all timeouts on keepalive_timer_.
			*/
			void cancel_keepalive_timeout();

			/*!
			Timer to keep track of local receive timeout.
			*/
			boost::asio::deadline_timer local_receive_timer_;

			/*!
			Register to local_receive_timer_ to timeout.
			*/
			void register_for_local_receive_timeout();

			/*!
			Cancel all timeouts on local_receive_timer_.
			*/
			void cancel_local_receive_timeout();

			/*!
			Timer to keep track of remote receive timeout.
			*/
			boost::asio::deadline_timer remote_receive_timer_;

			/*!
			Register to remote_receive_timer_ to timeout.
			*/
			void register_for_remote_receive_timeout();

			/*!
			Cancel all timeouts on remote_receive_timer_.
			*/
			void cancel_remote_receive_timeout();

			/*!
			Handle timeout of a timer.

			\param err error code if there is an error
			*/
			void handle_socket_timeout(const boost::system::error_code& err);

			/*!
			Whether connection is keep alive.
			*/
			boost::tribool is_keepalive_;

			/*!
			Whether connection is a tunnel.
			This will be used in some io handles.
			\returns whether connection is in a stream.
			*/
			bool is_tunnel();

			/*!
			Whether connection is a tunnel.
			*/
			bool is_tunnel_;

			/*!
			Whether connection is in a stream.
			This will be used in some io handles.
			\returns whether connection is in a stream.
			*/
			bool is_streaming();

			/*!
			Whether connection is in a stream.
			*/
			bool is_streaming_;

			/*!
			Set is_streaming to true.
			*/
			void mark_as_streaming();
			
			/*!
			Set is_streaming to false.
			*/
			void mark_as_not_streaming();

			/*!
			Shutdown local communication.  
			*/
			virtual void shutdown_local() = 0;

			/*!
			Shutdown remote communication.  
			*/
			virtual void shutdown_remote() = 0;

			/*!
			Shutdown TCP socket.
			\param socket to shutdown.
			*/
			void shutdown_socket(boost::asio::ip::tcp::socket::lowest_layer_type & socket);

			/*!
			Shutdown local TCP socket.  
			*/
			void shutdown_local_socket();

			/*!
			Shutdown remote TCP socket.  
			*/
			void shutdown_remote_socket();

			/*!
			Pushes current data to queue and sets new_data_ to NULL.
			*/
			void push_current_data_to_queue();

			/*!
			Whether connection is secure.
			*/
			bool is_secure_;

			/*!
			Local endpoint.
			*/
			boost::asio::ip::address local_endpoint_;

		};
		
		typedef boost::shared_ptr<connection> connection_ptr;
	}
}

#endif

