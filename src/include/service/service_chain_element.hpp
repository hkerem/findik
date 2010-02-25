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

#ifndef FINDIK_SERVICE_SERVICE_CHAIN_ELEMENT_HPP
#define FINDIK_SERVICE_SERVICE_CHAIN_ELEMENT_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "protocol.hpp"

namespace findik
{
	namespace service
	{
		/*!
		Abstract filter object to inherit in order to implement filters for findik.
		\extends boost::enable_shared_from_this<service_chain_element> to use boost shared pointers.
		@author H. Kerem Cevahir (shelta)
		*/
		class service_chain_element :
			public boost::enable_shared_from_this<service_chain_element>
		{
		public:
			
			/*!
			Protocol of element.
			\returns protocol of element
			*/	
			findik::io::protocol proto();

			/*!
			Whether element should be used in local chain.
			\returns whether element should be used in local chain.
			*/
			bool is_local();

			/*!
			Whether element should be used in remote chain.
			\returns whether element should be used in remote chain.
			*/
			bool is_remote();

		protected:
		
			/*!
			Protocol of element.
			*/	
			findik::io::protocol proto_;

			/*!
			Whether element should be used in local chain.
			*/
			bool is_local_;

			/*!
			Whether element should be used in remote chain.
			*/
			bool is_remote_;

		};
		
		typedef boost::shared_ptr<service_chain_element> service_chain_element_ptr;
	}
}

#endif

