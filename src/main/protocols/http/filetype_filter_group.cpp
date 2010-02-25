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
#include "filetype_filter_group.hpp"
#include "file_ext_filter.hpp"
#include "content_mime_filter.hpp"


namespace findik
{
	namespace protocols
	{
		namespace http
		{
			std::string filetype_filter_group::filter_code_ = "filetype";	

			// constructor definition of filter service registration inner class
			filetype_filter_group::initializer::initializer()
                        {
                                filetype_filter_group_ptr ffgp(new filetype_filter_group());

                                FI_SERVICES->filter_srv().register_filter(filter_code_, ffgp);
                        }

                        filetype_filter_group::initializer filetype_filter_group::initializer::instance;

			filetype_filter_group::filetype_filter_group()
			{
#ifdef HAVE_FILEEXT
				file_ext_filter_ptr p1(new file_ext_filter());
				filter_list_.push_back(p1);
#endif
#ifdef HAVE_MAGIC
				content_mime_filter_ptr p2(new content_mime_filter());
				filter_list_.push_back(p2);
#endif
			}

			filetype_filter_group::~filetype_filter_group()
			{}

		}
	}
}

