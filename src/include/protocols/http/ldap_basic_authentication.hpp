/*
  Copyright (C) 2008 Burak OGUZ (barfan) <findikmail@gmail.com>

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
#ifdef HAVE_LDAP

#ifndef FINDIK_AUTHENTICATOR_LDAP_BASIC_AUTHENTICATOR_HPP
#define FINDIK_AUTHENTICATOR_LDAP_BASIC_AUTHENTICATOR_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

#include "abstract_authenticator.hpp"
#include "reply_service.hpp"
#include "request.hpp"
#include "service_container.hpp"
#include "protocol.hpp"
#include "passwd_hash.hpp"

#include <string>
#include <map>

namespace findik
{
	namespace protocols 
	{
		namespace http
		{
			/*!
			  Abstract authenticator object to inherit in order to implement authenticators for findik.
			  \extends boost::enable_shared_from_this<ldap_basic_authentication> to use boost shared pointers.
			  \extends findik::authenticator::abstract_authenticator 
			  @author Burak OGUZ (barfan)
			 */
			class ldap_basic_authentication :
				public boost::enable_shared_from_this<ldap_basic_authentication>,
				public findik::authenticator::abstract_authenticator
			{
				public:
					/*!
					  Authenticates user on the connection via Aactive Directory Kerberos v5 Negotiation System.
					  \param connection_ connection contains new data to be inspected.
					  \returns a tuple containing whether content should be authenticated or not and the authentication object.
					 */
					boost::tuple<bool, findik::authenticator::authentication_result_ptr> authenticate(findik::io::connection_ptr connection_);

					/*!
					  Returns HTTP

					  \returns HTTP.
					 */
					findik::io::protocol proto();

					class initializer
					{
						protected:
							/*!
							  Default constructor.
							  Construction of this object will register a domain_filter instance to filter_service.
							 */
							initializer();

							/*!
							  Instance to create initializer once.
							 */
							static initializer instance;
					};


				protected:
					static log4cxx::LoggerPtr debug_logger_;
	
					static int authenticator_code_;

					static std::string ldap_server_;

					static std::string bind_dn_name_;

					//static BerValue bind_dn_creds; 
					
					static std::string base_dn_;

					static std::string extra_filter_;

					static std::string username_attr_;

					static std::string password_attr_;

					static char *result_attrs_[2];
				
					static struct timeval tv_; 

					static std::map<std::string,std::string> auth_cache_;
					

			};

			typedef boost::shared_ptr<ldap_basic_authentication> ldap_basic_authentication_ptr;
		}
	}
}

#endif
#endif /* HAVE_LDAP */
