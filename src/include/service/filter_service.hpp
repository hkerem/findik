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

#ifndef FINDIK_SERVICE_FILTER_SERVICE_HPP
#define FINDIK_SERVICE_FILTER_SERVICE_HPP


#define FI_GET_ACCEPT_REASON(conn) filter_reason_factory_map_[conn->proto()]->create_filter_reason(conn)
#define FI_GET_DENY_REASON(conn) filter_reason_factory_map_[conn->proto()]->create_filter_reason(conn,FC_ACL_DENY)
#define FI_GET_NA_REASON(conn) filter_reason_factory_map_[conn->proto()]->create_filter_reason(conn,FC_ACL_NA)

#include <boost/tuple/tuple.hpp>
#include <boost/noncopyable.hpp>

#include <map>

#include "connection.hpp"
#include "abstract_filter.hpp"
#include "abstract_filter_reason_factory.hpp"
#include "filter_reason.hpp"

namespace findik
{
	namespace service
	{
		/*!
		Manageble container for filters. 
                \extends boost::noncopyable this class has designed to be not copyable.
		*/
		class filter_service :
                        private boost::noncopyable
		{
		public:
			/*!
			Default constructor.
			*/
			filter_service();

			/*!
			Destructor.
			*/
			~filter_service();

			/*!
			Registers a filter for filter service in order to filter with that.
			\param filter_ filter to register
			*/
			void register_filter(const std::string & code, findik::filter::abstract_filter_ptr filter_);

			/*!
			Registers filter reason factory for specified protocol in order to
			generate filter reasons within filter_service.
			\param proto protocol
			\param afrfp filter reason factory to use.
			*/
			void register_filter_reason_factory(findik::io::protocol proto,
				findik::filter::abstract_filter_reason_factory_ptr afrfp);

			/*!
			Examines new_data of connection by using filters in filter_list_.
			\param connection_ connection contains new data to be inspected.
			\returns a tuple containing whether content should be filter or not and reason of this decision.
			*/
			boost::tuple<bool, findik::filter::filter_reason_ptr> 
				filter(findik::io::connection_ptr connection_);

		protected:
			/*!
			Map to store filters in an order. When filter operation has been requested
			filters in this list will be started to be executed in order.
			*/
			std::map<std::string ,findik::filter::abstract_filter_ptr> filter_map_;

			/*!
			Map to store filter reason generators.
			*/
			std::map<findik::io::protocol, findik::filter::abstract_filter_reason_factory_ptr> 
					filter_reason_factory_map_;
		};
	}
}

#endif

