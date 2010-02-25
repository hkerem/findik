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

#ifndef FINDIK_AUTHENTICATOR_AUTHENTICATOR_RESULT_HPP
#define FINDIK_AUTHENTICATOR_AUTHENTICATOR_RESULT_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <string>

namespace findik
{
	namespace authenticator
	{
		/*!
		Authentication result type to return for denying request with a authenticator.
		\extends boost::enable_shared_from_this<authentication_result> to use boost shared pointers.
		@author H. Kerem Cevahir (shelta)
		*/
		class authentication_result :
			public boost::enable_shared_from_this<authentication_result>
		{
		public:
			/*!
			Factory method.
			\param code code of result.
			\returns a new authenticator result instance
			*/
			static boost::shared_ptr<authentication_result> create_result(unsigned int code)
			{
				boost::shared_ptr<authentication_result> p(new authentication_result(code));
				return p;
			}

			/*
			Factory method.
			\param code code of result.
			\param result_str detail of result.
			\returns a new authenticator result instance
			*/
			static boost::shared_ptr<authentication_result> create_result(unsigned int code, const std::string & result_str, unsigned int return_code, bool close_connection, unsigned int protocol)
			{
				boost::shared_ptr<authentication_result> p(new authentication_result(code, result_str, return_code, close_connection, protocol));
				return p;
			}

			/*!
			Destructor.
			*/
			~authentication_result();

			/*!
			Detail of result.
			\returns result details
			*/
			const std::string & result_str();

			/*!
			Code of result. 
			\returns result code
			*/
			unsigned int code();

			/*!
                        Code of return.
                        \returns response return code
                        */
                        unsigned int return_code();

                        /*!
                        Close connection parameter if this reply should close connection or not.
                        \returns close connection parameter
                        */
                        bool close_connection();

                        /*!
                        Protocol which reason produced
                        \returns response return code
                        */
                        unsigned int protocol();


		protected:
			/*!
			Constructor.
			*/
			authentication_result(unsigned int code);

			/*!
			Constructor.
			*/
			authentication_result(unsigned int code, const std::string & result_str, unsigned int return_code, bool close_connection, unsigned int protocol);

			/*!
			Result code.
			*/
			unsigned int code_;

			/*!
			Result detail in a string.
			*/
			std::string result_str_;

			/*!
                        Connection close parameter for response header
                        */
                        bool close_connection_;

			/*!
                        Response code.
                        */
                        unsigned int return_code_;				

                        /*!
                        Protocol
                        */
                        unsigned int protocol_;
		};
		
		typedef boost::shared_ptr<authentication_result> authentication_result_ptr;
	}
}

#endif

