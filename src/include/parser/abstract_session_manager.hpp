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

#ifndef FINDIK_PARSER_ABSTRACT_SESSION_MANAGER_HPP
#define FINDIK_PARSER_ABSTRACT_SESSION_MANAGER_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "connection.hpp"
#include "session.hpp"

namespace findik
{
	namespace parser
	{
		/*!
		Abstract session manager object to inherit in order to implement protocol specific session managers for findik.
		\extends boost::enable_shared_from_this<abstract_parser> to use boost shared pointers.
		@author H. Kerem Cevahir (shelta)
		*/
		class abstract_session_manager :
			public boost::enable_shared_from_this<abstract_session_manager>
		{
		public:
                        /*!
			Session manager will use this method in order to determine whether specified session object
			is associated with specified connection object.
			\param session_ to associate.
			\param connection_ to analyze.
			\returns whether connection is associated to session.
                        */
			virtual bool is_associated(findik::io::session_ptr session_, findik::io::connection_ptr connection_) = 0;
		};
		
		typedef boost::shared_ptr<abstract_session_manager> abstract_session_manager_ptr;
	}
}

#endif

