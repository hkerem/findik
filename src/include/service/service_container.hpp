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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef FINDIK_SERVICE_SERVICE_CONTAINER_HPP
#define FINDIK_SERVICE_SERVICE_CONTAINER_HPP

#define FI_SERVICES findik::service::service_container::instance()
#define FI_CONFIG findik::service::service_container::instance()->config()

#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "authentication_service.hpp"
#include "session_service.hpp"
#include "filter_service.hpp"
#include "parser_service.hpp"
#include "reply_service.hpp"
#include "util_service.hpp"
#include "tracker_service.hpp"
#include "configuration_initializer.hpp"
#include "configuration_object.hpp"
#include "dbmanager.hpp"
#ifdef HAVE_MYSQL
#include "mysqldbmanager.hpp"
#endif
#ifdef HAVE_SIMPLEDB
#include "simple_dbmanager.hpp"
#endif

namespace findik
{
	namespace service
	{
		/*!
		Wrapper class for all supplied services. Via this interfaces all component can access to services.
                \extends boost::noncopyable this class has designed to be not copyable.
                \extends boost::enable_shared_from_this<service_container> to use boost shared pointers.
		*/
		class service_container :
                        private boost::noncopyable,
                        public boost::enable_shared_from_this<service_container>
		{
		public:
			/*!
			Destructor.
			*/
			~service_container();

			/*!
			Singleton method.
			\returns singleton instance.
			*/
			static boost::shared_ptr<service_container> instance()
			{
				if (instance_.get() == 0)
					instance_.reset(new service_container());
				return instance_;
			}

			/*!
			Method to start services.
			Some services like db_srv_ needs FI_SERVICES for construction, so we have to create these after initialization.
			*/
			void start();

			/*!
			Method to initialize ssl context including loading default certificate authorities and setting default verification policies.
			*/
			void initialize_ssl_context();

			/*!
			Method to check configuration.
			\returns true if configuration is valid.
			*/
			bool check_config();

			/*!
			Boost ASIO IO Service.
			\returns IO Service.
			*/
			boost::asio::io_service & io_srv();

			/*!
			Boost ASIO SSL Context for local.
			\returns SSL context.
			*/
			boost::asio::ssl::context & local_ssl_context();

			/*!
			Boost ASIO SSL Context for remote.
			\returns SSL context.
			*/
			boost::asio::ssl::context & remote_ssl_context();

			/*!
			Boost ASIO TCP IP resolver instance.
			\returns TCP/IP resolver.
			*/
			boost::asio::ip::tcp::resolver & resolver_srv();

			/*!
			Authentication service instance.
			\returns authentication service instance.
			*/
			authentication_service & authentication_srv();

			/*!
			Session service instance.
			\returns session service instance.
			*/
			session_service & session_srv();

			/*!
			Parser service instance.
			\returns parser service instance.
			*/
			parser_service & parser_srv();

			/*!
			Filter service instance.
			\returns filter service instance.
			*/
			filter_service & filter_srv();

			/*!
			Configuration instance. To read options from config file.
			\returns config instance.
			*/
			findik::config::configuration_object & config();

			/*!
			Reply service instance. To generate replies for local.
			\returns reply service instance.
			*/
			reply_service & reply_srv();

			/*!
			Util service instance. To access utilities.
			\returns util service instance.
			*/
			util_service & util_srv();

			/*!
			DB Manager to access database via findik specific interface.
			\returns dbmanager instasnce.
			*/
			findik::persistency::dbmanager & db_srv();

			/*!
			Tracker service instance. To track connections.
			\returns tracker service instance.
			*/
			tracker_service & tracker_srv();

		protected:

			/*!
			Singleton instance.
			*/
			static boost::shared_ptr<service_container> instance_;

			/*!
			Default constructor.
			*/
			service_container();

			/*!
			Authentication service instance.
			*/
			authentication_service authentication_srv_;

			/*!
			Session service instance.
			*/
			session_service session_srv_;

			/*!
			Parser service instance.
			*/
			parser_service parser_srv_;

			/*!
			Filter service instance.
			*/
			filter_service filter_srv_;

			/*!
			Boost ASIO IO service instance.
			*/
			boost::asio::io_service io_srv_;

			/*!
			Boost ASIO TCP IP resolver instance.
			*/
			boost::asio::ip::tcp::resolver resolver_srv_;

			/*!
			Boost ASIO SSL Context for local.
			*/
			boost::asio::ssl::context local_ssl_context_;

			/*!
			Boost ASIO SSL Context for remote.
			*/
			boost::asio::ssl::context remote_ssl_context_;

#ifdef HAVE_LIBCONFIG
			/*!
			Configuration service instance. To populate configuration object .
			*/
			findik::config::configuration_initializer config_srv_;
#endif

			/*!
			Configuration object instance. To read configuration.
			*/
			findik::config::configuration_object config_;

			/*!
			Util service instance. To access utilities.
			*/
			util_service util_srv_;

			/*!
			Reply service instance. To generate replies for local.
			*/
			reply_service reply_srv_;
#ifdef HAVE_MYSQL
			/*!
			DB Manager to access database via findik specific interface.
			*/
			findik::persistency::mysqldbmanager mysql_db_srv_;
#endif
#ifdef HAVE_SIMPLEDB
                        /*!
                        DB Manager to access database via findik specific interface.
                        */
                        findik::persistency::simple_dbmanager simple_db_srv_;
#endif


			/*!
			Tracker service instance. To track connections.
			*/
			tracker_service tracker_srv_;
		};
		
		typedef boost::shared_ptr<service_container> service_container_ptr;
	}
}

#endif

