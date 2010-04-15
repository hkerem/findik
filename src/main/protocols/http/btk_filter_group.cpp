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

#include "btk_filter_group.hpp"

#include "btk_filter.hpp"
#include "http_connect_filter.hpp"

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			std::string btk_filter_group::filter_code_ = "btk";	

			// constructor definition of filter service registration inner class
			btk_filter_group::initializer::initializer()
                        {
                                if (FI_CONFIG.server_http_btk())
                                {
                                        btk_filter_group_ptr bfgp(new btk_filter_group());
                                        FI_SERVICES->filter_srv().register_filter(filter_code_, bfgp);
                                }
                        }

                        btk_filter_group::initializer btk_filter_group::initializer::instance;

			btk_filter_group::btk_filter_group()
			{
				btk_filter_ptr p1(new btk_filter());
				http_connect_filter_ptr p2(new http_connect_filter());

				filter_list_.push_back(p1);
				filter_list_.push_back(p2);
			}

			btk_filter_group::~btk_filter_group()
			{}

		}
	}
}

