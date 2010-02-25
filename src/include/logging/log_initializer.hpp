/*
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
#ifndef FINDIK_LOGGING_LOG_INITIALIZER_HPP
#define FINDIK_LOGGING_LOG_INITIALIZER_HPP

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_LOG4CXX
// include log4cxx header files.
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>

namespace findik {
	namespace logging {
		class log_initializer
		{
		public:
			//constructer initialize logger
			log_initializer(void);

			//load conf from log4j conf file
			bool load_conf(std::string conf_file);

			//reload conf
			bool reload_conf(void);

			//reset logging globally
			static void reset(void);

			//shutdown logging globally
			static void shutdown(void);

			//destructor
			~log_initializer(void);

			//global user logger
			static log4cxx::LoggerPtr user_logger;

			//global debug logger
			static log4cxx::LoggerPtr debug_logger;

			//global filter logger
			static log4cxx::LoggerPtr filter_logger;


		private:
			//log4j file name
			std::string conf_file;
		};

	}
}
#else
#include "logger.hpp"
namespace findik {
        namespace logging {
                class log_initializer
                {
                public:
			//global user logger
                        static log4cxx::LoggerPtr user_logger;

                        //global debug logger
                        static log4cxx::LoggerPtr debug_logger;

                        //global filter logger
                        static log4cxx::LoggerPtr filter_logger;
		};
	}
}
#endif
#endif //  LOG_INITIALIZER_HPP

