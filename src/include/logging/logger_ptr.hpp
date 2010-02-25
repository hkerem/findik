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

#ifndef LOG4CXX_LOGGER_PTR_HPP
#define LOG4CXX_LOGGER_PTR_HPP

#include <string>

namespace log4cxx {
	class LoggerPtr 
	{
		public:
			LoggerPtr(const std::string &name);
			~LoggerPtr();
			std::string &getName();

		private:
			std::string name;

	};

}
#endif //  LOGGER_PTR
