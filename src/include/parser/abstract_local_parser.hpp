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

#ifndef FINDIK_PARSER_ABSTRACT_LOCAL_PARSER_HPP
#define FINDIK_PARSER_ABSTRACT_LOCAL_PARSER_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

#include "abstract_parser.hpp"
#include "connection.hpp"

#include <string>

namespace findik
{
	namespace parser
	{
		/*!
		Abstract local parser object to inherit in order to implement local parsers for findik.
		\extends boost::enable_shared_from_this<abstract_local_parser> to use boost shared pointers.
		@author H. Kerem Cevahir (shelta)
		*/
		class abstract_local_parser :
			public boost::enable_shared_from_this<abstract_local_parser>,
			public findik::parser::abstract_parser
		{
		public:
			/*!
			Updates remote_hostname parameter of connection.
			\param connection_ to inspect.
			\param hostname_ to update.
			*/
			virtual void update_hostname_of(findik::io::connection_ptr connection_, std::string & hostname_) = 0;

			/*!
			Updates remote_port parameter of connection.
			\param connection_ to inspect.
			\param port_ to update.
			*/
			virtual void update_port_of(findik::io::connection_ptr connection_, unsigned int & port_) = 0;

		protected:

		};
		
		typedef boost::shared_ptr<abstract_local_parser> abstract_local_parser_ptr;
	}
}

#endif

