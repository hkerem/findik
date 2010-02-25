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

#ifndef FINDIK_IMPORTER_PERSISTENCE_DBMANAGER_HPP
#define FINDIK_IMPORTER_PERSISTENCE_DBMANAGER_HPP

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace findik {
	namespace importer {
		namespace persistence {
			
			class dbmanager :
				public boost::enable_shared_from_this<dbmanager>
			{
			public:

				virtual void connectDb() = 0;
				virtual int newCategory(std::string category_name) = 0;
				virtual int newDomain(std::string domain_name,int category_id) = 0;
				virtual int newUrl(std::string url_name,int category_id) = 0;
				virtual int newContent(std::string content_name,int category_id) = 0;

				typedef boost::shared_ptr<dbmanager> pointer;

			protected:
				std::string host;
				std::string db;
				std::string username;
				std::string password;

			};

		}
	}
}

#endif
