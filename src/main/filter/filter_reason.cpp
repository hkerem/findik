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

#include "filter_reason.hpp"

namespace findik
{
	namespace filter
	{
		filter_reason::filter_reason() :
                        reply_str_(" "),
                        log_str_(" ")
		{}

		filter_reason::filter_reason(const std::string & reply_str,const std::string & log_str) :
			reply_str_(reply_str),
			log_str_(log_str)
		{}

		filter_reason::~filter_reason()
		{}

		const std::string & filter_reason::reply_str()
		{
			return reply_str_;
		}

                const std::string & filter_reason::log_str()
		{
			return log_str_;
		}
	}
}

