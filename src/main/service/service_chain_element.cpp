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

#include "service_chain_element.hpp"

namespace findik
{
	namespace service
	{
		findik::io::protocol service_chain_element::proto()
		{
			return proto_;
		}

		bool service_chain_element::is_local()
		{
			return is_local_;
		}

		bool service_chain_element::is_remote()
		{
			return is_remote_;
		}
	}
}

