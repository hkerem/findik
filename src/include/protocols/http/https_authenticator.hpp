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

#ifndef FINDIK_AUTHENTICATOR_HTTPS_AUTHENTICATOR_HPP
#define FINDIK_AUTHENTICATOR_HTTPS_AUTHENTICATOR_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "abstract_authenticator.hpp"

namespace findik
{
	namespace protocols 
	{
		namespace http
		{
			/*!
			  Authenticator to implement ssl tmp accept authentication checks.
			  \extends boost::enable_shared_from_this<ad_kerb_negotiator> to use boost shared pointers.
			  \extends findik::authenticator::abstract_authenticator 
			  @author H. Kerem Cevahir (shelta)
			 */
			class https_authenticator :
				public boost::enable_shared_from_this<https_authenticator>,
				public findik::authenticator::abstract_authenticator
			{
				public:
					/*!
					  Used for ssl temp accept authentications.
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
					static log4cxx::LoggerPtr debug_logger;

			};

			typedef boost::shared_ptr<https_authenticator> https_authenticator_ptr;
		}
	}
}

#endif

