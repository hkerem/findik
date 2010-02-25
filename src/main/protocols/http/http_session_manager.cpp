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

#include "http_session_manager.hpp"

#include "services.hpp"

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			http_session_manager::initializer::initializer()
			{
				http_session_manager_ptr p(new http_session_manager());

				FI_SERVICES->session_srv().register_session_manager(findik::io::http, p);
			}

			http_session_manager::initializer http_session_manager::initializer::instance;

			bool http_session_manager::is_associated(findik::io::session_ptr session_, findik::io::connection_ptr connection_)
			{
				return session_->connection_queue().front()->local_endpoint() == connection_->local_endpoint();
			}
		}
	}
}

