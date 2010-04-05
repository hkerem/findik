/*
  Copyright (C) 2008 H. Kerem Cevahir (shelta) <findikmail@gmail.com>

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

#include "mysqldbmanager.hpp"
#ifdef HAVE_MYSQL

#include "service_container.hpp"

#include <boost/lexical_cast.hpp>

#include <boost/scoped_ptr.hpp>
#include <vector>

namespace findik 
{
	namespace persistency 
	{
		log4cxx::LoggerPtr mysqldbmanager::debug_logger(log4cxx::Logger::getLogger("findik.persistency.mysqldbmanager"));

		mysqldbmanager::mysqldbmanager() :
			is_connected(false)
		{}

		void mysqldbmanager::connect()
		{
			if (is_connected)
				return;
			is_connected = false;

			host = FI_CONFIG.db_mysql_host();
			username = FI_CONFIG.db_mysql_username();
			password = FI_CONFIG.db_mysql_password();
			db = FI_CONFIG.db_mysql_database();
			pool_size_ = FI_CONFIG.db_pool_size();

			prepare_pool();
		}

		mysqldbmanager::~mysqldbmanager() 
		{
			for (size_t i = 0; i < pool_size_; ++i)
				((dbconnection<mysqlpp::Connection>::pointer)pool_[i])->connection()->disconnect();
		}

		mysqldbmanager::mysql_dbconnection_ptr mysqldbmanager::create_connection_object()
		{
			try {
				mysqlpp::Connection * myconn_ = new mysqlpp::Connection(db.c_str(), host.c_str(), username.c_str(), password.c_str());

				mysql_dbconnection_ptr dbconnection__(new mysql_dbconnection(myconn_));

				mysqlpp::Query * domain_q = new mysqlpp::Query(
					myconn_->query("SELECT d.id from domain d "
						"where d.domain = %0q and d.catid = %1"));
			        domain_q->parse();

				mysqlpp::Query * url_q = new mysqlpp::Query(
					myconn_->query("SELECT u.id from url u "
						"where u.url = %0q and u.catid = %1"));
			        url_q->parse();

				mysqlpp::Query * btk_q = new mysqlpp::Query(
					myconn_->query("SELECT u.id from btk_blacklist u "
						"where u.domain = %0q"));
			        btk_q->parse();

				mysqlpp::Query * pcre_q = new mysqlpp::Query(
					myconn_->query("SELECT c.content,c.catid from content c"));
				//	myconn_->query("SELECT c.content,c.catid from content c join blacklist_category bc "
				//		"where c.catid = bc.catid"));
			        pcre_q->parse();

				mysqlpp::Query * file_ext_q = new mysqlpp::Query(
					myconn_->query("SELECT mt.file_ext from mime_type_cross mtc "
						"left outer join mime_type mt ON mt.id = mtc.mime_type_id "
						"where mt.file_ext = %0q and mtc.mime_group_id = %1"));
			        file_ext_q->parse();

				mysqlpp::Query * mime_type_q = new mysqlpp::Query(
					myconn_->query("SELECT mt.mime_type from mime_type_cross mtc "
						"left outer join mime_type mt ON mt.id = mtc.mime_type_id "
						"where mt.mime_type = %0q and mtc.mime_group_id = %1"));
                                mime_type_q->parse();

				mysqlpp::Query * acl_q = new mysqlpp::Query(
					myconn_->query(
						"SELECT af.filter_key,af.param,ar.deny "
						"FROM acl_rule AS ar "
						"LEFT OUTER JOIN acl_ip_cross AS acn ON ar.id = acn.rule_id "
						"LEFT OUTER JOIN acl_time_cross AS act ON ar.id = act.rule_id "
						"LEFT OUTER JOIN ip_table AS an ON an.id = acn.acl_id "
						"LEFT OUTER JOIN time_table AS at ON at.id = act.acl_id "
						"LEFT OUTER JOIN acl_filter_param AS af ON ar.id = af.rule_id "
						"WHERE ( "
							"an.local_masked_ip IS NULL "
							"OR "
							"an.local_masked_ip = %0 & an.local_mask "
						") "
						"AND ( "
							"at.start_time IS NULL "
							"OR "
							"at.start_time < CURTIME( ) "
						") "
						"AND ( "
							"at.end_time IS NULL "
							"OR "
							"at.end_time > CURTIME( ) "
						") "
						"AND ( "
							"at.day_of_week IS NULL "
							"OR "
							"at.day_of_week & POW(2, DAYOFWEEK(CURDATE())) > 0 "
						") "
						"ORDER BY ar.id "
						"ASC "
					));
                                acl_q->parse();


				dbconnection__->set_object(domain_query, domain_q);

				dbconnection__->set_object(url_query, url_q);

				dbconnection__->set_object(btk_query, btk_q);

				dbconnection__->set_object(pcre_query, pcre_q);
				
				dbconnection__->set_object(file_ext_query, file_ext_q);
				
				dbconnection__->set_object(mime_type_query, mime_type_q);
				
				dbconnection__->set_object(acl_query, acl_q);
				

				return dbconnection__;
			}
			catch (const mysqlpp::Exception& e)
			{
				LOG4CXX_ERROR(debug_logger, "SQL Error: " );
				LOG4CXX_ERROR(logging::log_initializer::user_logger,
					"Unable to connect MySQL on host: " << host << " with user: " << username);
			}
		}

		bool mysqldbmanager::domainQuery(const std::string & hostname, unsigned int group) 
		{
			mysql_dbconnection_ptr dbconnection_(get_dbconnection());
			bool return_ = true;

			try {
				mysqlpp::StoreQueryResult res = ((mysqlpp::Query *)dbconnection_->get_object(domain_query))
					->store(hostname, boost::lexical_cast<std::string>(group));
				
				if(res.num_rows() > 0)
					return_ = false;

				res.clear();

			} catch (const mysqlpp::BadQuery& e) {
				LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
				return_ = false;
			}
			catch (const mysqlpp::BadConversion& e) {
				LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
				return_ = false;
			}
			catch (const mysqlpp::Exception& e) {
				LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
				return_ = false;
			}

			dbconnection_->unlock();

			return return_;
		}

		bool mysqldbmanager::urlQuery(const std::string & url, unsigned int group) 
		{
			mysql_dbconnection_ptr dbconnection_(get_dbconnection());

			bool return_ = true;
			
			try {
                                mysqlpp::StoreQueryResult res = ((mysqlpp::Query *)dbconnection_->get_object(url_query))
					->store(url, boost::lexical_cast<std::string>(group));

                                if(res.num_rows() > 0)
                                        return_ = false;
				
				res.clear();

                        } catch (const mysqlpp::BadQuery& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }
                        catch (const mysqlpp::BadConversion& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }
                        catch (const mysqlpp::Exception& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }

			dbconnection_->unlock();

                        return return_;
		}

		bool mysqldbmanager::btkQuery(const std::string & url) 
		{
			mysql_dbconnection_ptr dbconnection_(get_dbconnection());

			bool return_ = true;
			
			try {
                                mysqlpp::StoreQueryResult res = ((mysqlpp::Query *)dbconnection_->get_object(btk_query))
					->store(url);

                                if(res.num_rows() > 0)
                                        return_ = false;
				
				res.clear();

                        } catch (const mysqlpp::BadQuery& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }
                        catch (const mysqlpp::BadConversion& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }
                        catch (const mysqlpp::Exception& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }

			dbconnection_->unlock();

                        return return_;
		}

		void mysqldbmanager::pcreQuery(std::list<boost::tuple<int,std::string> > & pcre_map) 
		{
			mysql_dbconnection_ptr dbconnection_(get_dbconnection());

			try {
				mysqlpp::StoreQueryResult res1 = ((mysqlpp::Query *)dbconnection_->get_object(pcre_query))->store();
				for (std::size_t i = 0 ; i < res1.size() ; i++) 
				{
 					pcre_map.push_back( boost::make_tuple(
								(int)res1[i][1],
								res1[i][0].c_str()
							)); 
				}
				res1.clear();

			}  catch (const mysqlpp::BadQuery& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                        }
                        catch (const mysqlpp::BadConversion& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                        }
                        catch (const mysqlpp::Exception& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                        }

			dbconnection_->unlock();
		}

		void mysqldbmanager::aclQuery(
				std::list< boost::tuple<std::string, int, bool> > & filter_list, 
				unsigned long local_ip)
		{
			mysql_dbconnection_ptr dbconnection_(get_dbconnection());

			try {
				mysqlpp::StoreQueryResult res = 
					((mysqlpp::Query *)dbconnection_->get_object(acl_query))->
						store(boost::lexical_cast<std::string>(local_ip));
				for (std::size_t i = 0 ; i < res.size() ; i++) 
				{
					std::string filter_key("");
					if (! res[i][0].is_null() )
						filter_key = res[i][0].c_str();

					unsigned int filter_param = 0;
					if (! res[i][1].is_null() )
						filter_param = (unsigned int)( res[i][1] );

					bool deny_if_filtered = (bool) res[i][2];

 					filter_list.push_back(boost::make_tuple(filter_key, filter_param, deny_if_filtered)); 
				}
				res.clear();

			}  
			catch (const mysqlpp::BadQuery& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR " << e.what());
                        }
                        catch (const mysqlpp::BadConversion& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR " << e.what());
                        }
                        catch (const mysqlpp::Exception& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR " << e.what());
                        }

			dbconnection_->unlock();
		}
		
		bool mysqldbmanager::fileExtQuery(const std::string & file_ext, unsigned int group) 
		{
			mysql_dbconnection_ptr dbconnection_(get_dbconnection());

			bool return_ = true;
			
			try {
                                mysqlpp::StoreQueryResult res = ((mysqlpp::Query *)dbconnection_->get_object(file_ext_query))
					->store(file_ext, boost::lexical_cast<std::string>(group));

                                if(res.num_rows() > 0)
                                        return_ = false;

				res.clear();

                        } catch (const mysqlpp::BadQuery& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }
                        catch (const mysqlpp::BadConversion& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }
                        catch (const mysqlpp::Exception& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }

			dbconnection_->unlock();

                        return return_;
		}
		
		bool mysqldbmanager::mimeTypeQuery(const std::string & mime_type, unsigned int group) 
		{
			mysql_dbconnection_ptr dbconnection_(get_dbconnection());

			bool return_ = true;
			
			try {
                                mysqlpp::StoreQueryResult res = ((mysqlpp::Query *)dbconnection_->get_object(mime_type_query))
					->store(mime_type, boost::lexical_cast<std::string>(group));

                                if(res.num_rows() > 0)
                                        return_ = false;

				res.clear();

                        } catch (const mysqlpp::BadQuery& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }
                        catch (const mysqlpp::BadConversion& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }
                        catch (const mysqlpp::Exception& e) {
                                LOG4CXX_ERROR(debug_logger, "ERROR" << e.what());
                                return_ = false;
                        }

			dbconnection_->unlock();

                        return return_;
		}
	}
}
#endif /* HAVE_MYSQL */
