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

#include "passwd_hash.hpp"
#ifdef HAVE_LDAP

namespace findik {
	namespace util {
		
		bool crypt_check(const std::string & pass, const std::string & hash)
		{
			if(hash.length() == 13)
			{
				// Standart DES
				if(std::string((const char *)crypt(pass.c_str(), 
							hash.substr(0,2).c_str())) == hash)
					return true;
			} else if (hash.length() == 20) {
				// Extended DES
				if(std::string((const char *)crypt(pass.c_str(),
                                                        hash.substr(0,9).c_str())) == hash)
					return true;
			} else if (hash.substr(0,3) == "$1$") {
				// MD5
				if(std::string((const char *)crypt(pass.c_str(),
                                                        hash.substr(0,11).c_str())) == hash)
                                        return true;
			} else if (hash.substr(0,3) == "$2$") {
				// SHA1 with $2$ header
                                if(std::string((const char *)crypt(pass.c_str(),
                                                        hash.substr(0,19).c_str())) == hash)
                                        return true;
                        } else if (hash.substr(0,4) == "$2a$") {
				// SHA1 with $2a$ header
                                if(std::string((const char *)crypt(pass.c_str(),
                                                        hash.substr(0,20).c_str())) == hash)
                                        return true;
			} 

			return false;
		}

                bool md5_check(const std::string & pass, const std::string & hash)
		{
			unsigned char md[16];
			MD5((const unsigned char *)pass.c_str(), pass.length(), md);
			std::string sha1_value((const char *)md);
			if(hash == sha1_value)
				return true;
			return false;
		}

                bool sha1_check(const std::string & pass, const std::string & hash)
		{
			unsigned char md[20];
			SHA1((const unsigned char *)pass.c_str(), pass.length(), md);
			std::string sha1_value((const char *)md);
			if(hash == sha1_value)
				return true;
			return false;
		}
		
		std::string crypt_hash(const std::string & pass,  crypt_types type)
		{
			std::string hash_value = "";

			return hash_value;
		}

                std::string md5_hash(const std::string & pass)
		{
			unsigned char md[16];
			MD5((const unsigned char *)pass.c_str(), pass.length(), md);
			return std::string((const char *)md);
		}

                std::string sha1_hash(const std::string & pass)
		{
			unsigned char md[20];
			SHA1((const unsigned char *)pass.c_str(), pass.length(), md);
			return std::string((const char *)md);
		}
	}
}

#endif /* HAVE_LDAP */
