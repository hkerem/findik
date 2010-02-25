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

#include "logger.hpp"

namespace log4cxx {
	Logger::Logger() {}
	Logger::~Logger() {}

	LoggerPtr Logger::getLogger(const std::string& name)
	{
		return LoggerPtr(name);
	}
	void Logger::info(LoggerPtr &loggerPtr, const std::string &logStr)
	{
		log(loggerPtr, logStr, Logger::INFO);
	}
	void Logger::debug(LoggerPtr &loggerPtr, const std::string &logStr)
	{
		log(loggerPtr, logStr, Logger::DEBUG);
	}
	void Logger::warn(LoggerPtr &loggerPtr, const std::string &logStr)
	{
		log(loggerPtr, logStr, Logger::WARN);
	}
	void Logger::error(LoggerPtr &loggerPtr, const std::string &logStr)
	{
		log(loggerPtr, logStr, Logger::ERROR);
	}
	void Logger::fatal(LoggerPtr &loggerPtr, const std::string &logStr)
	{
		log(loggerPtr, logStr, Logger::FATAL);
	}

	void Logger::log(LoggerPtr &loggerPtr, const std::string &logStr, int level)
	{

	}
};

