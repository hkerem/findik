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

#ifndef FINDIK_IMPORTER_CONFIG_CONFIGURATION_HPP
#define FINDIK_IMPORTER_CONFIG_CONFIGURATION_HPP

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <libconfig.h++>
#include <iostream>


namespace findik {
	namespace importer {
		namespace config {

			class configuration : 
				public boost::enable_shared_from_this<configuration>
			{
			public:
				configuration(void);
				~configuration(void);

				const std::string & host();
				const std::string & username();
				const std::string & password();
				const std::string & db();


			private:

				std::string host_;
				std::string username_;
				std::string password_;
				std::string db_;
				libconfig::Config config_;
			};
				
			typedef boost::shared_ptr<configuration> configuration_ptr;

		}
	}
}

#endif
