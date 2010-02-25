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

#include "plain_connection.hpp"
#include "services.hpp"

#include <boost/bind.hpp>

namespace findik
{
	namespace io
	{
                log4cxx::LoggerPtr plain_connection::debug_logger(log4cxx::Logger::getLogger("findik.io.plain_connection"));

		plain_connection::plain_connection( protocol proto ) :
			connection(proto),
			local_socket_(FI_SERVICES->io_srv()),
			remote_socket_(FI_SERVICES->io_srv())
		{
			is_secure_ = false;
		}

		void plain_connection::shutdown_local()
		{
			shutdown_local_socket();
		}

		void plain_connection::shutdown_remote()
		{
			shutdown_remote_socket();
		}

		void plain_connection::register_for_local_read_io()
		{
			local_socket_.async_read_some(boost::asio::buffer(local_read_buffer_),
				strand_.wrap(
					boost::bind(&connection::handle_read_local,
						shared_from_this(),
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
		}

		void plain_connection::register_for_remote_read_io()
		{
			remote_socket_.async_read_some(boost::asio::buffer(remote_read_buffer_),
				strand_.wrap(
					boost::bind(&connection::handle_read_remote,
						shared_from_this(),
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred)));
		}

		void plain_connection::register_for_local_write_io()
		{
			boost::asio::async_write(local_socket_, local_write_buffer_,
				strand_.wrap(
					boost::bind(&connection::handle_write_local,
						shared_from_this(),
						boost::asio::placeholders::error)));
		}

		void plain_connection::register_for_local_write_io(char * data_, std::size_t size_)
		{
			boost::asio::async_write(local_socket_, boost::asio::buffer(data_, size_),
				strand_.wrap(
					boost::bind(&connection::handle_write_local,
						shared_from_this(),
						boost::asio::placeholders::error)));
		}

		void plain_connection::register_for_remote_write_io()
		{
			boost::asio::async_write(remote_socket_, remote_write_buffer_,
				strand_.wrap(
					boost::bind(&connection::handle_write_remote,
						shared_from_this(),
						boost::asio::placeholders::error)));
		}

		void plain_connection::register_for_remote_write_io(char * data_, std::size_t size_)
		{
			boost::asio::async_write(remote_socket_, boost::asio::buffer(data_, size_),
				strand_.wrap(
					boost::bind(&connection::handle_write_remote,
						shared_from_this(),
						boost::asio::placeholders::error)));
		}

		void plain_connection::start_local()
		{
			prepare_socket(local_socket_);
			register_for_local_read();
		}

		void plain_connection::start_remote()
		{
			current_data()->into_buffer(remote_write_buffer_);
			register_for_remote_write();
		}

		boost::asio::ip::tcp::socket::lowest_layer_type & plain_connection::local_socket()
		{
			return local_socket_.lowest_layer();
		}

		boost::asio::ip::tcp::socket::lowest_layer_type & plain_connection::remote_socket()
		{
			return remote_socket_.lowest_layer();
		}

		void plain_connection::handle_handshake_local(const boost::system::error_code& err)
		{
		}

		void plain_connection::handle_handshake_remote(const boost::system::error_code& err)
		{
		}

		void plain_connection::handle_shutdown_local(const boost::system::error_code& err)
		{
		}

		void plain_connection::handle_shutdown_remote(const boost::system::error_code& err)
		{
		}
	}
}

