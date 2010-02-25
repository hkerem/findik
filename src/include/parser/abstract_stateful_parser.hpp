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

#ifndef FINDIK_PARSER_ABSTRACT_STATEFUL_PARSER_HPP
#define FINDIK_PARSER_ABSTRACT_STATEFUL_PARSER_HPP

#include "connection.hpp"

#define FC_LPARSER_STATE 1001
#define FC_RPARSER_STATE 1002

#define FIR_LSTATE_OF(conn) conn->uint_map[FC_LPARSER_STATE]
#define FI_LSTATE_OF(conn) FIR_LSTATE_OF(conn)

#define FIR_RSTATE_OF(conn) conn->uint_map[FC_RPARSER_STATE]
#define FI_RSTATE_OF(conn) FIR_RSTATE_OF(conn)

#endif

