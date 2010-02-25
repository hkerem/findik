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

#include "content_filter_group.hpp"

#include "domain_filter.hpp"
#include "url_filter.hpp"

#ifdef HAVE_PCRE
#include "domain_re_filter.hpp"
#include "url_re_filter.hpp"
#include "html_content_filter.hpp"
#endif

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			std::string content_filter_group::filter_code_ = "content";	

			// constructor definition of filter service registration inner class
			content_filter_group::initializer::initializer()
                        {
                                content_filter_group_ptr cfgp(new content_filter_group());

                                FI_SERVICES->filter_srv().register_filter(filter_code_, cfgp);
                        }

                        content_filter_group::initializer content_filter_group::initializer::instance;

			content_filter_group::content_filter_group()
			{
				domain_filter_ptr p1(new domain_filter());
				url_filter_ptr p3(new url_filter());
#ifdef HAVE_PCRE
				domain_re_filter_ptr p2(new domain_re_filter());
				url_re_filter_ptr p4(new url_re_filter());
				html_content_filter_ptr p5(new html_content_filter());
#endif

				filter_list_.push_back(p1);
				filter_list_.push_back(p3);
#ifdef HAVE_PCRE
				filter_list_.push_back(p2);
				filter_list_.push_back(p4);
				filter_list_.push_back(p5);
#endif
			}

			content_filter_group::~content_filter_group()
			{}

		}
	}
}

