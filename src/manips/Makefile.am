## Process this file with automake to generate Makefile.in.
## 19991229 mortene.

RegularSources = \
	SoClipPlaneManip.cpp \
	SoDirectionalLightManip.cpp \
	SoPointLightManip.cpp \
	SoSpotLightManip.cpp \
	SoTransformManip.cpp \
	SoCenterballManip.cpp \
	SoHandleBoxManip.cpp \
	SoJackManip.cpp \
	SoTabBoxManip.cpp \
	SoTrackballManip.cpp \
	SoTransformBoxManip.cpp \
	SoTransformerManip.cpp \
	commoncode.cpp
LinkHackSources = \
	all-manips-cpp.cpp
PublicHeaders =
PrivateHeaders =
ObsoleteHeaders =

##$ BEGIN TEMPLATE Make-Common(manips, manips)
# **************************************************************************
# This template depends on the following variables having been set up before
# its invokation.
#
# PublicHeaders    - header files that should be installed
# PrivateHeaders   - source files that should be distributed, but not
#                    installed - including source-included source files.
# RegularSources   - source files compiled in this directory
# LinkHackSources  - alternative source files used on -enable-compact compilation
# ObsoletedHeaders - header files that should be removed upon installation

# Everything below is autogenerated from a macro, so don't modify
# within the Makefile.am itself. 19991201 mortene.

DISTCLEANFILES = \
	ii_files/*.ii

if HACKING_COMPACT_BUILD
BuildSources = $(LinkHackSources)
ExtraSources = $(RegularSources)
else
BuildSources = $(RegularSources)
ExtraSources = $(LinkHackSources)
endif

if BUILD_WITH_MSVC
noinst_LIBRARIES = manips.lst
else
if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libmanips@SUFFIX@LINKHACK.la
else
noinst_LTLIBRARIES = libmanips.la
endif
endif

# Remove any linklibraries detected by configure, as they are not
# needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

DEFAULT_INCLUDES = -I$(top_builddir)/include -I$(top_srcdir)/include $(ExtraIncludeDirs) -I$(top_builddir)/src -I$(top_srcdir)/src

# header installation
if MAC_FRAMEWORK
libmanipsincdir = $(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Headers/manips
# hack to change libdir value when installing Mac OS X compiler framework
lib@nop@dir = $(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Libraries
else
libmanipsincdir = $(includedir)/Inventor/manips
endif
libmanipsinc_HEADERS = $(PublicHeaders)

libmanips_la_SOURCES = $(BuildSources)
EXTRA_libmanips_la_SOURCES = \
	$(PublicHeaders) $(PrivateHeaders) $(ExtraSources)

libmanips@SUFFIX@LINKHACK_la_SOURCES = $(BuildSources)
EXTRA_libmanips@SUFFIX@LINKHACK_la_SOURCES = \
	$(PublicHeaders) $(PrivateHeaders) $(ExtraSources)

manips_lst_SOURCES = $(BuildSources)
EXTRA_manips_lst_SOURCES = \
	$(PublicHeaders) $(PrivateHeaders) $(ExtraSources)

# this rule causes a make warning, so we remove it when it is not in use
manips.lst: Makefile $(manips_lst_OBJECTS)
	@echo "Linking manips.lst..."; \
	rm -f $@; touch $@; \
	for i in $(manips_lst_OBJECTS); do echo $$i >>$@; done

install-libmanipsincHEADERS: $(libmanipsinc_HEADERS)
	@$(NORMAL_INSTALL)
	@if test x"$(libmanipsinc_HEADERS)" = x""; then :; else \
	  echo " $(mkinstalldirs) $(DESTDIR)$(libmanipsincdir)"; \
	  $(mkinstalldirs) $(DESTDIR)$(libmanipsincdir); \
	fi
	@list='$(libmanipsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(libmanipsincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst" || exit 1; \
	  fi \
	done
	@list='$(ObsoletedHeaders)'; for f in $$list; do \
	  file="$(DESTDIR)$(libmanipsincdir)/$$f"; \
	  if test -x "$$file"; then \
	    echo " rm -f $$file"; \
	    rm -f "$$file"; \
	  fi \
	done; \
	:

if MACOSX
# When using --enable-hacking, the user tends to manually replace some 
# of the installed libraries by symlinks back to the build directory. 
# But Mac OS X's install program refuses to overwrite symlinks => we 
# have to explicitly remove existing symlinks before installation.
install-exec-local:
	@for file in $(DESTDIR)$(lib@nop@dir)/libmanips@SUFFIX@LINKHACK*.dylib; do \
	  if test -L "$$file"; then \
	    rm -f "$$file"; \
	  fi \
	done; 
else 
install-exec-local:
endif

install-symlinks:
	@echo "Creating symlink to libmanips@SUFFIX@LINKHACK*@LIBEXT@*"; \
        for dir in $(SUBDIRS) ; do \
          (cd $$dir; $(MAKE) install-symlinks;) \
        done ; \
        here=`pwd`; \
	(cd $(DESTDIR)$(lib@nop@dir); \
	for file in libmanips@SUFFIX@LINKHACK*@LIBEXT@*; do \
	  ln -sf "$$here/.libs/$$file" "$$file"; \
	done; \
        cd -; )
##$ END TEMPLATE Make-Common
