noinst_LTLIBRARIES		=	libfindik_logging.la

libfindik_logging_la_SOURCES	= \
	log_initializer.cpp

if HAVE_LOG4CXX
else
libfindik_logging_la_SOURCES    =      	logger.cpp logger_ptr.cpp
endif

libfindik_logging_la_LIBADD	=	@LTLIBOBJS@

INCLUDES 			= \
	-I@top_srcdir@/src/include/logging 

