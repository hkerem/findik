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

#include "connection.hpp"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <string>

#include "service_container.hpp"
#include "filter_reason.hpp"
#include "authentication_result.hpp"
#include "reply_service.hpp"

namespace findik
{
	namespace io
	{
                log4cxx::LoggerPtr connection::debug_logger(log4cxx::Logger::getLogger("findik.io.connection"));

		connection::connection( protocol proto ) :
			proto_(proto),
			strand_(FI_SERVICES->io_srv()),
			is_keepalive_(boost::indeterminate),
			remote_port_(0),
			remote_hostname_(""),
			is_streaming_(false),
			is_tunnel_(false),
			local_receive_timer_(FI_SERVICES->io_srv()),
			remote_receive_timer_(FI_SERVICES->io_srv()),
			keepalive_timer_(FI_SERVICES->io_srv()),
			local_buffer_remaining_(0),
			local_buffer_resume_point_(NULL)
		{}

		connection::~connection()
		{
			FI_SERVICES->tracker_srv().closing_connection(
				local_endpoint().to_v4().to_ulong(),
				this
				);
		}

		void connection::prepare_socket(boost::asio::ip::tcp::socket::lowest_layer_type & socket)
		{
		        boost::asio::socket_base::non_blocking_io command(true);
			socket.io_control(command);

			boost::asio::ip::tcp::no_delay no_delay_option(true);
			socket.set_option(no_delay_option);

			boost::asio::socket_base::linger linger_option(false, 0);
			socket.set_option(linger_option);
		}

		void connection::shutdown_socket(boost::asio::ip::tcp::socket::lowest_layer_type & socket)
		{
			if (socket.is_open())
			{
				socket.cancel();

				boost::system::error_code ignored_ec;
				socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
				socket.close(ignored_ec);
			}
		}

		void connection::shutdown_remote_socket()
		{
			shutdown_socket(remote_socket());
		}

		void connection::shutdown_local_socket()
		{
			shutdown_socket(local_socket());
		}

		void connection::register_for_local_read()
		{
			LOG4CXX_DEBUG(debug_logger, "Registering connection for local read.");

			//timeouts
			if (is_keepalive())
			{
				register_for_keepalive_timeout();
			}
			else
			{
				register_for_local_receive_timeout();
			}

			// local pipelining or not
			if (local_buffer_resume_point_ == NULL)
			{
                                if (local_socket().is_open())
                                {
                                        register_for_local_read_io();
                                }
                                else
                                {
                                        LOG4CXX_DEBUG(debug_logger, "Local socket is already closed. Canceling local read.");
                                }
			}
			else
			{
				handle_read_local(boost::system::error_code(), local_buffer_remaining_);
			}
		}

		void connection::register_for_remote_read()
		{
			LOG4CXX_DEBUG(debug_logger, "Registering connection for remote read.");

                        if (remote_socket().is_open())
                        {
                                register_for_remote_receive_timeout();
                                register_for_remote_read_io();
                        }
                        else
                        {
                                LOG4CXX_DEBUG(debug_logger, "Remote socket is already closed. Canceling remote read.");
                        }
		}

		void connection::register_for_resolve(const std::string & hostname, unsigned int port)
		{
			LOG4CXX_DEBUG(debug_logger, "Registering connection for remote resolve.");
			boost::asio::ip::tcp::resolver::query query_(
                                hostname, boost::lexical_cast<std::string>(port)
                                );

			FI_SERVICES->resolver_srv().async_resolve(query_,
				boost::bind(&connection::handle_resolve_remote, shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::iterator));
		}

		void connection::register_for_local_write()
		{
			LOG4CXX_DEBUG(debug_logger, "Registering connection for local write.");

                        if (local_socket().is_open())
                        {
                                register_for_local_write_io();
                        }
                        else
                        {
                                LOG4CXX_DEBUG(debug_logger, "Local socket is already closed. Canceling local write.");
                        }

		}

