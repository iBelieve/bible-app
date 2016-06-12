# - Try to find sword
# Once done, this will define
#
#  SWORD_FOUND - system has SWORD
#  SWORD_INCLUDE_DIRS - the SWORD include directories
#  SWORD_LIBRARIES - link these to use SWORD

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(SWORD_PKGCONF sword)

# Include dir
find_path(SWORD_INCLUDE_DIR
  NAMES swmgr.h
  PATHS ${SWORD_PKGCONF_INCLUDE_DIRS}
  PATH_SUFFIXES sword
)

# Finally the library itself
find_library(SWORD_LIBRARY
  NAMES sword
  PATHS ${SWORD_PKGCONF_LIBRARY_DIRS}
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(SWORD_PROCESS_INCLUDES SWORD_INCLUDE_DIR)
set(SWORD_PROCESS_LIBS SWORD_LIBRARY)
libfind_process(SWORD)
