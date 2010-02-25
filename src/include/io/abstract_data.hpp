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

#ifndef FINDIK_IO_ABSTRACT_DATA_HPP
#define FINDIK_IO_ABSTRACT_DATA_HPP

#include <boost/asio.hpp>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>

namespace findik
{
	namespace io
	{
		/*!
		Abstract data encapsulator.
		\extends boost::enable_shared_from_this<abstract_data> to use boost shared pointers.
		@author H. Kerem Cevahir (shelta)
		*/
		class abstract_data :
			public boost::enable_shared_from_this<abstract_data>
		{
		public:

			/*!
			If target is a stream, fetched data will be sent to other side immediatly 
			even if fetching operation is not completed.
			\returns whether data is a stream.
			*/
			bool is_stream();

			/*!
			Various FINDIK subsystems should know whether data is local in order to process it with
			components prepared for local data.
			\returns whether data is local.
			*/
			bool is_local();

			/*!
			Various FINDIK subsystems should know whether data is remote in order to process it with
			components prepared for remote data.
			\returns whether data is remote.
			*/
			bool is_remote();

			/*!
			Raw data to sent other side.
			\param sbuf streambuffer to insert raw data into.
			*/
			virtual void into_buffer(boost::asio::streambuf & sbuf) = 0;

			/*!
			In some protocols, content will be teriminated with socket close. 
			For this situations connection object should know wheter this is an appropriate behavior.
			\returns whether data is expecting eof.
			*/
			bool is_expecting_eof();

			/*!
			In some cases, data may not have content, it could contain nothing more than meta data.
			For situation like this, filters should know whether data has content in order to avoid
			running unnecessary algorithms.
			\returns whether data has content.
			*/
			bool has_content();

			/*!
			Pushes character to raw content vector.
			\param input char to be pushed.
			*/
			void push_to_content(char input);

			/*!
			Size of raw content.
			\returns size of raw content;
			*/
			std::size_t content_size();

			/*!
			Raw content.
			\returns raw content;
			*/
			const std::vector<char> & content();

		protected:
			
			/*!
			Whether data is local. If false, data is remote.
			*/
			bool is_local_;

			/*!
			Whether data is a stream.
			*/
			bool is_stream_;

			/*!
			Content of data.
			*/
			std::vector<char> content_;

			/*
			Whether data expects EOF.
			*/
			bool is_expecting_eof_;

		};
		
		typedef boost::shared_ptr<abstract_data> abstract_data_ptr;
	}
}

#endif