		void connection::register_for_local_write(char * data_, std::size_t size_)
		{
			LOG4CXX_DEBUG(debug_logger, "Registering connection for local write.");

                        if (local_socket().is_open())
                        {
                                register_for_local_write_io(data_, size_);
                        }
                        else
                        {
                                LOG4CXX_DEBUG(debug_logger, "Local socket is already closed. Canceling local write.");
                        }
		}

		void connection::register_for_remote_write()
		{
			LOG4CXX_DEBUG(debug_logger, "Registering connection for remote write.");

                        if (remote_socket().is_open())
                        {
                                register_for_remote_write_io();
                        }
                        else
                        {
                                LOG4CXX_DEBUG(debug_logger, "Remote socket is already closed. Canceling remote write.");
                        }
		}

		void connection::register_for_remote_write(char * data_, std::size_t size_)
		{
			LOG4CXX_DEBUG(debug_logger, "Registering connection for local write.");

                        if (remote_socket().is_open())
                        {
                                register_for_remote_write_io(data_, size_);
                        }
                        else
                        {
                                LOG4CXX_DEBUG(debug_logger, "Remote socket is already closed. Canceling remote write.");
                        }
		}

		void connection::register_for_connect(boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
		{
			LOG4CXX_DEBUG(debug_logger, "Registering connection for remote connect.");
			boost::asio::ip::tcp::endpoint endpoint = *endpoint_iterator;
			remote_socket().async_connect(endpoint,
				strand_.wrap(boost::bind(&connection::handle_connect_remote, shared_from_this(),
					boost::asio::placeholders::error, ++endpoint_iterator)));
		}

		void connection::register_for_keepalive_timeout()
		{
			unsigned int timeout = 15;

			FI_SERVICES->parser_srv().update_keepalive_timeout_of(shared_from_this(), timeout);

			keepalive_timer_.expires_from_now(boost::posix_time::seconds(timeout));
			keepalive_timer_.async_wait(boost::bind(&connection::handle_socket_timeout, shared_from_this(),
					boost::asio::placeholders::error));
		}

		void connection::cancel_keepalive_timeout()
		{
			keepalive_timer_.cancel();
		}

		void connection::register_for_local_receive_timeout()
		{
			local_receive_timer_.expires_from_now(boost::posix_time::seconds(FI_CONFIG.server_local_receive_timeout()));
			local_receive_timer_.async_wait(boost::bind(&connection::handle_socket_timeout, shared_from_this(),
					boost::asio::placeholders::error));
		}

		void connection::cancel_local_receive_timeout()
		{
			local_receive_timer_.cancel();
		}

		void connection::register_for_remote_receive_timeout()
		{
			remote_receive_timer_.expires_from_now(boost::posix_time::seconds(FI_CONFIG.server_remote_receive_timeout()));
			remote_receive_timer_.async_wait(boost::bind(&connection::handle_socket_timeout, shared_from_this(),
					boost::asio::placeholders::error));
		}

		void connection::cancel_remote_receive_timeout()
		{
			remote_receive_timer_.cancel();
		}

		void connection::handle_socket_timeout(const boost::system::error_code& err)
		{
			if (err != boost::asio::error::operation_aborted)
			{
				close();
			}
		}

		void connection::start_processing()
		{
			if ( FI_SERVICES->tracker_srv().accepting_connection(shared_from_this()) ) // accept connection
			{
				local_endpoint_ = local_socket().remote_endpoint().address();
				start_local();
			}
			else // reject connection, limit reached.
			{ // TODO: log and respond with protocol reject (eg. HTTP 501).
				close();
			}
		}

		void connection::close()
		{
			LOG4CXX_DEBUG(debug_logger, "Closing connection.");
			shutdown_local();
			shutdown_remote();

			FI_SERVICES->tracker_srv().closing_connection(shared_from_this());
		}

                bool connection::is_keepalive()
		{
		//	if (boost::logic::indeterminate(is_keepalive_))
			FI_SERVICES->parser_srv().update_is_keepalive_of(shared_from_this(), is_keepalive_);

			return is_keepalive_;
		}

                bool connection::is_secure()
		{
			return is_secure_;
		}

		unsigned int connection::remote_port()
		{
			if (remote_port_ == 0)
				FI_SERVICES->parser_srv().update_port_of(shared_from_this(), remote_port_);

			return remote_port_;
		}

		const boost::asio::ip::address & connection::local_endpoint()
		{
			if (local_endpoint_.to_v4().to_ulong() == 0)
			{
				local_endpoint_ = local_socket().remote_endpoint().address();
			}
			return local_endpoint_;
		}

		const std::string & connection::remote_hostname()
		{
			if (remote_hostname_ == "")
				FI_SERVICES->parser_srv().update_hostname_of(shared_from_this(), remote_hostname_);

			return remote_hostname_;
		}

		enum protocol connection::proto()
		{
			return proto_;
		}

		abstract_data_ptr connection::current_data()
		{
			return new_data_;
		}

		void connection::mark_as_tunnel()
		{
			is_tunnel_ = true;
		}

		bool connection::is_tunnel()
		{
			return is_tunnel_;
		}

		void connection::mark_as_streaming()
		{
			is_streaming_ = true;
		}

		void connection::mark_as_not_streaming()
		{
			is_streaming_ = false;
		}

		bool connection::is_streaming()
		{
			return is_streaming_;
		}

		void connection::update_current_data(abstract_data_ptr data_)
		{
			new_data_ = data_;
		}

		void connection::push_current_data_to_queue()
		{
			data_queue_.push_front(new_data_);
			new_data_.reset( (abstract_data *) 0 );

			if (data_queue_.size() > FI_CONFIG.connection_queue_size())
				data_queue_.pop_back();
		}

		const std::deque<abstract_data_ptr> & connection::data_queue()
		{
			return data_queue_;
		}

		void connection::handle_read_local(const boost::system::error_code& err,
			std::size_t bytes_transferred)
		{
			LOG4CXX_DEBUG(debug_logger, "Handling local read.");
			// TODO: refinement
			if (err)
			{
				if (err != boost::asio::error::operation_aborted)
					LOG4CXX_ERROR(debug_logger, "Handling local read: " + err.message());
				close();
				return;
			}

			if (is_keepalive())
			{
				cancel_keepalive_timeout();
			}
			else
			{
				cancel_local_receive_timeout();
			}

			if (is_tunnel())
			{
                for (int i = 0; i++; i < bytes_transferred)
                    std::cout << local_read_buffer_.data() + i;
				register_for_remote_write(local_read_buffer_.data(), bytes_transferred);
				return;
			}

			//local pipelining
			char * start_point;
			if (local_buffer_resume_point_ == NULL)
			{
				start_point = local_read_buffer_.data();
			}
			else
			{
				start_point = local_buffer_resume_point_;
			}

			// parsing data.
			boost::tribool parser_result;
			char * resume_point;

			boost::tie(parser_result, resume_point) = 
				FI_SERVICES->parser_srv().parse(
					shared_from_this(), true,
					start_point, start_point + bytes_transferred);


			// whether there are more than one local data (pipelining)
			if (resume_point == start_point + bytes_transferred)
			{
				// no pipelining
				local_buffer_remaining_ = 0;
				local_buffer_resume_point_ = NULL;
			}
			else
			{
				local_buffer_remaining_ = bytes_transferred - (resume_point - start_point)/sizeof(char);
				local_buffer_resume_point_ = resume_point;
			}

			if (parser_result) // successfully parsed
			{
				LOG4CXX_DEBUG(debug_logger, "Handling local read: data successfully parsed.");

				if (current_data()->is_stream())
				{
					mark_as_not_streaming(); // if parser had returned true, all request should have been completed. No need for re-read_local.
					register_for_remote_write(local_read_buffer_.data(), bytes_transferred);
				}
				else
				{
					bool is_authenticated;
					findik::authenticator::authentication_result_ptr authentication_result;
					boost::tie(is_authenticated, authentication_result) =
						FI_SERVICES->authentication_srv().authenticate(shared_from_this());
					
					if (is_authenticated) // authenticated
					{
						// TODO: save creditentials.

						bool filter_result;
						findik::filter::filter_reason_ptr filter_reason;
						boost::tie(filter_result, filter_reason) =
							FI_SERVICES->filter_srv().filter(shared_from_this());
						
						if (filter_result) // not denied
						{
							LOG4CXX_DEBUG(debug_logger, "Accepted local data.");

							if (is_keepalive())
							{
								current_data()->into_buffer(remote_write_buffer_);
								register_for_remote_write();
							}
							else
							{
								register_for_resolve(remote_hostname(), remote_port());
							}
						}
						else // denied
						{
							LOG4CXX_DEBUG(debug_logger, "Data from local had been rejected.");

							FI_SERVICES->reply_srv().reply(local_write_buffer_,
									proto(), filter_reason);
							register_for_local_write();
						}
					}
					else // not authenticated
					{
						LOG4CXX_DEBUG(debug_logger, "Local connection is not authenticated.");

						FI_SERVICES->reply_srv().reply(local_write_buffer_,
								proto(), authentication_result);
						register_for_local_write();
					}
				}
			}
			else if (!parser_result) // bad request
			{
				LOG4CXX_ERROR(debug_logger, "Handling local read: data can not be parsed.");

				// no pipelining: to ensure connection will not go in to an infinate loop for pipelining read 
				local_buffer_remaining_ = 0;
				local_buffer_resume_point_ = NULL;

				FI_SERVICES->reply_srv().reply(local_write_buffer_,
						proto(), FC_BAD_LOCAL);
				register_for_local_write();
			}
			else // more data required
			{
				if ( current_data().get() != 0 && current_data()->is_stream())
				{
					if (is_streaming())
					{
						register_for_remote_write(local_read_buffer_.data(), bytes_transferred);
					}
					else
					{
						mark_as_streaming();

						if (is_keepalive())
						{
							current_data()->into_buffer(remote_write_buffer_);
							register_for_remote_write();
						}
						else
						{
							register_for_resolve(remote_hostname(), remote_port());
						}
					}
				}
				else
				{
	                                register_for_local_read();
				}
			}
		}

		void connection::handle_resolve_remote(const boost::system::error_code& err,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
		{
			LOG4CXX_DEBUG(debug_logger, "Handling remote resolve.");
			if (err)
			{
				if (err != boost::asio::error::operation_aborted)
					LOG4CXX_ERROR(debug_logger, "Handling resolve remote: " + err.message());
				close();
				return;
			}

			register_for_connect(endpoint_iterator);
		}

		void connection::handle_connect_remote(const boost::system::error_code& err,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator)
		{
			if (!err) // connected
			{
				LOG4CXX_DEBUG(debug_logger, "Handling remote connect: successfully connected.");
				prepare_socket(remote_socket());
				start_remote();
			}
			// not connected, but iterator has more elements
			else if (endpoint_iterator != boost::asio::ip::tcp::resolver::iterator()) 
			{
				LOG4CXX_DEBUG(debug_logger, "Handling remote connect: trying next endpoint.");
				remote_socket().close();
				register_for_connect(endpoint_iterator);
			}
			else // can not connect
			{
				LOG4CXX_ERROR(debug_logger, "On remote connect: " + err.message() + " -> " +
						 remote_hostname_);
				close();
				return;
			}
		}

		void connection::handle_write_remote(const boost::system::error_code& err)
		{
			LOG4CXX_DEBUG(debug_logger, "Handling remote write.");
			if (err)
			{
				if (err != boost::asio::error::operation_aborted)
					LOG4CXX_ERROR(debug_logger, "Handling write remote: " + err.message());
				close();
				return;
			}

			if (is_tunnel())
			{
				register_for_local_read();
			}
			else if (is_streaming())
			{
				register_for_local_read();
			}
			else
			{
				push_current_data_to_queue();
				register_for_remote_read();
			}
		}

		void connection::handle_read_remote(const boost::system::error_code& err,
			std::size_t bytes_transferred)
		{
			LOG4CXX_DEBUG(debug_logger, "Handling remote read.");
			if ( (err && err != boost::asio::error::eof) || 
				( err == boost::asio::error::eof &&
					!(current_data().get() != 0 &&
					current_data()->is_expecting_eof() 
				)))
			{
				if (err != boost::asio::error::operation_aborted)
					LOG4CXX_ERROR(debug_logger, "Handling remote read: " + err.message());
				close();
				return;
			}

			cancel_remote_receive_timeout();

			if (is_tunnel())
			{
				// start tunneling
				register_for_local_write(remote_read_buffer_.data(), bytes_transferred);
				return;
			}

                        boost::tribool parser_result;

			if ( err == boost::asio::error::eof && 
				current_data().get() != 0 &&
				current_data()->is_expecting_eof() )
			{
				parser_result = true;
			}
			else 
			{
				// parsing data.
				boost::tie(parser_result, boost::tuples::ignore) =
					FI_SERVICES->parser_srv().parse(
						shared_from_this(), false,
						remote_read_buffer_.data(), remote_read_buffer_.data() + bytes_transferred);
			}

                        if (parser_result) // successfully parsed
                        {
				LOG4CXX_DEBUG(debug_logger, "Handling remote read: data successfully parsed.");

				if (is_tunnel())
				{
					// start tunneling
					LOG4CXX_DEBUG(debug_logger, "Starting tunneling operation.");
					current_data()->into_buffer(local_write_buffer_);
					register_for_local_write();
					register_for_local_read();
					return;
				}

				if (!is_keepalive()) {
					LOG4CXX_DEBUG(debug_logger, "Shutting down remote socket.");
					shutdown_remote();
				}

				if (current_data()->is_stream())
				{
					mark_as_not_streaming(); // if parser had returned true, all response should have been completed. No need for re-read_remote.
					register_for_local_write(remote_read_buffer_.data(), bytes_transferred);
				}
				else
				{
					bool filter_result;
					findik::filter::filter_reason_ptr filter_reason;
	                                boost::tie(filter_result, filter_reason) =
        	                                FI_SERVICES->filter_srv().filter(shared_from_this());

					if (filter_result) // not denied
					{
						LOG4CXX_DEBUG(debug_logger, "Accepted remote data.");
						current_data()->into_buffer(local_write_buffer_);
						register_for_local_write();
					}
					else // denied
					{
						LOG4CXX_DEBUG(debug_logger, "Data from remote had been rejected.");
						FI_SERVICES->reply_srv().reply(local_write_buffer_,
								proto(), filter_reason);
						register_for_local_write();
					}
				}
                        }
                        else if (!parser_result) // bad response
                        {
				LOG4CXX_ERROR(debug_logger, "Handling remote read: data can not be parsed.");

				if (!is_keepalive())
                                        shutdown_remote();

				FI_SERVICES->reply_srv().reply(local_write_buffer_,
						proto(), FC_BAD_REMOTE);
				register_for_local_write();
                        }
                        else // more data required
                        {
				if ( current_data().get() != 0 && current_data()->is_stream())
				{
					if (is_streaming())
					{
						register_for_local_write(remote_read_buffer_.data(), bytes_transferred);
					}
					else
					{
						mark_as_streaming();
						current_data()->into_buffer(local_write_buffer_);
						register_for_local_write();
					}
				}
				else
				{
	                                register_for_remote_read();
				}
                        }
		}

		void connection::handle_write_local(const boost::system::error_code& err)
		{
			LOG4CXX_DEBUG(debug_logger, "Handling local write.");
			if (err)
			{
				if (err != boost::asio::error::operation_aborted)
					LOG4CXX_ERROR(debug_logger, "Handling write local: " + err.message());
				close();
				return;
			}

			if (is_tunnel())
			{
				register_for_remote_read();
			}
			else if (is_streaming())
			{
				register_for_remote_read();
			}
			else if (is_keepalive()) 
			{
				push_current_data_to_queue();
				register_for_local_read();
			} 
			else
			{
				LOG4CXX_DEBUG(debug_logger, "Shutting down local socket.");
				shutdown_local();
			}
		}
	}
}

