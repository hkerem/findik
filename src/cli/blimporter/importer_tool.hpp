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

#ifndef FINDIK_IMPORTER_TOOL_IMPORTER_TOOL_HPP
#define FINDIK_IMPORTER_TOOL_IMPORTER_TOOL_HPP

#include <iostream>
#include <string>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "mysqldbmanager.hpp"

namespace findik{
	namespace importer{
		namespace tool{
			class import{
			public:
				import();	
				import(std::string cmd);
				~import(void);
				
			private:
				void pushToDb(boost::filesystem::path p);
				findik::importer::persistence::dbmanager::pointer dbmanager_;
			};
		}
	}
}


#endif
