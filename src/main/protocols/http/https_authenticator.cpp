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

#include "https_authenticator.hpp"
#include "services.hpp"

#include "session.hpp"
#include "request.hpp"

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			// initialization of logger
			log4cxx::LoggerPtr https_authenticator::debug_logger(log4cxx::Logger::getLogger("findik.protocols.http.https_authenticator"));	
			https_authenticator::initializer::initializer()
                        {
                                https_authenticator_ptr p(new https_authenticator());
				FI_SERVICES->authentication_srv().register_authenticator(p);
                        }

                        https_authenticator::initializer https_authenticator::initializer::instance;

			boost::tuple<bool, findik::authenticator::authentication_result_ptr> https_authenticator::authenticate(findik::io::connection_ptr connection_) 
			{
				if (! connection_->is_secure())
					return true;

				findik::io::session_ptr session_ = FI_SERVICES->session_srv().get_session(connection_);

				findik::authenticator::authentication_result_ptr auth = 
					findik::authenticator::authentication_result::create_result(
						FC_SSL_TMPACCEPT, connection_->remote_hostname(), FC_SSL_TMPACCEPT, false, proto()
					);

				// if it has authentication request object that ssl_connection created. this is a workaround
				if ( session_->is_already_authenticated(FC_SSL_TMPACCEPT_REQ, connection_->remote_hostname()) )
				{
					request_ptr req = boost::static_pointer_cast<request>(connection_->current_data());

					std::size_t spos;

					if ( (spos = req->request_path().find(";findik=trust_this_site")) != std::string::npos )
					{
						req->uri = req->request_path().substr(0, spos);
						req->reset_uri_and_path();
						session_->store_authentication(auth);
					}
				}

				return boost::make_tuple(true, auth);
			}

			findik::io::protocol https_authenticator::proto()
			{
				// set this filter to be used in request only
				return findik::io::http;	
			}
		}
	}
}

