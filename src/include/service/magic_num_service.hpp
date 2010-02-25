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
#ifdef HAVE_MAGIC

#ifndef FINDIK_SERVICE_MAGIC_NUM_SERVICE_HPP
#define FINDIK_SERVICE_MAGIC_NUM_SERVICE_HPP

#include <boost/noncopyable.hpp>

#include <string>
#include <vector>
#include <magic.h>

#include <boost/thread/mutex.hpp>

namespace findik
{
	namespace service
	{
		/*!
		Container to manage magic number opertaions. 
                \extends boost::noncopyable this class has designed to be not copyable.
		@author Burak OGUZ (barfan)
		*/
		class magic_num_service :
                        private boost::noncopyable
		{
		public:
			/*!
			Default constructor.
			*/
			magic_num_service();

			/*!
			Destructor.
			*/
			~magic_num_service();

			/*!
			Method to initialize regular expressions.
			*/
			void start();

			/*!
			Returns the mime type of the given character vector.
			
			\param character vector which mime-type will be found
			*/
			void get_magic_number(const std::vector<char> & data, std::string & mime_type);

		protected:

			/*!
			Magic number structure from magic.h
			*/
			magic_t magic_mime;

			/*!
			Mutex for magic mime cookie.
			*/
			boost::mutex magic_mime_mutex_;

		};
	}
}

#endif
#endif /* HAVE_MAGIC */
