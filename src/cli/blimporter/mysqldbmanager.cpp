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

#include "mysqldbmanager.hpp"

namespace findik {
	namespace importer {
		namespace persistence {

			mysqldbmanager::mysqldbmanager(void)
			{
			}

			mysqldbmanager::~mysqldbmanager(void)
			{
				//if (con.get() != NULL && !con->isClosed())
					//con->close();
				//this->host.clear();
				//this->username.clear();
				//this->password.clear();
				//this->db.clear();
			}

			void mysqldbmanager::connectDb()
			{
				try {
					config::configuration_ptr cptr(new config::configuration());
			
					myconn_ = new mysqlpp::Connection(cptr->db().c_str(), cptr->host().c_str(), cptr->username().c_str(), cptr->password().c_str());	
					category_select = new mysqlpp::Query(myconn_->query("SELECT * from category where name = %0q"));
					category_insert = new mysqlpp::Query(myconn_->query("INSERT INTO category VALUES (NULL, %0q)"));
					domain_select = new mysqlpp::Query(myconn_->query("Select * from domain where domain = %0q"));
					domain_insert = new mysqlpp::Query(myconn_->query("INSERT INTO domain VALUES (NULL,%0q,%1q)"));
					url_select = new mysqlpp::Query(myconn_->query("Select * from url where url = %0q"));
					url_insert = new mysqlpp::Query(myconn_->query("INSERT INTO url VALUES (NULL,%0q,%1q)"));
					re_select = new mysqlpp::Query(myconn_->query("Select * from content where content = %0q"));
					re_insert = new mysqlpp::Query(myconn_->query("INSERT INTO content VALUES (NULL,%0q,%1q)"));
			
					category_select->parse();
					category_insert->parse();
					domain_select->parse();
					domain_insert->parse();
					url_select->parse();
					url_insert->parse();
					re_select->parse();
					re_insert->parse();

				} catch (const mysqlpp::Exception& e) {
					std::cerr << "SQL Error" << std::endl;
				}
			}

			/**
			 * First checks whether category name is inserted. If inserted returns category_id
			 * Then inserts category and returns newly added category name
			 */
			int mysqldbmanager::newCategory(std::string category_name)
			{
				try {
					std::cout << "Importing category : " << category_name << std::endl;
					mysqlpp::StoreQueryResult res = category_select->store(category_name);
					if(res.num_rows() > 0) 
						return (int)res[0][0];

					category_insert->store(category_name);

					res.clear();
					res = category_select->store(category_name);

					if(res.num_rows() > 0)
						return (int)res[0][0]; 

					res.clear();
					return 0;
				} catch (const mysqlpp::BadQuery& e) {
					std::cout << "Bad Query " <<  e.what() << std::endl;
					return 0;
				}
				catch (const mysqlpp::BadConversion& e) {
					std::cout << "Bad Conversion" << std::endl;
					return 0;
				}
				catch (const mysqlpp::Exception& e) {
					std::cout << "Exception" << std::endl;
					return 0;
				}
			}

			/**
			 * First checks whether domain name is inserted. If inserted returns 0
			 * Then inserts domain name and returns 0 if process true
			 * returns 1 if exception occurs
			 */
			int mysqldbmanager::newDomain(std::string domain_name,int category_id)
			{
				try {
					mysqlpp::StoreQueryResult res = domain_select->store(domain_name);
					if(res.num_rows() > 0)
						return (int)res[0][0];

					domain_insert->store(domain_name, category_id);
					res.clear();

					res = domain_select->store(domain_name);

					if(res.num_rows() > 0)
						return (int)res[0][0];

					res.clear();
					return 0;
				} catch (const mysqlpp::BadQuery& e) {
					std::cout << "Bad Query" << std::endl;
					return 0;
				}
				catch (const mysqlpp::BadConversion& e) {
					std::cout << "Bad Conversion" << std::endl;
					return 0;
				}
				catch (const mysqlpp::Exception& e) {
					std::cout << "Exception" << std::endl;
					return 0;
				}

			}

			/**
			 * First checks whether url name is inserted. If inserted returns 0
			 * Then inserts url name and returns 0 if process true
			 */
			int mysqldbmanager::newUrl(std::string url_name,int url_id)
			{
				try {
					mysqlpp::StoreQueryResult res = url_select->store(url_name);
                                        if(res.num_rows() > 0)
                                                return (int)res[0][0];

                                        url_insert->store(url_name, url_id);
                                        res.clear();
                                        res = url_select->store(url_name);

                                        if(res.num_rows() > 0)
                                                return (int)res[0][0];

                                        res.clear();
                                        return 0;
				} catch (const mysqlpp::BadQuery& e) {
                                        std::cout << "Bad Query" << std::endl;
                                        return 0;
                                }
                                catch (const mysqlpp::BadConversion& e) {
                                        std::cout << "Bad Conversion" << std::endl;
                                        return 0;
                                }
                                catch (const mysqlpp::Exception& e) {
                                        std::cout << "Exception" << std::endl;
                                        return 0;
                                }
			}

			int mysqldbmanager::newContent(std::string content_name,int content_id)
			{
				try {
                                        mysqlpp::StoreQueryResult res = re_select->store(content_name);

                                        if(res.num_rows() > 0)
                                                return (int)res[0][0];

                                        re_insert->store(content_name, content_id);

                                        res.clear();
                                        res = re_select->store(content_name);

                                        if(res.num_rows() > 0)
                                                return (int)res[0][0];

                                        res.clear();
                                        return 0;
				} catch (const mysqlpp::BadQuery& e) {
                                        std::cout << "Bad Query" << std::endl;
                                        return 0;
                                }
                                catch (const mysqlpp::BadConversion& e) {
                                        std::cout << "Bad Conversion" << std::endl;
                                        return 0;
                                }
                                catch (const mysqlpp::Exception& e) {
                                        std::cout << "Exception" << std::endl;
                                        return 0;
                                }
			}
		}
	}
}
