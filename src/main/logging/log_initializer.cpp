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

#include "log_initializer.hpp"
namespace findik {
	namespace logging {

#ifdef HAVE_LOG4CXX
		log_initializer::log_initializer(void)
		{
		}

		bool log_initializer::load_conf(std::string file_name)
		{
			conf_file = file_name;
			try
			{
				log4cxx::PropertyConfigurator::configure(file_name);
				LOG4CXX_INFO(user_logger, "Findik logging started");
			}
			catch(log4cxx::helpers::Exception&)
			{
				return false;
			}
			return true;
		}

		bool log_initializer::reload_conf()
		{
			if(conf_file==""){
				LOG4CXX_ERROR(user_logger,"unable reload log configuration file, load_conf first");
				return false;
			}

			reset();
			load_conf(conf_file);
			return true;
		}

		void log_initializer::shutdown(){
			log_initializer::reset();
		}

		void log_initializer::reset()
		{
			LOG4CXX_INFO(user_logger,"Findik logging stopped");
			log4cxx::Logger::getRootLogger()->removeAllAppenders();
		}

		log_initializer::~log_initializer(void)
		{
		}
#endif

		//initialize static global application logger
		log4cxx::LoggerPtr log_initializer::user_logger(log4cxx::Logger::getLogger("findik-log"));

		//initialize static global filter logger
		log4cxx::LoggerPtr log_initializer::filter_logger(log4cxx::Logger::getLogger("filter"));

		//initialize static global debug logger
		log4cxx::LoggerPtr log_initializer::debug_logger(log4cxx::Logger::getLogger("findik"));
	}
}
