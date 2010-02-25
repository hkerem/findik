noinst_LTLIBRARIES		=	libfindik_config.la

libfindik_config_la_SOURCES	= configuration_object.cpp configuration_initializer.cpp

#if HAVE_LIBCONFIG
#	libfindik_config_la_SOURCES += configuration_initializer.cpp
#endif

libfindik_config_la_LIBADD	=	@LTLIBOBJS@

INCLUDES 			= \
	-I@top_srcdir@/src/include/authenticator \
	-I@top_srcdir@/src/include/config \
	-I@top_srcdir@/src/include/filter \
	-I@top_srcdir@/src/include/io \
	-I@top_srcdir@/src/include/logging \
	-I@top_srcdir@/src/include/parser \
	-I@top_srcdir@/src/include/persistency \
	-I@top_srcdir@/src/include/service \
	-I@top_srcdir@/src/include/util 
if HAVE_MYSQL
        INCLUDES += -I/usr/include/mysql
endif

