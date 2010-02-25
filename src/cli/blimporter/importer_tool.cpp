/*
  Copyright (C) 2008 Burak OGUZ (barfan) <findikmail@gmail.com>
  Copyright (C) 2008 Kutay Sanal (ktysnl) <findikmail@gmail.com>

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

#include "importer_tool.hpp"
namespace findik{
        namespace importer{
                namespace tool{
			
			import::import() { }

			import::import(std::string cmd){
				dbmanager_ = findik::importer::persistence::dbmanager::pointer(new findik::importer::persistence::mysqldbmanager());
				dbmanager_->connectDb();
				boost::filesystem::path p(cmd);
				boost::filesystem::directory_iterator d1(p), d2;
        			for(; d1 != d2; ++d1)
                			if( boost::filesystem::is_directory(d1->path()) )
						pushToDb(d1->path());
			}
			import::~import(void){
			}

			void import::pushToDb(boost::filesystem::path p)
			{
				int category_id = dbmanager_->newCategory(p.leaf());
				
				if(category_id == 0)
					return;
				boost::filesystem::path temp(p);
				temp /= "domains";
				
				std::string line;
				std::string fpath(temp.string());
				if(boost::filesystem::exists(temp))
				{
					std::ifstream inp(fpath.c_str());
					while(!inp.eof())
					{
						inp>>line;
        					dbmanager_->newDomain(line,category_id);
					}
					inp.close();
				}

				
				temp = p;
				temp /= "urls";
				fpath = temp.string();
				if(boost::filesystem::exists(temp))
				{
					std::ifstream inp(fpath.c_str());
					while(!inp.eof())
					{
						inp>>line;
        					dbmanager_->newUrl(line,category_id);

					}
					inp.close();
				}

				temp = p;
                                temp /= "expressions";
                                fpath = temp.string();
                                if(boost::filesystem::exists(temp))
                                {
                                        std::ifstream inp(fpath.c_str());
                                        while(!inp.eof())
                                        {
                                                inp>>line;
                                                dbmanager_->newContent(line,category_id);

                                        }
                                        inp.close();
                                }	
			}
		}
	}
}

