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


#include "configuration.hpp"

namespace findik {
	namespace importer{
		namespace config {

			configuration::configuration(void)
			{
				try {
#if defined(_WIN32)
					config_.readFile("c:/findik.cfg");
#else 
					config_.readFile("/etc/findik/findik.cfg");
#endif

					config_.lookupValue("findik.db.mysql.host",host_); 
					config_.lookupValue("findik.db.mysql.username",username_);
					config_.lookupValue("findik.db.mysql.password",password_);
					config_.lookupValue("findik.db.mysql.database",db_);

				} catch(libconfig::FileIOException & e) {
					std::cout << "IOException in config file read" << std::endl;
				} catch(libconfig::ParseException & e) {
					std::cout << "ParseException in config file read" << std::endl;
				}

			}

			configuration::~configuration(void)
			{
			}

			const std::string & configuration::host()
			{
				return host_;
			}

			const std::string & configuration::username()
			{
				return username_;
			}

			const std::string & configuration::password()
			{
				return password_;
			}

			const std::string & configuration::db()
			{
				return db_;
			}

		}
	}
}
