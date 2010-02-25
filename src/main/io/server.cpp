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

#include "server.hpp"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <string>

#include "services.hpp"

#include "plain_connection.hpp"
#ifdef HAVE_SSL
#include "ssl_connection.hpp"
#endif

namespace findik
{
	namespace io
	{
		log4cxx::LoggerPtr server::debug_logger(log4cxx::Logger::getLogger("findik.io.server"));

		server::server( protocol proto, std::string listen_address,
				unsigned int listen_port, bool is_ssl) :
			protocol_(proto),
			listen_address_(listen_address),
			listen_port_(listen_port),
			is_ssl_(is_ssl),
			acceptor_(FI_SERVICES->io_srv())
		{
			boost::asio::ip::tcp::resolver::query query(
				listen_address_, boost::lexical_cast<std::string>(listen_port_));
			boost::asio::ip::tcp::endpoint endpoint = 
				*(FI_SERVICES->resolver_srv().resolve(query));
			acceptor_.open(endpoint.protocol());
			acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
			acceptor_.bind(endpoint);
			acceptor_.listen();

			create_new_connection_and_register();
		}

		server::~server()
		{}

		void server::create_new_connection_and_register()
		{
			LOG4CXX_DEBUG(debug_logger, "Creating a new connection object and registering for next accept");
			if (is_ssl_)
			{
#ifdef HAVE_SSL
				new_connection_.reset(new ssl_connection(protocol_));
#endif 
			}
			else
			{
				new_connection_.reset(new plain_connection(protocol_)); 
			}

			acceptor_.async_accept(new_connection_->local_socket(),
					boost::bind(&server::handle_accept, this,
						boost::asio::placeholders::error)
				);
		}


		void server::handle_accept(const boost::system::error_code& e)
		{
			new_connection_->start_processing();
			create_new_connection_and_register();
		}

	}
}

