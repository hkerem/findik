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

#ifndef FINDIK_UTIL_ZLIB_UTIL_HPP
#define FINDIK_UTIL_ZLIB_UTIL_HPP

#define FC_ZLIB_BUFFER_SIZE 8192

#include "service_container.hpp"

#include <zlib.h>
#include <vector>

namespace findik 
{
	namespace util
	{

		/*!
		Uncompresses source data to destination for gzip and zlib compression by using zlib library.

		\param src_ compressed data.
		\param dest_ destination to store uncompressed data.

		\return return code of zlib library. If operation is completed successfully, function will return Z_OK ( 0 ).
		*/
		int zlib_inflate(const std::vector<char> src_, std::vector<char> & dest_) {

			int ret = 0;
			unsigned int have = 0;
			z_stream strm;

			dest_.clear();

			unsigned char buffer_[FC_ZLIB_BUFFER_SIZE];

			/* allocate deflate state */
			strm.zalloc = Z_NULL;
			strm.zfree = Z_NULL;
			strm.opaque = Z_NULL;

			strm.avail_in = 0;
			strm.next_in = Z_NULL;

			ret = inflateInit2(&strm, 47);

			if (ret != Z_OK)
				return ret;

			/* uncompress until end of file */
			do {
				strm.avail_in = src_.size();
				strm.next_in = (Bytef*) (&(src_[0]));

				if (strm.avail_in == 0)
				break;

				do {
					strm.avail_out = FC_ZLIB_BUFFER_SIZE;
					strm.next_out = buffer_;

					ret = inflate(&strm, Z_NO_FLUSH); /* no bad return value */

					switch (ret)
					{
					case Z_STREAM_ERROR:
						return ret;
					case Z_NEED_DICT:
						ret = Z_DATA_ERROR;	/* and fall through */
					case Z_DATA_ERROR:
					case Z_MEM_ERROR:
						(void)inflateEnd(&strm);
						return ret;
					}

					have = FC_ZLIB_BUFFER_SIZE - strm.avail_out;

					std::vector<char>::iterator it = dest_.end();

					dest_.insert(it, (const char *)buffer_, (const char *)buffer_ + have);

				} while (strm.avail_out == 0);

				/* done when last data in file processed */
			} while (ret != Z_STREAM_END);

			/* clean up and return */
			(void)inflateEnd(&strm);

			return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
		}

	}
}

#endif

