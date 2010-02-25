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

#ifndef FINDIK_FILTER_FILTER_GROUP_HPP
#define FINDIK_FILTER_FILTER_GROUP_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "abstract_filter.hpp"

#include <list>

namespace findik
{
	namespace filter
	{
		/*!
		Decorator object to group several filters.
		\extends boost::enable_shared_from_this<filter_group> to use boost shared pointers.
		\extends findik::service::service_chain_element to be used in service chains.
		@author H. Kerem Cevahir (shelta)
		*/
		class filter_group :
			public boost::enable_shared_from_this<filter_group>,
			public abstract_filter
		{
		public:
                        /*!
			Function to execute registered filters' filter methods if filter is applicable.
                        \param connection_ connection contains new data to be inspected.
			\param param optional parameter passed to filter.
                        \returns a tuple containing whether content should be filter or not and reason of this decision.
                        */
                        boost::tuple<bool, filter_reason_ptr> filter(findik::io::connection_ptr connection_, unsigned int param = 0);

			/*!
			Simply returns true.
			\param connection_ to test applicability
			\return whether filter is applicable
			*/
			bool is_applicable(findik::io::connection_ptr connection_);

		protected:

			/*!
			List to store filters.
			*/
			std::list<abstract_filter_ptr> filter_list_;

		};
		
		typedef boost::shared_ptr<filter_group> filter_group_ptr;
	}
}

#endif

