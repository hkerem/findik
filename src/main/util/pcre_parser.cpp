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

#include "pcre_parser.hpp"
#ifdef HAVE_PCRE

namespace findik
{
	namespace util
	{
		pcre_parser::pcre_parser()
		{
		}
		pcre_parser::~pcre_parser()
		{
		}
		pcre_parser::pcre_parser(int category_id_, const std::string & pattern_) 
		{
			options_ = new pcrecpp::RE_Options();
			options_->set_caseless(true);
			options_->set_extra(true);
			options_->set_extended(true);
			options_->set_multiline(true);
			this->category_id_ = category_id_;
			this->re_ = new pcrecpp::RE(pattern_, *options_);
		}
		pcre_parser::pcre_parser(pcre_parser const &pcre_parser_)
		{
			delete re_;
			delete options_;
		}
		pcrecpp::RE * pcre_parser::get_re()
		{
			return this->re_;
		}
		void pcre_parser::set_re(pcrecpp::RE * re_)
		{
			this->re_ = re_;
		}	
		int pcre_parser::get_category_id()
		{
			return this->category_id_;
		}
		void pcre_parser::set_category_id(int category_id_)
		{
			this->category_id_ = category_id_;
		}

	}
}

#endif /* HAVE_PCRE */
