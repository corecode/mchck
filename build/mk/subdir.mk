all: ${SUBDIRS}

${SUBDIRS}:
	@${MAKE} -C "$@" ${MAKECMDGOALS}

.PHONY: ${SUBDIRS}
