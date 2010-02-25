findikconfdir = $(sysconfdir)/findik
findikconfssldir = $(findikconfdir)/ssl

findikconf_DATA = \
	findik.cfg \
	findik_log.conf \
	forbidden_page.htm \
	ssl_trust.htm

findikconfssl_DATA = \
	ssl/ca.pem

DISTCLEANFILES = \
	$(findikconf_DATA) \
	$(findikconfssl_DATA)

EXTRA_DIST = \
	$(findikconf_DATA) \
	$(findikconfssl_DATA)
	
