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

#ifndef FINDIK_FILTER_FILTER_REASON_HPP
#define FINDIK_FILTER_FILTER_REASON_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <string>

namespace findik
{
	namespace filter
	{
		/*!
		Filter reason type to return for denying content with a filter.
		\extends boost::enable_shared_from_this<filter_reason> to use boost shared pointers.
		@author H. Kerem Cevahir (shelta)
		*/
		class filter_reason :
			public boost::enable_shared_from_this<filter_reason>
		{
		public:
			/*!
			Factory method.
			\param code code of reason.
			\returns a new filter reason instance
			*/
			static boost::shared_ptr<filter_reason> create_reason()
			{
				boost::shared_ptr<filter_reason> p(new filter_reason());
				return p;
			}

			/*
			Factory method.
			\param reason_str detail of reason.
			\param log_str log of reason.
			\returns a new filter reason instance
			*/
			static boost::shared_ptr<filter_reason> create_reason(
					const std::string & reply_str, const std::string & log_str)
			{
				boost::shared_ptr<filter_reason> p(new filter_reason(reply_str, log_str));
				return p;
			}

			/*!
			Destructor.
			*/
			~filter_reason();

			/*!
			Reply string which will return to user
			\returns reason details
			*/
			const std::string & reply_str();

			/*!
                        Detail of log.
                        \returns log details
                        */
                        const std::string & log_str();

		protected:
			/*!
			Constructor.
			*/
			filter_reason();

			/*!
			Constructor.
			*/
			filter_reason(const std::string & reply_str, const std::string & log_str);

			/*!
			Reason detail in a string.
			*/
			std::string reply_str_;

			/*!
                        Log string.
                        */
                        std::string log_str_;
		};
		
		typedef boost::shared_ptr<filter_reason> filter_reason_ptr;
	}
}

#endif

