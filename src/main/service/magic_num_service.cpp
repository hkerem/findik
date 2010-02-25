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
#include "magic_num_service.hpp"
#ifdef HAVE_MAGIC

namespace findik
{
	namespace service
	{
		magic_num_service::magic_num_service() 
		{}

		magic_num_service::~magic_num_service() 
		{}

		void magic_num_service::start()
		{
			magic_mime = magic_open( MAGIC_MIME );
			if( !magic_mime ){
				magic_close(magic_mime);
			}
			int rc = magic_load( magic_mime, NULL );
			if( rc == -1) {
				magic_close(magic_mime);
			}
		}

		void magic_num_service::get_magic_number(const std::vector<char> & data, std::string & mime_type)
		{
			if(data.size() > 0)
			{
				const char * result_magic_ = NULL;
				{
					boost::mutex::scoped_lock lock1(magic_mime_mutex_);
					result_magic_ = magic_buffer( magic_mime, &(data[0]), data.size() );
				}

				if ( result_magic_ != NULL )
				{
					mime_type = result_magic_;
				}
				else
				{
					mime_type = "application/x-unknown";
				}
			}
		}

	}
}
#endif /* HAVE_MAGIC */
