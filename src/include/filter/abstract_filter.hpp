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

#ifndef FINDIK_FILTER_ABSTRACT_FILTER_HPP
#define FINDIK_FILTER_ABSTRACT_FILTER_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/tuple/tuple.hpp>

#include "filter_reason.hpp"
#include "service_chain_element.hpp"
#include "connection.hpp"
#include "log.hpp"
#include "protocol.hpp"

namespace findik
{
	namespace filter
	{
		/*!
		Abstract filter object to inherit in order to implement filters for findik.
		\extends boost::enable_shared_from_this<abstract_filter> to use boost shared pointers.
		\extends findik::service::service_chain_element to be used in service chains.
		@author H. Kerem Cevahir (shelta)
		*/
		class abstract_filter :
			public boost::enable_shared_from_this<abstract_filter>,
			public findik::service::service_chain_element
		{
		public:
                        /*!
                        Examines new_data of connection by using filters in filter_list_.
			If method returns true for decision, service will execute other filters too,
			otherwise it will stop execution and return false with reason of this operation.
                        \param connection_ connection contains new data to be inspected.
			\param param optional parameter passed to filter.
                        \returns a tuple containing whether content should be filter or not and reason of this decision.
                        */
                        virtual boost::tuple<bool, filter_reason_ptr> 
				filter(findik::io::connection_ptr connection_, unsigned int param = 0) = 0;

			/*!
			Filter should return whether current data of connection applicable for self or not.
			For example a filter designed to analysis content is not applicable for dat objects without content or
			a filter designed for local data is not applicable for remote data.
			\param connection_ to test applicability
			\return whether filter is applicable
			*/
			virtual bool is_applicable(findik::io::connection_ptr connection_) = 0;

		protected:

		};
		
		typedef boost::shared_ptr<abstract_filter> abstract_filter_ptr;
	}
}

#endif

