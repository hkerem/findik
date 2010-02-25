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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_PCRE

#ifndef FINDIK_SERVICE_PCRE_SERVICE_HPP
#define FINDIK_SERVICE_PCRE_SERVICE_HPP

#include <boost/noncopyable.hpp>

#include <string>
#include <vector>
#include "pcre_parser.hpp"
#include "pcre_analyzer.hpp"

namespace findik
{
	namespace service
	{
		/*!
		Container to manage pcre. 
                \extends boost::noncopyable this class has designed to be not copyable.
		*/
		class pcre_service :
                        private boost::noncopyable
		{
		public:
			/*!
			Default constructor.
			*/
			pcre_service();

			/*!
			Destructor.
			*/
			~pcre_service();

			/*!
			Method to initialize regular expressions.
			*/
			void start();

			/*!
			Searches the given string for regular expressions saved in db.
			*/
			bool matches_predefined(const std::string & date, unsigned int category = 0);

			/*!
			Searches the given string for regular expressions saved in db.
			*/
			std::vector<findik::util::pcre_analyzer> matches_custom(const std::string & pattern, const std::string & data);

			/*!
			Searches the given string for regular expressions saved in db.
			*/
			void global_replace(const std::string & pattern, const std::string & replace, std::string & data);


		protected:

			/*!
			Vector containing predefined regular expressions in database.
			*/
			std::vector<findik::util::pcre_parser *> re_vector;

		};
	}
}

#endif
#endif /* HAVE_PCRE */
