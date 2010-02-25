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

#ifndef FINDIK_UTIL_PCRE_PARSER_HPP
#define FINDIK_UTIL_PCRE_PARSER_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <pcrecpp.h>
#include <iostream>
#include <vector>
#include <map>

#include <string>

namespace findik
{
	namespace util
	{
		/*!
		PCRE Parser class for regular expression operations.
		@author Burak OGUZ (barfan)
		 */
		class pcre_parser
		{
			public:
				pcre_parser();
				~pcre_parser();
				pcre_parser(int category_id_, const std::string & pattern_);
				pcre_parser(pcre_parser const &pcre_parser_);
				pcrecpp::RE * get_re();
				void set_re(pcrecpp::RE *re_);
				int get_category_id();
				void set_category_id(int category_id_);

			private:
				pcrecpp::RE * re_;
				int category_id_;
				pcrecpp::RE_Options *options_;
		};

		typedef boost::shared_ptr<pcre_parser> pcre_parser_ptr;	
	}
}

#endif
#endif /* HAVE_PCRE */
