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

#ifndef FINDIK_SERVICE_UTIL_SERVICE_HPP
#define FINDIK_SERVICE_UTIL_SERVICE_HPP

#include <boost/noncopyable.hpp>

#include "pcre_service.hpp"
#ifdef HAVE_MAGIC
#include "magic_num_service.hpp"
#endif

namespace findik
{
	namespace service
	{
		/*!
		Container to manage various utilities. 
                \extends boost::noncopyable this class has designed to be not copyable.
		*/
		class util_service :
                        private boost::noncopyable
		{
		public:
			/*!
			Default constructor.
			*/
			util_service();

			/*!
			Destructor.
			*/
			~util_service();
#ifdef HAVE_PCRE
			/*!
			Method to access pcre service.
			\returns pcre_service instance.
			*/
			pcre_service & pcre();
#endif

#ifdef HAVE_MAGIC
			/*!
                        Method to access magic number service.
                        \returns magic_number_service instance.
                        */
                        magic_num_service & magic_num();
#endif

			/*!
			Method to initialize utilities.
			*/
			void start();

		protected:
#ifdef HAVE_PCRE
			/*!
			Pcre service instance.
			*/
			pcre_service pcre_srv_;
#endif
#ifdef HAVE_MAGIC
			/*!
                        Magic number service instance.
                        */
                        magic_num_service magic_num_srv_;
#endif
		};
	}
}

#endif

