noinst_LTLIBRARIES		=	libfindik_logging.la

if HAVE_LOG4CXX
libfindik_logging_la_SOURCES	= \
	log_initializer.cpp
else
libfindik_logging_la_SOURCES    =      	logger.cpp logger_ptr.cpp
endif

libfindik_logging_la_LIBADD	=	@LTLIBOBJS@

INCLUDES 			= \
	-I@top_srcdir@/src/include/logging 

