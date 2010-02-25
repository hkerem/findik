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

#include "clamd_av_filter.hpp"
#ifdef HAVE_CLAMD
#include <sstream>

namespace findik
{
	namespace protocols
	{
		namespace http
		{
			// initialization of logger
			log4cxx::LoggerPtr clamd_av_filter::debug_logger_(log4cxx::Logger::getLogger("findik.protocols.http.clamd_av_filter"));	
			std::string clamd_av_filter::filter_code_ = "av";	

			// constructor definition of filter service registration inner class
			clamd_av_filter::initializer::initializer()
			{
				clamd_av_filter_ptr dfp(new clamd_av_filter());

				if(FI_CONFIG.use_clamd())
				{	
					try
					{
						boost::asio::io_service io_service;
						boost::asio::ip::tcp::resolver resolver(io_service);
						boost::asio::ip::tcp::resolver::query query(FI_CONFIG.clamd_host(), FI_CONFIG.clamd_port());
						boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
						boost::asio::ip::tcp::resolver::iterator end;

						boost::asio::ip::tcp::socket socket(io_service);
						boost::system::error_code error = boost::asio::error::host_not_found;
						while (error && endpoint_iterator != end)
						{
							socket.close();
							socket.connect(*endpoint_iterator++, error);
						}
						if (error)
							throw boost::system::system_error(error);

						boost::asio::write(socket, boost::asio::buffer("PING\r\n", 6));

						char reply[4];
						boost::asio::read(socket, boost::asio::buffer(reply, 4));

						std::string received_data(reply);
						// Check that response is OK.
						socket.close();

						if(received_data.substr(0,3) == "PON") {
							FI_SERVICES->filter_srv().register_filter(filter_code_,dfp);
							LOG4CXX_DEBUG(debug_logger_, "Clamd AV filter is running");
						}
						else
							LOG4CXX_ERROR(debug_logger_, "Clamd AV filter is not running");

					}
					catch (std::exception& e)
					{
						LOG4CXX_ERROR(debug_logger_, "Clamd AV filter is not running. Exception: " + (std::string)e.what());
					}

				}

			}

			clamd_av_filter::initializer clamd_av_filter::initializer::instance;

			boost::tuple<bool, findik::filter::filter_reason_ptr> 
					clamd_av_filter::filter(findik::io::connection_ptr connection_, unsigned int param) 
			{
				LOG4CXX_DEBUG(debug_logger_, "Clamd AV filter entered"); // log for filter entrance
				response_ptr resp = boost::static_pointer_cast<response>(connection_->current_data());
				request_ptr req = last_request_of(connection_);

				if(resp->content().size() > 0) {
					try
					{
						// Prepare first socket
						boost::asio::ip::tcp::resolver::query query(FI_CONFIG.clamd_host(), FI_CONFIG.clamd_port());
						boost::asio::ip::tcp::resolver::iterator endpoint_iterator = FI_SERVICES->resolver_srv().resolve(query);
						boost::asio::ip::tcp::resolver::iterator end;
						boost::asio::ip::tcp::socket socket(FI_SERVICES->io_srv());
						boost::system::error_code error = boost::asio::error::host_not_found;
						while (error && endpoint_iterator != end)
						{
							socket.close();
							socket.connect(*endpoint_iterator++, error);
						}
						if (error)
							throw boost::system::system_error(error);

						boost::asio::write(socket, boost::asio::buffer("STREAM\r\n", 8));

						boost::asio::streambuf response;
						boost::asio::read_until(socket, response, "\n");
						std::istream response_stream(&response);
						std::string port_str;
						std::getline(response_stream, port_str);

						// ---------------------------------
						// Prepare second socket
						boost::asio::ip::tcp::resolver::query query2(FI_CONFIG.clamd_host(), port_str.substr(5));
						boost::asio::ip::tcp::resolver::iterator endpoint_iterator2 = FI_SERVICES->resolver_srv().resolve(query2);
						boost::asio::ip::tcp::resolver::iterator end2;
						boost::asio::ip::tcp::socket socket2(FI_SERVICES->io_srv());
						boost::system::error_code error2 = boost::asio::error::host_not_found;
						while (error2 && endpoint_iterator2 != end)
						{
							socket2.close();
							socket2.connect(*endpoint_iterator2++, error2);
						}
						if (error)
							throw boost::system::system_error(error2);

						boost::asio::write(socket2, boost::asio::buffer(&(resp->content())[0], resp->content().size()));
						boost::asio::write(socket2, boost::asio::buffer("\r\n", 2));

						socket2.close();
						// ---------------------------------

						boost::asio::read_until(socket, response, "\0");
						std::string av_result;
						std::getline(response_stream, av_result);
						// stream: Eicar-Test-Signature FOUND
						// stream: OK
						av_result = av_result.substr(8);
						int pos = av_result.find(" FOUND");

						if(pos != std::string::npos) 
						{
							boost::shared_ptr<http_filter_result_generator> reply_(new http_filter_result_generator(filter_code_, false, response::forbidden, true, "Virus Found : " + av_result.substr(0,pos), av_result.substr(0,pos), connection_, req, resp));
							return boost::make_tuple(false, findik::filter::filter_reason::create_reason(reply_->reply_str(), reply_->log_str()));
						}	
						socket.close();
					}
					catch (std::exception& e)
					{
						LOG4CXX_ERROR(debug_logger_, "Clamd AV filter EXCEPTION " + std::string(e.what())); // log for filter entrance
						return boost::make_tuple(false, findik::filter::filter_reason::create_reason());
					}
				}
			
				findik::filter::filter_reason_ptr frp_;
				return boost::make_tuple(true, frp_);
			}

			bool clamd_av_filter::is_applicable(findik::io::connection_ptr connection_)
			{
				// set this filter to be used in response only
				return connection_->proto() == findik::io::http 
					&& connection_->current_data()->is_remote()
					&& connection_->current_data()->has_content();	
			}
		}
	}
}
#endif
