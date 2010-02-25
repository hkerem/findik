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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "server.hpp"
#include "service_container.hpp"

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/thread.hpp>
#include <pthread.h>
#include <signal.h>

#include <syslog.h>
#include <pwd.h>

#ifdef HAVE_BOOST_PO
#include <boost/program_options.hpp>
#endif

#include <sys/stat.h>

#define DAEMON_NAME "findik"
#define RUN_AS_USER "daemon"

#define FINDIK_CONFIG_FILE "/etc/findik/findik.cfg"
#define FINDIK_LOG_CONFIG_FILE "/etc/findik/findik_log.conf"

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

static void child_handler(int signum)
{
	switch(signum) {
		case SIGALRM: exit(EXIT_FAILURE); break;
		case SIGUSR1: exit(EXIT_SUCCESS); break;
		case SIGCHLD: exit(EXIT_FAILURE); break;
	}
}

static void daemonize()
{
	pid_t pid, sid, parent;
	int lfp = -1;

	/* already a daemon */
	if ( getppid() == 1 ) return;

	/* Drop user if there is one, and we were run as root */
	if ( getuid() == 0 || geteuid() == 0 ) {
		struct passwd *pw = getpwnam(RUN_AS_USER);
		if ( pw ) {
			syslog( LOG_NOTICE, "setting user to " RUN_AS_USER );
			setuid( pw->pw_uid );
		}
	}

	/* Trap signals that we expect to receive */
	signal(SIGCHLD,child_handler);
	signal(SIGUSR1,child_handler);
	signal(SIGALRM,child_handler);

	/* Fork off the parent process */
	pid = fork();
	if (pid < 0) {
		syslog( LOG_ERR, "unable to fork daemon, code=%d (%s)",
			errno, strerror(errno) );
		exit(EXIT_FAILURE);
	}

	/* If we got a good PID, then we can exit the parent process. */
	if (pid > 0) {
		/* Wait for confirmation from the child via SIGTERM or SIGCHLD, or
		for two seconds to elapse (SIGALRM).  pause() should not return. */
		alarm(2);
		pause();

		exit(EXIT_FAILURE);
	}

	/* At this point we are executing as the child process */
	parent = getppid();

	/* Cancel certain signals */
	signal(SIGCHLD,SIG_DFL); /* A child process dies */
	signal(SIGTSTP,SIG_IGN); /* Various TTY signals */
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGHUP, SIG_IGN); /* Ignore hangup signal */
	signal(SIGTERM,SIG_DFL); /* Die on SIGTERM */

	/* Change the file mode mask */
	umask(0);

	/* Create a new SID for the child process */
	sid = setsid();
	if (sid < 0) {
		syslog( LOG_ERR, "unable to create a new session, code %d (%s)", errno, strerror(errno) );
		exit(EXIT_FAILURE);
	}

	/* Change the current working directory.  This prevents the current
	directory from being locked; hence not being able to remove it. */
	if ((chdir("/")) < 0) {
		syslog( LOG_ERR, "unable to change directory to %s, code %d (%s)", "/", errno, strerror(errno) );
		exit(EXIT_FAILURE);
	}

	/* Redirect standard files to /dev/null */
	freopen( "/dev/null", "r", stdin);
	freopen( "/dev/null", "w", stdout);
	freopen( "/dev/null", "w", stderr);

	/* Tell the parent process that we are A-okay */
	kill( parent, SIGUSR1 );
}

int main(int argc, char* argv[])
{
	try
	{
		bool _daemonize = false;
		std::string findik_conf(FINDIK_CONFIG_FILE);
		std::string findik_log_conf(FINDIK_LOG_CONFIG_FILE);

#ifdef HAVE_BOOST_PO
		// Declare the supported options.
		boost::program_options::options_description desc("FINDIK binary.");
		desc.add_options()
			("help,h", "Produces this help message")
			("daemonize,d", "Run FINDIK process in background.")
			("config,c", boost::program_options::value<std::string>(), 
				"Set FINDIK config file. (default: " FINDIK_CONFIG_FILE " )")
			("log,l", boost::program_options::value<std::string>(), 
				"Set FINDIK log4j config file. (default: " FINDIK_LOG_CONFIG_FILE " )")
			;

		boost::program_options::variables_map vm;
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
		boost::program_options::notify(vm);    

		if (vm.count("help"))
		{
			std::cout << desc << "\n";
			return 1;
		} 

		if(vm.count("daemonize"))
			_daemonize = true;

		if(vm.count("config"))
			findik_conf = vm["config"].as<std::string>();

		if(vm.count("log"))
			findik_log_conf = vm["log"].as<std::string>();
		
#else
		_daemonize = true;
#endif

		if (_daemonize)
			daemonize();

		// Block all signals for background thread.
		sigset_t new_mask;
		sigfillset(&new_mask);
		sigset_t old_mask;
		pthread_sigmask(SIG_BLOCK, &new_mask, &old_mask);

		// Run server in background thread.
		std::string address("0.0.0.0");
		unsigned int port = 8080;
		unsigned int ssl_port = 8443;

		// Initialize request filter
		//findik::filter::generate_request_filter_factory_map();
		//findik::filter::generate_response_filter_factory_map();

#ifdef HAVE_LOG4CXX
		findik::logging::log_initializer log_init;
		log_init.load_conf("/etc/findik/findik_log.conf");
#endif
		//First accesto FI_SERVICES must be under log init.
		if (! FI_SERVICES->check_config())
			exit(1);

		FI_SERVICES->start();

		// Initialise server.
		findik::io::server s(findik::io::http, address, port);
#ifdef HAVE_SSL
		findik::io::server s2(findik::io::http, address, ssl_port, true);
#endif

		// LOG4CXX_INFO(findik::log_initializer::user_logger,"findik started to listen " + address + ":" + port);

		// Create a pool of threads to run all of the io_services.
		std::vector<boost::shared_ptr<boost::thread> > threads;
		for (std::size_t i = 0; i < FI_CONFIG.io_number_of_threads(); ++i)
		{
			boost::shared_ptr<boost::thread> thread(new boost::thread(
			boost::bind(&boost::asio::io_service::run, &(FI_SERVICES->io_srv()))));
			threads.push_back(thread);
		}

		// LOG4CXX_DEBUG(findik::logging::log_initializer::debug_logger,"listening with " << num_threads << " threads");

		// Restore previous signals.
		pthread_sigmask(SIG_SETMASK, &old_mask, 0);

		// Wait for signal indicating time to shut down.
		sigset_t wait_mask;
		sigemptyset(&wait_mask);
		sigaddset(&wait_mask, SIGINT);
		sigaddset(&wait_mask, SIGQUIT);
		sigaddset(&wait_mask, SIGTERM);
		pthread_sigmask(SIG_BLOCK, &wait_mask, 0);

		int sig = 0;
		sigwait(&wait_mask, &sig);

		FI_SERVICES->io_srv().stop(); // stop IO service

		

		// Wait for all threads in the pool to exit.
		for (std::size_t i = 0; i < threads.size(); ++i)
			threads[i]->join();
	}
	catch (std::exception& e)
	{
		std::cerr << "exception: " << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

