/*
  Copyright (C) 2008 H. Kerem Cevahir (shelta), Burak OGUZ (barfan) <findikmail@gmail.com>

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

#include "pcre_service.hpp"
#ifdef HAVE_PCRE
#include "pcre_analyzer.hpp"
#include "service_container.hpp"

namespace findik
{
	namespace service
	{
		pcre_service::pcre_service() 
		{}

		pcre_service::~pcre_service() 
		{}

		void pcre_service::start()
		{
			std::list<boost::tuple<int,std::string> > pcre_map;
                        FI_SERVICES->db_srv().pcreQuery(pcre_map);
                        for(std::list<boost::tuple<int,std::string> >::iterator iter = pcre_map.begin(); 
				iter != pcre_map.end(); iter++)
                        {
				int catid;
				std::string regex;
				boost::tie(catid, regex) = *iter;
                                re_vector.push_back(new findik::util::pcre_parser(catid, regex));
                        }

		}

		bool pcre_service::matches_predefined(const std::string & data, unsigned int category)
		{
			bool result = false;
			
			pcrecpp::StringPiece *input = new pcrecpp::StringPiece(data);
			std::string match;

			for( int i = 0, j = 0; i < re_vector.size(); i++ ) 
			{
				if (re_vector[i]->get_category_id() == category)
				{
					if (re_vector[i]->get_re()->FindAndConsume(input,&match))
					{
						result = true;
						break;
					}
				}
			}

			delete input;
			
			return result;

		}

		std::vector<findik::util::pcre_analyzer> pcre_service::matches_custom(const std::string & pattern, const std::string & data)
		{	
			pcrecpp::StringPiece *input = new pcrecpp::StringPiece(data);
			std::string match;
			std::vector<findik::util::pcre_analyzer> results;
			findik::util::pcre_parser_ptr pcre(new findik::util::pcre_parser(0, pattern));
			int j = 0;
	
			while(pcre->get_re()->FindAndConsume(input,&match)) {
				std::transform(match.begin(), match.end(), match.begin(), tolower);
				for(j = 0; j < results.size(); j++) {
					if(results[j].get_word() == match) {
						results[j].set_count(results[j].get_count()+1);
						break;
					}
				}
				if(j == results.size()) {
					findik::util::pcre_analyzer analyzer;
					analyzer.set_word(match);
					analyzer.set_count(1);
					analyzer.set_catid(pcre->get_category_id());
					results.push_back(analyzer);
				}
			}	

			delete input;

			return results;
		}

		void pcre_service::global_replace( const std::string & pattern, const std::string & replace, std::string & data)
		{
                        findik::util::pcre_parser_ptr pcre(new findik::util::pcre_parser(0, pattern));
			pcrecpp::StringPiece *replace_piece = new pcrecpp::StringPiece(replace);
			std::string input(data.c_str(), data.length());

			// int i = pcre->get_re()->GlobalReplace(replace_piece, &input);
			int i = pcrecpp::RE(pattern).GlobalReplace(*replace_piece,&data);

			delete replace_piece;
		}


	}
}

#endif /* HAVE_PCRE */
