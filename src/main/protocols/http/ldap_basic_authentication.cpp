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

#include "ldap_basic_authentication.hpp"
#ifdef HAVE_LDAP

#include "base64.hpp"
#include <ldap.h>

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			// initialization of logger
			log4cxx::LoggerPtr ldap_basic_authentication::debug_logger_(log4cxx::Logger::getLogger("findik.protocols.http.ldap_basic_authentication"));	

			int ldap_basic_authentication::authenticator_code_ = 102;
			char *ldap_basic_authentication::result_attrs_[2];
			//BerValue ldap_basic_authentication::bind_dn_creds;
			struct timeval ldap_basic_authentication::tv_;
			std::string ldap_basic_authentication::ldap_server_ = FI_CONFIG.ldap_server_name();
			std::string ldap_basic_authentication::bind_dn_name_ = FI_CONFIG.ldap_bind_dn_name();
			std::string ldap_basic_authentication::base_dn_ = FI_CONFIG.ldap_base_dn();
			std::string ldap_basic_authentication::extra_filter_ = FI_CONFIG.ldap_extra_filter();
			std::string ldap_basic_authentication::username_attr_ = FI_CONFIG.ldap_search_attr();
			std::string ldap_basic_authentication::password_attr_ = FI_CONFIG.ldap_password_attr();
			std::map<std::string,std::string> ldap_basic_authentication::auth_cache_;

			// constructor definition of filter service registration inner class
			ldap_basic_authentication::initializer::initializer()
                        {
                                ldap_basic_authentication_ptr akn(new ldap_basic_authentication());
			
				BerValue bind_dn_creds;

				bind_dn_creds.bv_len=FI_CONFIG.ldap_bind_dn_password().length();
                                bind_dn_creds.bv_val=(char *)FI_CONFIG.ldap_bind_dn_password().c_str();

				result_attrs_[0] = (char *)FI_CONFIG.ldap_password_attr().c_str();
				result_attrs_[1] = NULL;

				tv_.tv_sec=60*60;
	                        tv_.tv_usec=0;
				
				LDAP *ld = (LDAP *) NULL;
				int rc = -1;

				if(FI_CONFIG.use_ldap_basic_auth()) {

                                        rc = ldap_initialize (&ld, ldap_basic_authentication::ldap_server_.c_str());

                                        if (rc != LDAP_SUCCESS)
                                        {
						LOG4CXX_ERROR(debug_logger_, "Could not initiazlize LDAP, reason: " + std::string(ldap_err2string(rc)));
                                                return;
                                        }


                                        rc=ldap_sasl_bind_s(ld,bind_dn_name_.c_str(),NULL,&bind_dn_creds,NULL,NULL,NULL);
                                        if (rc != LDAP_SUCCESS)
                                        {
						LOG4CXX_ERROR(debug_logger_, "Could not bind to LDAP server, reason: " + std::string(ldap_err2string(rc)));
                                                return;
                                        }

	                                FI_SERVICES->authentication_srv().register_authenticator(akn);			
				}
                        }

                        ldap_basic_authentication::initializer ldap_basic_authentication::initializer::instance;

			boost::tuple<bool, findik::authenticator::authentication_result_ptr> ldap_basic_authentication::authenticate(findik::io::connection_ptr connection_) 
			{
				LOG4CXX_DEBUG(debug_logger_, "LDAP Basic Authenticator entered"); // log for filter entrance
						
				// get request object from current data
				request_ptr req = boost::static_pointer_cast<request>(connection_->current_data());
				std::string auth = "";
				
				// get hostname from request header
				BOOST_FOREACH( header h, req->get_headers() ) {
					if (h.name == "Proxy-Authorization") {
						auth = h.value;
					}
				}
				if( auth == "")
					return boost::make_tuple(false, findik::authenticator::authentication_result::create_result(authenticator_code_,"Proxy authorization required", authenticator_code_, false, findik::io::http));	
				else {

					std::string auth_response = base64_decode(auth.substr(6));
					std::string username = "", passwd = "";
					LDAP *ld = (LDAP *) NULL;
					LDAPMessage *result = (LDAPMessage *) NULL;
					int rc = -1;
					int msgid;
					size_t found;
					BerValue bind_dn_creds;
					std::map<std::string,std::string>::iterator it;

  					found=auth_response.find(":");
					if (found != std::string::npos)
					{
						username = auth_response.substr(0,found);
						passwd = auth_response.substr(found+1); 		
					}
					if(username == "" ||passwd == "")
						goto fail;
					
					it = auth_cache_.find(username);					
					if(it != auth_cache_.end()) 
					{
						if(it->second == findik::util::md5_hash(passwd)) {
							LOG4CXX_DEBUG(debug_logger_, "Authentication done from cache for LDAP for user: " + username);
							return boost::make_tuple(true, findik::authenticator::authentication_result::create_result(0));							
						}
					}

	                                bind_dn_creds.bv_len=FI_CONFIG.ldap_bind_dn_password().length();
        	                        bind_dn_creds.bv_val=(char *)FI_CONFIG.ldap_bind_dn_password().c_str();
					
					rc = ldap_initialize (&ld, ldap_server_.c_str());

					if (rc != LDAP_SUCCESS)
					{
						LOG4CXX_ERROR(debug_logger_, "Could not initiazlize LDAP, reason: " + std::string(ldap_err2string(rc)));				
						goto fail;
					}


					if(bind_dn_name_ == "")
						rc=ldap_sasl_bind_s(ld,NULL,NULL,NULL,NULL,NULL,NULL);
					else
						rc=ldap_sasl_bind_s(ld,bind_dn_name_.c_str(),NULL,&bind_dn_creds,NULL,NULL,NULL);
					if (rc != LDAP_SUCCESS)
					{
						LOG4CXX_ERROR(debug_logger_, "Could not bind to LDAP server, reason: " + std::string(ldap_err2string(rc)));
						goto fail;
					}

						
					rc = ldap_search_ext(ld,base_dn_.c_str(),LDAP_SCOPE_SUBTREE,("(&"+ extra_filter_ +"(" + username_attr_ + "=" + username + "))").c_str(),result_attrs_, 0,NULL,NULL,&tv_,1000000,&msgid);

					if (rc != LDAP_SUCCESS)
					{
						LOG4CXX_ERROR(debug_logger_, "Could not search LDAP server, reason: " + std::string(ldap_err2string(rc)));
						goto fail;
					}

					rc=ldap_result(ld, msgid, 0, &tv_, &result);

					if(ldap_count_messages(ld,result) == 1) {

						LDAPMessage *entry = ldap_first_message(ld,result);

						BerValue **n_val = ldap_get_values_len(ld, entry, password_attr_.c_str());

						if(n_val != NULL) {
							std::string userpass(n_val[0]->bv_val);
							bool is_passwd_correct = false;	
							if(userpass.substr(1,5) == "crypt" || userpass.substr(1,5) == "CRYPT") {
								if(findik::util::crypt_check(passwd,userpass.substr(7))) 
									is_passwd_correct = true;
							}
							else if(userpass.substr(1,3) == "md5" || userpass.substr(1,3) == "MD5") {
								if(findik::util::md5_check(passwd,userpass.substr(5))) 
									is_passwd_correct = true;
							}
							else if(userpass.substr(1,3) == "sha" || userpass.substr(1,3) == "SHA") {
								if(findik::util::sha1_check(passwd,userpass.substr(5))) 
									is_passwd_correct = true;
							}
							else if(userpass.substr(1,9) == "cleartext" || userpass.substr(1,9) == "CLEARTEXT") {
								if(passwd == userpass.substr(11)) 
									is_passwd_correct = true;
							}

							if(is_passwd_correct)
							{
								if(it != auth_cache_.end())
									auth_cache_.erase(it);
								auth_cache_.insert(std::pair<std::string,std::string>(username,findik::util::md5_hash(passwd)));
								LOG4CXX_DEBUG(debug_logger_, "Authentication done from LDAP host for user: " + username);
								return boost::make_tuple(true, findik::authenticator::authentication_result::create_result(0));
							}
						}
					}
					
					fail:
					return boost::make_tuple(false, findik::authenticator::authentication_result::create_result(authenticator_code_,"Proxy authorization required", authenticator_code_, false, findik::io::http));	
				}

			}

			findik::io::protocol ldap_basic_authentication::proto()
			{
				// set this filter to be used in request only
				return findik::io::http;	
			}
		}
	}
}

#endif /* HAVE_LDAP */
