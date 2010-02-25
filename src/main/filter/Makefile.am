noinst_LTLIBRARIES		=	libfindik_filter.la

libfindik_filter_la_SOURCES	= \
	filter_group.cpp \
	filter_reason.cpp

libfindik_filter_la_LIBADD	=	@LTLIBOBJS@

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

