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

#include "filter_group.hpp"

namespace findik
{
	namespace filter
	{
		boost::tuple<bool, filter_reason_ptr> 
			filter_group::filter(findik::io::connection_ptr connection_, unsigned int param)
		{
			std::list<abstract_filter_ptr>::iterator it;

			for (it = filter_list_.begin();
				it != filter_list_.end(); it++)
			{
				abstract_filter_ptr afp = *it;

				if (afp->is_applicable(connection_))
				{
					bool result_;
					filter_reason_ptr frp_;

					boost::tie(result_, frp_) = afp->filter(connection_, param);

					if (!result_)
					{
						return boost::make_tuple(result_, frp_);
					}
				}
			}

			filter_reason_ptr frp__;
			return boost::make_tuple(true, frp__);
		}

		bool filter_group::is_applicable(findik::io::connection_ptr connection_)
		{
			return true;
		}

	}
}

