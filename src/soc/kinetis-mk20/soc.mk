include ${_libdir}/src/soc/kinetis/soc.mk

SRCS.force-kinetis-mk20=	startup.c
ifdef LOADER
SRCS.force-mchck+=	flashconfig_k20.c
endif
SRCS-kinetis-mk20.dir=	soc/kinetis-mk20
SRCS.libs+=	kinetis-mk20
