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

#include "parser_service.hpp"

namespace findik
{
	namespace service
	{
                log4cxx::LoggerPtr parser_service::debug_logger(log4cxx::Logger::getLogger("findik.service.parser_service"));

		parser_service::parser_service()
		{}

		parser_service::~parser_service()
		{}

		void parser_service::register_local_parser(findik::io::protocol proto, 
				findik::parser::abstract_local_parser_ptr local_parser)
		{
			local_parser_map_[proto] = local_parser;
		}

		void parser_service::register_remote_parser(findik::io::protocol proto, 
				findik::parser::abstract_remote_parser_ptr remote_parser)
		{
			remote_parser_map_[proto] = remote_parser;
		}

		boost::tuple<boost::tribool, char*> parser_service::parse(
				findik::io::connection_ptr connection_, bool is_local,
				char* begin, char* end
			)
		{
			if (is_local)
				return local_parser_map_[connection_->proto()]->parse(
						connection_, begin, end
					);
			else
				return remote_parser_map_[connection_->proto()]->parse(
						connection_, begin, end
					);

		}

		void parser_service::update_hostname_of(findik::io::connection_ptr connection_,
				std::string & hostname_)
		{
			local_parser_map_[connection_->proto()]->update_hostname_of(connection_, hostname_);
		}

		void parser_service::update_port_of(findik::io::connection_ptr connection_,
				unsigned int & port_)
		{
			local_parser_map_[connection_->proto()]->update_port_of(connection_, port_);
		}

		void parser_service::update_is_keepalive_of(findik::io::connection_ptr connection_,
				boost::tribool & is_keepalive_)
		{
			remote_parser_map_[connection_->proto()]->update_is_keepalive_of(connection_, is_keepalive_);
		}

		void parser_service::update_keepalive_timeout_of(findik::io::connection_ptr connection_, 
				unsigned int & keepalive_timeout_)
		{
			remote_parser_map_[connection_->proto()]->update_keepalive_timeout_of(connection_, keepalive_timeout_);
		}
	}
}

