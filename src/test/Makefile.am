#SUBDIRS = filter io parser service

noinst_PROGRAMS			=	findik_test

findik_test_SOURCES		=	findik_test.cpp

findik_test_LDADD		= \
	../main/libfindik.la 

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
if HAVE_LDAP
LIBS += -lcrypt
LIBS += -lldap
endif
if HAVE_SSL
LIBS += -lcrypto
LIBS += -lssl
endif

