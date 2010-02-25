#SUBDIRS = filter io parser service

noinst_PROGRAMS			=	findik_test

findik_test_SOURCES		=	findik_test.cpp

findik_test_LDADD		= \
	../main/libfindik.la 

LIBS = \
	-lboost_thread \
	-lboost_system \
	-lpthread \
	-lz 

if HAVE_LDAP
        LIBS += -lcrypt
        LIBS += -lldap
endif
if HAVE_SSL
        LIBS += -lcrypto
        LIBS += -lssl
endif

