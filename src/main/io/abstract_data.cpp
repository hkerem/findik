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

#include "abstract_data.hpp"

namespace findik
{
	namespace io
	{
		bool abstract_data::is_stream()
		{
			return is_stream_;
		}

		bool abstract_data::is_local()
		{
			return is_local_;
		}

		bool abstract_data::is_remote()
		{
			return ! is_local_;
		}

		bool abstract_data::has_content()
		{
			return ! content_.empty();
		}

		void abstract_data::push_to_content(char input)
		{
			content_.push_back(input);
		}

		std::size_t abstract_data::content_size()
		{
			return content_.size();
		}

		const std::vector<char> & abstract_data::content()
		{
			return content_;
		}

		bool abstract_data::is_expecting_eof()
		{
			return is_expecting_eof_;
		}

	}
}

