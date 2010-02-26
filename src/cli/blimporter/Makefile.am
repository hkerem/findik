noinst_HEADERS		= \
	configuration.hpp \
	dbmanager.hpp \
	importer_tool.hpp \
	mysqldbmanager.hpp

bin_PROGRAMS		= findik_import

findik_import_SOURCES	= \
			main.cpp\
			importer_tool.cpp\
			configuration.cpp\
			mysqldbmanager.cpp

LIBS = \
	-lboost_program_options \
	-llog4cxx \
	-lconfig++ \
	-lmysqlpp\
	-lboost_filesystem

if USE_BOOST_MT
LIBS += -lboost_system-mt
LIBS += -lboost_thread-mt
else
LIBS += -lboost_system
LIBS += -lboost_thread
endif

INCLUDES = \
	-I@top_srcdir@/ 

