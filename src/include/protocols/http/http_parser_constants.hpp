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

#ifndef FINDIK_PROTOCOLS_HTTP_HTTP_PARSER_CONSTANTS_HPP
#define FINDIK_PROTOCOLS_HTTP_HTTP_PARSER_CONSTANTS_HPP

#define FI_CHECK_SINT(arg) if (arg > 1024) return false;
#define FI_CHECK_SSTR(arg) if (arg.size() > 16) return false;
#define FI_CHECK_MSTR(arg) if (arg.size() > 128) return false;
#define FI_CHECK_LSTR(arg) if (arg.size() > 1024) return false;
#define FI_CHECK_HSTR(arg) if (arg.size() > 8192) return false;

#define FI_CHECK_VCTR(arg) if (arg.size() > 128) return false;

#endif

