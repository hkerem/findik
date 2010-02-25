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

#ifndef LOG4CXX_LOGGER_HPP
#define LOG4CXX_LOGGER_HPP

#include "logger_ptr.hpp"
#include <string>

#define LOG4CXX_INFO log4cxx::Logger::info
#define LOG4CXX_DEBUG log4cxx::Logger::debug
#define LOG4CXX_WARN log4cxx::Logger::warn
#define LOG4CXX_ERROR log4cxx::Logger::error
#define LOG4CXX_FATAL log4cxx::Logger::fatal

namespace log4cxx {
	class Logger 
	{
		public:
			Logger();
			~Logger();
			static LoggerPtr getLogger(const std::string& name);
			static void info(LoggerPtr &loggerPtr, const std::string &logStr);
			static void debug(LoggerPtr &loggerPtr, const std::string &logStr);
			static void warn(LoggerPtr &loggerPtr, const std::string &logStr);
			static void error(LoggerPtr &loggerPtr, const std::string &logStr);
			static void fatal(LoggerPtr &loggerPtr, const std::string &logStr);
			static void log(LoggerPtr &loggerPtr, const std::string &logStr, int level);

			enum LogLevel {
				INFO=1,
				DEBUG=2,
				WARN=3,
				ERROR=4,
				FATAL=5
			};

	};

}
#endif //  LOGGER
