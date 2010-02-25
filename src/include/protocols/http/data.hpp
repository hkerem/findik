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

#ifndef FINDIK_PROTOCOLS_HTTP_DATA_HPP
#define FINDIK_PROTOCOLS_HTTP_DATA_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include "abstract_data.hpp"
#include "header.hpp"

#include <vector>
#include <string>

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			/*!
			Base data implementation for HTTP protocol.
			\extends boost::enable_shared_from_this<data> to use boost shared pointers.
			\extends findik::io::abstract_data because this is a data for FINDIK IO subsystem.
			@author H. Kerem Cevahir (shelta)
			*/
			class data :
				public boost::enable_shared_from_this<data>,
				public findik::io::abstract_data
			{
			public:

				/*!
				Major number of HTTP Version.
				*/
				unsigned int http_version_major;

				/*!
				Minor number of HTTP Version.
				*/
				unsigned int http_version_minor;

				/*!
				Whether data is HTTPS data. Will be used for request line regeneration.
				\returns whether data is HTTPS data.
				*/
				bool is_https();

				/*!
				Adds new black header object to end of header array.
				*/
				void add_blank_header();

				/*!
				Set is_stream_ variable to true.
				*/
				void mark_as_stream();

				/*!
				Returns last added header. This method returns a non constant value. This method
				should be used by only parsers.
				\returns last inserted header.
				*/
				header & last_header();

				/*!
				Gets all headers.
				\returns headers array.
				*/
				const std::vector<header> & get_headers();

				/*!
				Length of content specified in Content-Length header.
				\returns length of content.
				*/
				int content_length();

				/*!
				Overrides abstract_data::content_size() because of streaming operation handle requirement.
				\returns recived byte count of content
				*/
				std::size_t content_size();

				/*!
				Increments stream_content_size_ variable
				\param size_ increment amount
				*/
				void add_to_stream_content_size(std::size_t size_);

				/*!
				Pushes character to raw content vector.
				\param input char to be pushed.
				*/
				void push_to_content(char input);

			protected:

				/*!
				Default constructor.
				*/
				data();

				/*!
				HTTP headers.
				*/
				std::vector<header> headers_;

				/*!
				Length of content specified in Content-Length header.
				This variable will be used as cache.
				*/
				int content_length_;

				/*!
				Clears content_ vector.
				*/
				void clear_content();

				/*!
				Content size.
				Only used in streaming operation.
				*/
				std::size_t stream_content_size_;

				/*!
				Whether data is HTTPS data.
				*/
				bool is_https_;
			};
			
			typedef boost::shared_ptr<data> data_ptr;
		}
	}
}

#endif

