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

#include "util_service.hpp"

namespace findik
{
	namespace service
	{
		util_service::util_service()
		{}

		util_service::~util_service() 
		{}

		void util_service::start()
		{
#ifdef HAVE_PCRE
			pcre_srv_.start();
#endif
#ifdef HAVE_MAGIC
			magic_num_srv_.start();
#endif
		}
#ifdef HAVE_PCRE
		pcre_service & util_service::pcre()
		{
			return pcre_srv_;
		}
#endif
#ifdef HAVE_MAGIC
		magic_num_service & util_service::magic_num()
                {
                        return magic_num_srv_;
                }
#endif
		
	}
}

