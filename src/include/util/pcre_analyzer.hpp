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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_PCRE

#ifndef FINDIK_UTIL_PCRE_ANALYZER_HPP
#define FINDIK_UTIL_PCRE_ANALYZER_HPP

#include <iostream>
#include <string>

namespace findik
{
	namespace util
	{
		/*!
		PCRE analyzer class which saves results for regular expression operations.
		@author Burak OGUZ (barfan)
		 */
		class pcre_analyzer
		{
			public:
				pcre_analyzer();
				~pcre_analyzer();
				
				void set_word(const std::string & word);
				std::string get_word();
				void set_count(int count);
				int get_count();
				void set_catid(int catid);
				int get_catid();

			protected:
				std::string word;
				int count;
				int catid;	
		};


	}
}

#endif
#endif /* HAVE_PCRE */
