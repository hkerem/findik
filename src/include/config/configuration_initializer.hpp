/*
  Copyright (C) 2008 Burak Oguz (barfan) <findikmail@gmail.com>

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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_LIBCONFIG

#ifndef FINDIK_CONFIG_CONFIGURATION_INITIALIZER_HPP
#define FINDIK_CONFIG_CONFIGURATION_INITIALIZER_HPP

#include <boost/noncopyable.hpp>

#include <libconfig.h++>
#include <iostream>

#include "log.hpp"

namespace findik {
	namespace config {

		/*!
		 This class represents the configuration manager utility for findik.

		 @author Burak Oguz
		*/
		class configuration_initializer :
                        private boost::noncopyable
		{
		public:
			/*!
			 Constructor for configuration class. Reads the configuration from /etc/findik.cfg for linux
			 and reads configuration from c:/findik.cfg for Windows.
			*/
			configuration_initializer(void);
			~configuration_initializer(void);

			/*!
			Checks wheter config values are valid.
			*/
			bool check();
			
			/*!
			 Returns a string represented configuration value. It takes a path from configuration file and
			 string reference and saves configuration value into string reference if path is correct. 
			 \param setting_path path to configuration setting in the configuration file
			 \param value variable in which configuration value will be put
			 \return true if path is correct and can read configuration value
			 \sa getConfigValue_Int()
			*/
			bool getConfigValue_String(std::string setting_path, std::string & value);
			
			/*!
                         Returns an unsigned integer represented configuration value. It takes a path from configuration file and
                         unsigned integer reference and saves configuration value into unsigned integer reference if path is correct.
                         \param setting_path path to configuration setting in the configuration file
                         \param value variable in which configuration value will be put
                         \return true if path is correct and can read configuration value
                         \sa getConfigValue_String()
                        */
			bool getConfigValue_UInt(std::string setting_path, unsigned int & value);
			
			/*!
                         Returns an integer represented configuration value. It takes a path from configuration file and
                         integer reference and saves configuration value into integer reference if path is correct.
                         \param setting_path path to configuration setting in the configuration file
                         \param value variable in which configuration value will be put
                         \return true if path is correct and can read configuration value
                         \sa getConfigValue_String()
                        */
			bool getConfigValue_Int(std::string setting_path, int & value);
			
			/*!
                         Returns a boolean represented configuration value. It takes a path from configuration file and
                         boolean reference and saves configuration value into boolean reference if path is correct.
                         \param setting_path path to configuration setting in the configuration file
                         \param value variable in which configuration value will be put
                         \return true if path is correct and can read configuration value
                         \sa getConfigValue_String()
                        */
			bool getConfigValue_Bool(std::string setting_path, bool & value);

			/*!
                         Returns a boolean represented configuration value.
                         \param setting_path path to configuration setting in the configuration file
                         \returns value of setting path as a boolean
                         \sa getConfigValue_Bool()
                        */
			bool returnBool(std::string setting_path);

			/*!
                         Returns an unsigned integer represented configuration value.
                         \param setting_path path to configuration setting in the configuration file
                         \returns value of setting path as unsigned int
                         \sa getConfigValue_Int()
                        */
			unsigned int returnUInt(std::string setting_path);

		protected:
			/*!
			Debug logger for server class.
			*/
                        static log4cxx::LoggerPtr debug_logger;

			libconfig::Config config_;
		};
	}
}

#endif
#endif /* HAVE_LIBCONFIG */
