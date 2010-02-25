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

#ifndef FINDIK_IMPORTER_PERSISTENCE_MYSQLDBMANAGER_HPP
#define FINDIK_IMPORTER_PERSISTENCE_MYSQLDBMANAGER_HPP

#include "dbmanager.hpp"
#include "configuration.hpp"
#include <iostream>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <mysql++/mysql++.h>

namespace findik {
	namespace importer {
		namespace persistence {

			class mysqldbmanager :
				public dbmanager, 
				public boost::enable_shared_from_this<mysqldbmanager>
			{
			public:
				mysqldbmanager(void);
				~mysqldbmanager(void);

				void connectDb();
				int newCategory(std::string category_name);
				int newDomain(std::string domain_name,int category_id);
				int newUrl(std::string url_name,int category_id);
				int newContent(std::string content_name,int category_id);

				typedef boost::shared_ptr<mysqldbmanager> pointer;

			private:
				mysqlpp::Connection * myconn_;	
				mysqlpp::Query *domain_select;
				mysqlpp::Query *url_select;
				mysqlpp::Query *re_select;
				mysqlpp::Query *category_select;
				mysqlpp::Query *domain_insert;
				mysqlpp::Query *url_insert;
				mysqlpp::Query *re_insert;
				mysqlpp::Query *category_insert;
			};

		}
	}
}

#endif
