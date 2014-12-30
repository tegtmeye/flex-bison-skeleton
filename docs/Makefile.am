html-local:
	doxygen $(srcdir)/Doxyfile

install-html-local: html
	if test -d html; then \
	find html -type d ! -empty -exec $(INSTALL) -d $(htmldir)/{} \; ;\
	fi && \
	find html ! -type d -exec $(INSTALL_DATA) {} $(htmldir)/{} \;

uninstall-local:
	\rm -rf $(htmldir)/html

clean-local:
	\rm -rf html
	\rm -rf latex

EXTRA_DIST= \
	Doxyfile \
	source_doc
