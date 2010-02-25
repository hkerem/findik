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

#include "pcre_analyzer.hpp"
#ifdef HAVE_PCRE

namespace findik
{
	namespace util
	{
		pcre_analyzer::pcre_analyzer()
		{
		}
		pcre_analyzer::~pcre_analyzer()
		{
		}

		void pcre_analyzer::set_word(const std::string & word)
		{
			this->word = word;
                }

		std::string pcre_analyzer::get_word()
		{
			return word;
                }

		void pcre_analyzer::set_count(int count)
		{
			this->count = count;
                }

		int pcre_analyzer::get_count()
		{
			return count;
                }

		void pcre_analyzer::set_catid(int catid)
		{
			this->catid = catid;
                }

		int pcre_analyzer::get_catid()
		{
			return catid;
                }
	}
}

#endif /* HAVE_PCRE */
