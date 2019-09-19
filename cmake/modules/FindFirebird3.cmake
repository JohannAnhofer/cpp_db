# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#[=======================================================================[.rst:
FindFirebird3
-----------

Find the Firebird libraries, v3

IMPORTED targets
^^^^^^^^^^^^^^^^

This module defines the following :prop_tgt:`IMPORTED` target:

``Firebird::Firebird3``

Result variables
^^^^^^^^^^^^^^^^

This module will set the following variables if found:

``Firebird3_INCLUDE_DIRS``
  where to find ibase.h, etc.
``Firebird3_LIBRARIES``
  the libraries to link against to use Firebird3.
``Firebird3_VERSION``
  version of the Firebird3 library found
``Firebird3_FOUND``
  TRUE if found

#]=======================================================================]

# Look for the necessary header
find_path(Firebird3_INCLUDE_DIR NAMES ibase.h)
mark_as_advanced(Firebird3_INCLUDE_DIR)

# Look for the necessary library
find_library(Firebird3_LIBRARY NAMES fbclient)
mark_as_advanced(Firebird3_LIBRARY)

# Extract version information from the header file
if(Firebird3_INCLUDE_DIR)
    file(STRINGS ${Firebird3_INCLUDE_DIR}/ibase.h _ver_line
         REGEX "^#define Firebird_VERSION  *\"[0-9]+\\.[0-9]+\\.[0-9]+\""
         LIMIT_COUNT 1)
    string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+"
           Firebird3_VERSION "${_ver_line}")
    unset(_ver_line)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/FindPackageHandleStandardArgs.cmake)
find_package_handle_standard_args(Firebird3
    REQUIRED_VARS Firebird3_INCLUDE_DIR Firebird3_LIBRARY
    VERSION_VAR Firebird3_VERSION)

# Create the imported target
if(Firebird3_FOUND)
    set(Firebird3_INCLUDE_DIRS ${Firebird3_INCLUDE_DIR})
    set(Firebird3_LIBRARIES ${Firebird3_LIBRARY})
    if(NOT TARGET Firebird::Firebird3)
        add_library(Firebird::Firebird3 UNKNOWN IMPORTED)
        set_target_properties(Firebird::Firebird3 PROPERTIES
            IMPORTED_LOCATION             "${Firebird3_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${Firebird3_INCLUDE_DIR}")
    endif()
endif()
