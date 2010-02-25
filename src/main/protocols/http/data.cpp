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

#include "data.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			data::data() :
				http_version_major(0),
				http_version_minor(0),
				content_length_(-1),
				stream_content_size_(0)
			{
				is_stream_ = false;
				is_expecting_eof_ = false;
			}

			void data::mark_as_stream()
			{
				is_stream_ = true;
			}

			void data::add_blank_header()
			{
				headers_.push_back(header());
			}

			header & data::last_header()
			{
				return headers_.back();
			}

			const std::vector<header> & data::get_headers()
			{
				return headers_;
			}

			int data::content_length()
			{
				if (content_length_ == -1)
					BOOST_FOREACH( header h, get_headers() )
						if (h.name == "Content-Length")
						{
							content_length_ = boost::lexical_cast< int >(h.value);
							break;
						}

				return content_length_;
			}

			void data::clear_content()
			{
				content_.clear();
			}

			std::size_t data::content_size()
			{
				if (is_stream())
					return stream_content_size_;
				else
					return abstract_data::content_size();
			}

			void data::add_to_stream_content_size(std::size_t size_)
			{
				stream_content_size_ += size_;
			}

			bool data::is_https()
			{
				return is_https_;
			}

			void data::push_to_content(char input)
			{
				if (is_stream())
					stream_content_size_++;

				abstract_data::push_to_content(input);
			}
		}
	}
}

