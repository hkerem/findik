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
#include <boost/program_options.hpp>

int main(int argc, char* argv[])
{
	// Declare the supported options.
	boost::program_options::options_description desc("This program imports urlblacklist.com blcklists into FINDIK compatible database structure.\nHere are the allowed options");
	desc.add_options()
		("help,h", "Produces this help message")
		("dir", boost::program_options::value<std::string>(), "Set urlblacklist.org unpackaged directory")
		;

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
	boost::program_options::notify(vm);    

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	} 

	if(vm.count("dir")) {
		findik::importer::tool::import(vm["dir"].as<std::string>());
		return 0;
	} else {
		std::cout << desc << "\n";
                return 1;
	}
}
