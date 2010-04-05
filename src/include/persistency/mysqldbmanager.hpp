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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_MYSQL

#ifndef FINDIK_PERSISTENCY_MYSQLDBMANAGER_HPP
#define FINDIK_PERSISTENCY_MYSQLDBMANAGER_HPP

#include "pooled_dbmanager.hpp"
#include "log.hpp"

#include <mysql++/mysql++.h>
#include <boost/enable_shared_from_this.hpp>
#include <boost/tuple/tuple.hpp>
#include <list>

namespace findik {
	namespace persistency {
		class mysqldbmanager :
			public pooled_dbmanager<mysqlpp::Connection>,
			public boost::enable_shared_from_this<mysqldbmanager>
		{

		public:
			mysqldbmanager();
			~mysqldbmanager();

			void connect();

			bool domainQuery(const std::string & hostname, unsigned int group);

			bool urlQuery(const std::string & url, unsigned int group);

			bool btkQuery(const std::string & url);

                        void btkWarningPage(std::string & warning_page);

			void pcreQuery(std::list<boost::tuple<int,std::string> > &pcre_map);

			bool fileExtQuery(const std::string & file_ext, unsigned int group);

			bool mimeTypeQuery(const std::string & mime_type, unsigned int group);

			void aclQuery(
				std::list< boost::tuple<std::string, int, bool> > & filter_list, 
				unsigned long local_ip);

		protected:
			static log4cxx::LoggerPtr debug_logger;

			bool is_connected;

			typedef dbconnection<mysqlpp::Connection> mysql_dbconnection;

			typedef boost::shared_ptr<mysql_dbconnection> mysql_dbconnection_ptr;

			mysql_dbconnection_ptr create_connection_object();

			typedef mysqlpp::Connection * connection_ptr;

			enum prepared_statement_types {
				domain_query = 100,
				url_query = 101,
				btk_query = 201,
				btk_warning_page = 202,
				pcre_query = 102,
				file_ext_query = 103,
				mime_type_query = 104,
				acl_query = 105
			};

		};
	}
}

#endif
#endif /* HAVE_MYSQL */

