noinst_LTLIBRARIES		=	libfindik_http_auth_util.la

libfindik_http_auth_util_la_SOURCES =

if HAVE_ADKERB
libfindik_http_auth_util_la_SOURCES	+= \
	accept_sec_context.c \
	asn1_NegTokenInit.c \
	der_get.c \
	init_sec_context.c \
	asn1_ContextFlags.c \
	asn1_NegTokenTarg.c  \
	der_length.c \
	asn1_err.et \
	decapsulate.c \
	der_put.c \
	timegm.c \
	asn1_MechType.c \
	der_copy.c \
	encapsulate.c \
	asn1_MechTypeList.c \
	der_free.c \
	external.c 
endif	

libfindik_http_auth_util_la_LIBADD	=	@LTLIBOBJS@

INCLUDES 			= \
	-I@top_srcdir@/src/include/protocols/http/auth_util 

