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

#include "http_filter_reason_factory.hpp"

#include "http_filter_result_generator.hpp"
#include "http_util.hpp"

namespace findik
{
	namespace protocols
	{
		namespace http
		{

			http_filter_reason_factory::initializer::initializer()
                        {
				http_filter_reason_factory_ptr p(new http_filter_reason_factory());

                                FI_SERVICES->filter_srv().register_filter_reason_factory(findik::io::http, p);
                        }

                        http_filter_reason_factory::initializer http_filter_reason_factory::initializer::instance;

			http_filter_reason_factory::http_filter_reason_factory()
			{}

			http_filter_reason_factory::~http_filter_reason_factory()
			{}

			findik::filter::filter_reason_ptr http_filter_reason_factory::create_filter_reason(
					findik::io::connection_ptr connection_, unsigned int code)
			{

				if (code == 0)
				{
					http_filter_result_generator_ptr generator_;
					request_ptr req = last_request_of(connection_);
					response_ptr resp = last_response_of(connection_);

					generator_.reset(
						new http_filter_result_generator(
							"filter_service", true, 
							200, false, "", "",
							connection_, req, resp
						));

					return findik::filter::filter_reason::create_reason(generator_->reply_str(), generator_->log_str());
				}
				else if (code == FC_ACL_DENY)
				{
					http_filter_result_generator_ptr generator_;
					request_ptr req = last_request_of(connection_);

					generator_.reset(
						new http_filter_result_generator(
							"filter_service", false, 
							response::forbidden, true, 
							"ACL deny : " + req->request_host(), 
							req->request_host(), connection_, req
						));

					return findik::filter::filter_reason::create_reason(generator_->reply_str(), generator_->log_str());
				}
				else if (code == FC_ACL_NA)
				{
					http_filter_result_generator_ptr generator_;
					request_ptr req = last_request_of(connection_);

					generator_.reset(
						new http_filter_result_generator(
							"N/A", false, 
							response::forbidden, true, 
							"ACL deny : " + req->request_host(), 
							req->request_host(), connection_, req
						));

					return findik::filter::filter_reason::create_reason(generator_->reply_str(), generator_->log_str());
				}
				else
				{
					findik::filter::filter_reason_ptr p;
					return p;
				}
			}

		}
	}
}

