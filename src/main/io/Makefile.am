noinst_LTLIBRARIES		=	libfindik_io.la

libfindik_io_la_SOURCES		= \
	abstract_data.cpp \
	connection.cpp \
	plain_connection.cpp \
	server.cpp \
	session.cpp \
	ssl_connection.cpp

#if HAVE_SSL
#	libfindik_io_la_SOURCES += ssl_connection.cpp
#endif

libfindik_io_la_LIBADD		=	@LTLIBOBJS@

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

