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

#include "service_container.hpp"

log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("findik"));

namespace findik
{
	namespace service
	{
		service_container::service_container() :
			local_ssl_context_(io_srv_, boost::asio::ssl::context::sslv23_server),
			remote_ssl_context_(io_srv_, boost::asio::ssl::context::sslv23_client),
			resolver_srv_(io_srv_),
#ifdef HAVE_LIBCONFIG
			config_(config_srv_)
#else
			config_()
#endif
		{}

		service_container_ptr service_container::instance_;

		service_container::~service_container() 
		{}

		void service_container::initialize_ssl_context()
		{
			local_ssl_context_.set_options(
					boost::asio::ssl::context::default_workarounds
					| boost::asio::ssl::context::single_dh_use);

			local_ssl_context_.use_certificate_chain_file(FI_CONFIG.ssl_local_certificate());
			local_ssl_context_.use_private_key_file(FI_CONFIG.ssl_local_private_key(), boost::asio::ssl::context::pem);
			local_ssl_context_.use_tmp_dh_file(FI_CONFIG.ssl_local_dh_parameters());

			remote_ssl_context_.set_verify_mode(boost::asio::ssl::context::verify_none);
//			remote_ssl_context_.set_verify_mode(boost::asio::ssl::context::verify_peer);
			remote_ssl_context_.load_verify_file(FI_CONFIG.ssl_remote_ca());
		}

		void service_container::start()
		{
#ifdef HAVE_MYSQL
			mysql_db_srv_.connect();
#endif
			util_srv_.start();
			initialize_ssl_context();
			reply_srv_.start();
		}

		bool service_container::check_config()
		{
#ifdef HAVE_LIBCONFIG
			return config_srv_.check();
#endif
		}

		boost::asio::ssl::context & service_container::local_ssl_context()
		{
			return local_ssl_context_;
		}

		boost::asio::ssl::context & service_container::remote_ssl_context()
		{
			return remote_ssl_context_;
		}

		boost::asio::io_service & service_container::io_srv()
		{
			return io_srv_;
		}

		boost::asio::ip::tcp::resolver & service_container::resolver_srv()
		{
			return resolver_srv_;
		}

		authentication_service & service_container::authentication_srv()
		{
			return authentication_srv_;
		}

		session_service & service_container::session_srv()
		{
			return session_srv_;
		}

		parser_service & service_container::parser_srv()
		{
			return parser_srv_;
		}

		filter_service & service_container::filter_srv()
		{
			return filter_srv_;
		}

		findik::config::configuration_object & service_container::config()
		{
			return config_;
		}

		util_service & service_container::util_srv()
		{
			return util_srv_;
		}

		reply_service & service_container::reply_srv()
		{
			return reply_srv_;
		}

		findik::persistency::dbmanager & service_container::db_srv()
		{
#ifdef HAVE_MYSQL
			return mysql_db_srv_;
#endif
#ifdef HAVE_SIMPLEDB
			return simple_db_srv_;
#endif
		}

		tracker_service & service_container::tracker_srv()
		{
			return tracker_srv_;
		}
	}
}

