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

#ifndef FINDIK_PERSISTENCY_POOLED_DBMANAGER_HPP
#define FINDIK_PERSISTENCY_POOLED_DBMANAGER_HPP

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <vector>

#include <boost/thread/mutex.hpp>

#include "dbmanager.hpp"
#include "dbconnection.hpp"
#include "log.hpp"

namespace findik 
{
	namespace persistency
	{
		template <class T>
		class pooled_dbmanager :
			public findik::persistency::dbmanager,
			public boost::enable_shared_from_this< pooled_dbmanager < T > >
		{
		public:
			typedef boost::shared_ptr< pooled_dbmanager < T > > pointer;

		protected:
			typedef boost::shared_ptr< dbconnection < T > > dbconnection_ptr;

			void prepare_pool();

			std::size_t pool_size_;

			std::size_t pool_index_;

			std::vector<dbconnection_ptr> pool_;

			virtual dbconnection_ptr create_connection_object() = 0;

			dbconnection_ptr get_dbconnection();

			boost::mutex mutex_;

			static log4cxx::LoggerPtr debug_logger;
		};

		template <class T>
		void pooled_dbmanager<T>::prepare_pool()
		{
			for (size_t i = 0; i < pool_size_; ++i)
				pool_.push_back(create_connection_object());

			pool_index_ = 0;
		}

		template <class T>
		typename pooled_dbmanager<T>::dbconnection_ptr pooled_dbmanager<T>::get_dbconnection()
		{
			dbconnection_ptr connection;

			do
			{
				connection = pool_[pool_index_];
				{
					boost::mutex::scoped_lock lock1(mutex_);
					pool_index_++;
					pool_index_ = pool_index_%pool_size_;
				}
				//LOG4CXX_DEBUG(debug_logger, "trying to get connection " << pool_index_);
			}
			while (!connection->try_lock());

			//LOG4CXX_DEBUG(debug_logger, "got connection " << 	pool_index_ << ". returning.");

			return connection;
		}

		template <class T>
		log4cxx::LoggerPtr pooled_dbmanager<T>::debug_logger(log4cxx::Logger::getLogger("findik.persistency.pooled_dbmanager"));

	}
}

#endif

