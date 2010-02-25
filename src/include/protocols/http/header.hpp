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

#ifndef FINDIK_PROTOCOLS_HTTP_HEADER_HPP
#define FINDIK_PROTOCOLS_HTTP_HEADER_HPP

#include <string>

namespace findik {
	namespace protocols {
		namespace http {

			/*!
			A struct to store an HTTP Header

			@author H. Kerem Cevahir (shelta)
			*/
			struct header
			{
				/*!
				Name of HTTP header.
				*/
				std::string name;

				/*!
				Value of HTTP header.
				*/
				std::string value;
			};

		} 
	}
}

#endif

