SUBDIRS                         =       auth_util
noinst_LTLIBRARIES		=	libfindik_http.la

libfindik_http_la_SOURCES	= \
	content_filter_group.cpp \
	data.cpp \
	domain_filter.cpp \
	btk_filter.cpp \
	btk_filter_group.cpp \
	filetype_filter_group.cpp \
	http_connect_filter.cpp \
	http_filter_logger.cpp \
	http_filter_reason_factory.cpp \
	http_filter_result_generator.cpp \
	http_session_manager.cpp \
	https_authenticator.cpp \
	request.cpp \
	request_parser.cpp \
	response.cpp \
	response_parser.cpp \
	url_filter.cpp 

if HAVE_MAGIC
        libfindik_http_la_SOURCES +=  content_mime_filter.cpp
endif
if HAVE_CLAMD
        libfindik_http_la_SOURCES += clamd_av_filter.cpp 
endif
if HAVE_FILEEXT
        libfindik_http_la_SOURCES += file_ext_filter.cpp
endif
if HAVE_ADKERB
        libfindik_http_la_SOURCES += ad_kerb_negotiator.cpp 
endif
if HAVE_LDAP
        libfindik_http_la_SOURCES += ldap_basic_authentication.cpp
endif
if HAVE_PCRE
        libfindik_http_la_SOURCES += domain_re_filter.cpp url_re_filter.cpp html_content_filter.cpp
endif


libfindik_http_la_LIBADD	=	@LTLIBOBJS@

INCLUDES 			= \
	-I@top_srcdir@/src/include/authenticator \
	-I@top_srcdir@/src/include/config \
	-I@top_srcdir@/src/include/filter \
	-I@top_srcdir@/src/include/io \
	-I@top_srcdir@/src/include/logging \
	-I@top_srcdir@/src/include/parser \
	-I@top_srcdir@/src/include/persistency \
	-I@top_srcdir@/src/include/protocols/http \
	-I@top_srcdir@/src/include/protocols/http/auth_util \
	-I@top_srcdir@/src/include/service \
	-I@top_srcdir@/src/include/util 
if HAVE_MYSQL
        INCLUDES += -I/usr/include/mysql
endif
