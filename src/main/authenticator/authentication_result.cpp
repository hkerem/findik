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

#include "authentication_result.hpp"

namespace findik
{
	namespace authenticator
	{
		authentication_result::authentication_result(unsigned int code) :
			code_(code)
		{}

		authentication_result::authentication_result(unsigned int code, const std::string & result_str, unsigned int return_code, bool close_connection, unsigned int protocol) :
			code_(code),
			result_str_(result_str),
			return_code_(return_code),
                        close_connection_(close_connection_),
                        protocol_(protocol)
		{}

		authentication_result::~authentication_result()
		{}

		const std::string & authentication_result::result_str()
		{
			return result_str_;
		}

		unsigned int authentication_result::code()
		{
			return code_;
		}

		unsigned int authentication_result::return_code()
                {
                        return return_code_;
                }

                bool authentication_result::close_connection()
                {
                        return close_connection_;
                }

                unsigned int authentication_result::protocol()
                {
                        return protocol_;
                }

	}
}

