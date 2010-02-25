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

#ifndef FINDIK_SERVICE_REPLY_SERVICE_HPP
#define FINDIK_SERVICE_REPLY_SERVICE_HPP

#define FC_BAD_LOCAL 400
#define FC_BAD_REMOTE 500

#define FC_SSL_TMPACCEPT 80001
#define FC_SSL_TMPACCEPT_REQ 80002

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <map>
#include <fstream>

#include "protocol.hpp"
#include "filter_reason.hpp"
#include "authentication_result.hpp"

namespace findik
{
	namespace service
	{
		/*!
		Service for getting prepared replies.
                \extends boost::noncopyable this class has designed to be not copyable.
		*/
		class reply_service :
                        private boost::noncopyable
		{
		public:
			/*!
			Default constructor.
			*/
			reply_service();

			/*!
			Destructor.
			*/
			~reply_service();

			/*!
			Crafts request reply into given stream buffer.

			\param sbuf buffer to populate reply into. 
			\param proto protocol of reply.
			\param reason deny reason of a filter.
			*/
			void reply(boost::asio::streambuf & sbuf, findik::io::protocol proto, 
				findik::filter::filter_reason_ptr reason);

			/*!
			Crafts request reply into given stream buffer.

			\param sbuf buffer to populate reply into. 
			\param proto protocol of reply.
			\param reason result of an authenticator.
			*/
			void reply(boost::asio::streambuf & sbuf, findik::io::protocol proto,
				 findik::authenticator::authentication_result_ptr result);

			/*!
			Crafts request reply into given stream buffer.

			\param sbuf buffer to populate reply into. 
			\param proto protocol of reply.
			\param code reply code.
			*/
			void reply(boost::asio::streambuf & sbuf, findik::io::protocol proto, unsigned int code);

			/*!
			Method to initialize service.
			*/
			void start();

			/*!
			Method to retrieve reply_html_
			*/
			const std::string & reply_html();

			/*!
                        Method to retrieve ssl_reply_html_
                        */
                        const std::string & ssl_reply_html();


		protected:
			/*!
			Map to store stock replies.
			*/
			std::map<findik::io::protocol, std::map<unsigned int, std::string> > stock_replies_;

			/*!
                        String to hold reply html data
                        */
			std::string reply_html_;

			/*!
                        String to hold SSL reply html data
                        */
			std::string ssl_reply_html_;

			/*!
                        Returns GMT Date format like Wed, 18 Feb 2009 12:34:12 GMT
                        */
			std::string generateGMTDate(); 
		};
	}
}

#endif

