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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_LDAP

#ifndef FINDIK_UTIL_PASSWD_HASH_HPP
#define FINDIK_UTIL_PASSWD_HASH_HPP

#include <string>
#include <unistd.h>
#include <iostream>
#include <openssl/md5.h>
#include <openssl/sha.h>


namespace findik {
	namespace util {
		
		enum crypt_types {
			CRYPT_STD_DES = 1,
			CRYPT_EXT_DES = 2,
			CRYPT_MD5 = 3,
			CRYPT_BLOWFISH = 4
		};

		bool crypt_check(const std::string & pass, const std::string &hash);
		bool md5_check(const std::string & pass, const std::string &hash);
		bool sha1_check(const std::string & pass, const std::string &hash);

		std::string crypt_hash(const std::string & pass, crypt_types type);
		std::string md5_hash(const std::string & pass);
		std::string sha1_hash(const std::string & pass);

	}
}

#endif
#endif /* HAVE_LDAP */
