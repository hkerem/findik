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

#ifndef FINDIK_CONFIG_CONFIGURATION_OBJECT_HPP
#define FINDIK_CONFIG_CONFIGURATION_OBJECT_HPP

#ifdef HAVE_LIBCONFIG
#include "configuration_initializer.hpp"
#endif

#include <string>

namespace findik 
{
	namespace config 
	{

		/*!
		An object encapsulates configuration parameters.
		@author H. Kerem Cevahir (shelta)
		*/
		class configuration_object
		{
		public:
			/*!
			Default constructor.
			*/
			configuration_object();
#ifdef HAVE_LIBCONFIG
			/*!
			Default constructor.
			*/
			configuration_object(configuration_initializer & config_);
#endif
			/*!
			Default destructor.
			*/
			~configuration_object();

			bool server_http_run_with_squid();

			const std::string & server_http_squid_host();

			unsigned int server_http_squid_port();

			unsigned int server_http_squid_keepalive_timeout();

			unsigned int server_local_receive_timeout();

			unsigned int server_remote_receive_timeout();

			unsigned int server_http_max_object_size();

			unsigned int connection_queue_size();

			const std::string & db_mysql_host();

			const std::string & db_mysql_database();

			const std::string & db_mysql_username();

			const std::string & db_mysql_password();

			unsigned int db_pool_size();

			unsigned int io_number_of_threads();

			const std::string & reply_reply_file();

			const std::string & reply_ssl_reply_file();

			const std::string & ssl_local_private_key();

			const std::string & ssl_local_certificate();

			const std::string & ssl_local_dh_parameters();

			bool ssl_remote_trust_all();

			const std::string & ssl_remote_ca();

			unsigned int server_max_session();

			unsigned int server_max_connection_per_session();

			unsigned int server_max_concurrent_connections();

			unsigned int server_max_concurrent_connections_per_user();

			bool use_clamd();

            const std::string & clamd_host();

            const std::string & clamd_port();

            bool use_ad_kerbv5_negotiation_auth();

            const std::string & ad_service_name();

            const std::string & ad_keytab_file();

			bool use_ldap_basic_auth();
                        
			const std::string & ldap_base_dn();

                        const std::string & ldap_bind_dn_name();

                        const std::string & ldap_bind_dn_password();

			const std::string & ldap_extra_filter();

                        const std::string & ldap_server_name();

                        const std::string & ldap_search_attr();

                        const std::string & ldap_password_attr();
		
			const std::string & ldap_realm();

		protected:

			bool server_http_run_with_squid_;

			std::string server_http_squid_host_;

			unsigned int server_http_squid_port_;

			unsigned int server_http_squid_keepalive_timeout_;

			unsigned int server_local_receive_timeout_;

			unsigned int server_remote_receive_timeout_;

			unsigned int server_http_max_object_size_;

			unsigned int connection_queue_size_;

			std::string db_mysql_host_;

			std::string db_mysql_database_;

			std::string db_mysql_username_;

			std::string db_mysql_password_;

			unsigned int db_pool_size_;

			unsigned int io_number_of_threads_;
			
			std::string reply_reply_file_;

			std::string reply_ssl_reply_file_;

			std::string ssl_local_private_key_;

			std::string ssl_local_certificate_;

			std::string ssl_local_dh_parameters_;

			bool ssl_remote_trust_all_;

			std::string ssl_remote_ca_;

			unsigned int server_max_session_;

			unsigned int server_max_connection_per_session_;

			unsigned int server_max_concurrent_connections_;

			unsigned int server_max_concurrent_connections_per_user_;

			bool use_clamd_;
			
			std::string clamd_host_;
			
			std::string clamd_port_;
			
			bool use_ad_kerbv5_negotiation_auth_;
		
            std::string ad_service_name_;

            std::string ad_keytab_file_;

			bool use_ldap_basic_auth_;
                        
			std::string ldap_base_dn_;

            std::string ldap_bind_dn_name_;

            std::string ldap_bind_dn_password_;

			std::string ldap_extra_filter_;

			std::string ldap_server_name_;

			std::string ldap_search_attr_;
		
			std::string ldap_password_attr_;

			std::string ldap_realm_;

		};
	}
}

#endif

