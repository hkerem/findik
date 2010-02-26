SUBDIRS = authenticator config filter logging io parser persistency protocols service util

lib_LTLIBRARIES		=	libfindik.la

libfindik_la_SOURCES	=
libfindik_la_LDFLAGS	=	-no-undefined -version-info 0:0:0
libfindik_la_LIBADD	= \
	authenticator/libfindik_authenticator.la \
	config/libfindik_config.la \
	filter/libfindik_filter.la \
	logging/libfindik_logging.la \
	io/libfindik_io.la \
	parser/libfindik_parser.la \
	persistency/libfindik_persistency.la \
	protocols/http/libfindik_http.la \
	protocols/http/auth_util/libfindik_http_auth_util.la \
	service/libfindik_service.la \
	util/libfindik_util.la

bin_PROGRAMS		=	findik
findik_SOURCES		=	findik.cpp
findik_LDADD		=	libfindik.la

LIBS = \
	-lpthread \
	-lz 

if USE_BOOST_MT
LIBS += -lboost_thread-mt
LIBS += -lboost_system-mt
else
LIBS += -lboost_thread
LIBS += -lboost_system
endif
if HAVE_MAGIC 
LIBS += -lmagic
endif
if HAVE_PCRE
LIBS += -lpcrecpp
endif
if HAVE_ADKERB
LIBS += -lgssapi_krb5
endif
if HAVE_LDAP
LIBS += -lcrypt
LIBS += -lldap
endif
if HAVE_MYSQL
LIBS += -lmysqlpp
endif
if HAVE_SSL
LIBS += -lcrypto
LIBS += -lssl
endif
if HAVE_LIBCONFIG
LIBS += -lconfig++
endif
if HAVE_BOOST_PO
LIBS += -lboost_program_options
endif
if HAVE_LOG4CXX
LIBS += -llog4cxx 
endif

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

