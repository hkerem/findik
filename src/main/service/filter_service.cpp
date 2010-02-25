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

#include "filter_service.hpp"

#include "services.hpp"

#include <boost/tuple/tuple.hpp>
#include <list>

namespace findik
{
	namespace service
	{
		filter_service::filter_service()
		{}

		filter_service::~filter_service()
		{}

		void filter_service::register_filter(const std::string & code, findik::filter::abstract_filter_ptr filter_)
		{
			filter_map_.insert(std::pair<std::string, findik::filter::abstract_filter_ptr>(code, filter_));
		}

		void filter_service::register_filter_reason_factory(findik::io::protocol proto,
				findik::filter::abstract_filter_reason_factory_ptr afrfp)
		{
			filter_reason_factory_map_.insert(
				std::pair<findik::io::protocol, findik::filter::abstract_filter_reason_factory_ptr>(proto, afrfp)
				);
		}

		boost::tuple<bool, findik::filter::filter_reason_ptr> 
			filter_service::filter(findik::io::connection_ptr connection_)
		{
			findik::filter::filter_reason_ptr frp;

			std::list< boost::tuple<std::string, int, bool> > filter_list;
			std::list< boost::tuple<std::string, int, bool> >::iterator it;

			FI_SERVICES->db_srv().aclQuery(filter_list, 
				connection_->local_socket().remote_endpoint().address().to_v4().to_ulong());

			for (it = filter_list.begin();
				it != filter_list.end(); it++)
			{
				std::string filter_key;
				unsigned long filter_param;
				bool deny_if_filtered;
				boost::tie(filter_key, filter_param, deny_if_filtered) = *it;

				if (filter_key == "") // No filter operation defined in acl. So, rule action will be performed
				{ 
					if (deny_if_filtered) // deny
					{
						findik::filter::filter_reason_ptr deny_reason_ = FI_GET_DENY_REASON(connection_);
						LOG4CXX_WARN(logging::log_initializer::filter_logger, deny_reason_->log_str());
						return boost::make_tuple(false, deny_reason_);
					}
					else // accept
					{
						break;
					}
				}

				findik::filter::abstract_filter_ptr filter_ = filter_map_[filter_key];

				if ( filter_.get() == 0 )
				{
					findik::filter::filter_reason_ptr deny_reason_ = FI_GET_NA_REASON(connection_);
					LOG4CXX_WARN(logging::log_initializer::filter_logger, deny_reason_->log_str());
					return boost::make_tuple(false, deny_reason_);
				}
					

				if (filter_->is_applicable(connection_))
				{
					boost::tuple<bool, findik::filter::filter_reason_ptr> result = 
						filter_->filter(connection_, filter_param);
					
					if (!boost::get<0>(result)) //filtered
					{
						if (deny_if_filtered) // deny
						{
							LOG4CXX_WARN(logging::log_initializer::filter_logger, boost::get<1>(result)->log_str());
							return result;
						}
						else // accept
						{
							// store reason for logging
							//frp = boost::get<1>(result);
							break;
						}
					} 
					else
					{
						continue; // try to apply next filter
					}
				}
				else
				{
					continue; // try to apply next filter
				}
			}

			//TODO: may be return score for greylisting
			if( connection_->current_data()->is_remote()
			//	&& connection_->current_data()->has_content() 
				) 
			{
				if (frp.get() == 0)
				{
					frp = FI_GET_ACCEPT_REASON(connection_);
				}
				LOG4CXX_INFO(logging::log_initializer::filter_logger, frp->log_str());
			}

			return boost::make_tuple(true, frp);
		}
	}
}

