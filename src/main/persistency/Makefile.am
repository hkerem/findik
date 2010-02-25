noinst_LTLIBRARIES		=	libfindik_persistency.la

libfindik_persistency_la_SOURCES = mysqldbmanager.cpp 
#if HAVE_MYSQL
#libfindik_persistency_la_SOURCES += mysqldbmanager.cpp 
#else
#libfindik_persistency_la_SOURCES += simple_dbmanager.cpp 
#endif

libfindik_persistency_la_LIBADD	=	@LTLIBOBJS@

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
