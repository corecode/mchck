.DEFAULT: subdirs

subdirs: ${SUBDIRS}

define mksubdir
$1:
	@$${MAKE} -C "$$@" $${MAKECMDGOALS}
endef

$(foreach d,${SUBDIRS},$(eval $(call mksubdir,$d)))

.PHONY: ${SUBDIRS}
