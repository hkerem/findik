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

#ifndef FINDIK_PROTOCOLS_HTTP_PARSER_UTIL_HPP
#define FINDIK_PROTOCOLS_HTTP_PARSER_UTIL_HPP

#include <string>

namespace findik 
{
	namespace protocols 
	{
		namespace http 
		{

			/*!
			A method to check if a byte is an HTTP character.

			\param c byte to be checked
			\return true if given byte is an HTTP character.
			*/
			static bool is_char(int c)
			{
				return c >= 0 && c <= 127;
			}

			/*!
			Check if a byte is an HTTP control character.

			\param c byte to be checked
			\return true if given byte is an HTTP control character.
			*/
			static bool is_ctl(int c)
			{
				return c >= 0 && c <= 31 || c == 127;
			}

			/*!
			Check if a byte is defined as an HTTP tspecial character.

			\param c byte to be checked
			\return true if given byte is defined as an HTTP tspecial character.
			*/
			static bool is_tspecial(int c)
			{
				switch (c)
				{
					case '(': case ')': case '<': case '>': case '@':
					case ',': case ';': case ':': case '\\': case '"':
					case '/': case '[': case ']': case '?': case '=':
					case '{': case '}': case ' ': case '\t':
						return true;
					default:
						return false;
				}
			}

			/*!
			Check if a byte is a digit.

			\param c byte to be checked
			\return true if given byte is a digit.
			*/
			static bool is_digit(int c)
			{
				return c >= '0' && c <= '9';
			}

			/*!
			Check if all characters of string is figit.

			\param c string to be checked
			\return true if given string is all consisted of digits.
			*/
			static bool is_digit(const std::string & str_)
			{
				for (int i = 0; i < str_.length(); i++)
					if (!is_digit(str_[i]))
						return false;
				return true;
			}

			/*!
			Check if a byte is a hex digit.

			\param c byte to be checked
			\return true if given byte is a hex digit.
			*/
			static bool is_hex(int c)
			{
				if (is_digit(c) ||
					( c >= 'a' && c <= 'f' ) ||
					( c >= 'A' && c <= 'F' )
					)
					return true;
				else
					return false;
			}

			/*!
			Converts given hex string to integer.

			\param hex hex string to convert.
			\return integer value of hex string.
			*/
			static int hex2int(std::string hex){
				std::stringstream ss(hex);
				int i;
				ss>>std::hex>>i;
				return i;
			}

			/*!
			Check if a byte is defined as an HTTP URI unreserved character.

			\param c byte to be checked
			\return true if given byte is defined as an HTTP URI unreserved character.
			*/
			static bool is_uri_char_unreserved(int c)
			{
				if (is_digit(c) || isalpha(c))
					return true;
				switch (c)
				{
					case '-': case '_': case '.': case '~':
					return true;
				}

				return false;
			}

			/*!
			Check if a byte is defined as an HTTP URI reserved character.

			\param c byte to be checked
			\return true if given byte is defined as an HTTP URI reserved character.
			*/
			static bool is_uri_char_reserved(int c) {
				switch (c)
				{
					case '!': case '*': case '\'': case '(': case ')':
					case ';': case ':': case '@': case '&': case '=':
					case '+': case '$': case ',': case '?': case '/':
					case '%': case '#': case '[': case ']':
					return true;
				}

				return false;
			}

			/*!
			Check if a byte is defined as an HTTP URI character.

			\param c byte to be checked
			\return true if given byte is defined as an HTTP URI character.
			*/
			static bool is_uri_char(int c)
			{
				return is_uri_char_unreserved(c) || is_uri_char_reserved(c);
			}

			/*!
			Trims trailing and heading spaces of specified string.

			\param str string to be trimmed.
			*/
			static void trim_spaces( std::string& str)
			{
			    std::size_t startpos = str.find_first_not_of(" \t");
			    std::size_t endpos = str.find_last_not_of(" \t");

			    if(( std::string::npos == startpos ) || ( std::string::npos == endpos))
				str = "";
			    else
				str = str.substr( startpos, endpos - startpos + 1 );
			}

		}
	}
}

#endif

