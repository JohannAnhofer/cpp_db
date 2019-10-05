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

# Look for the necessary header and library
if (WIN32)
	find_path(Firebird3_INCLUDE_DIR NAMES ibase.h HINTS "$ENV{ProgramW6432}/Firebird/Firebird_3_0" PATH_SUFFIXES "include" ENV FIREBIRD_HOME)
	find_library(Firebird3_LIBRARY NAMES fbclient_ms.lib HINTS "$ENV{ProgramW6432}/Firebird/Firebird_3_0" PATH_SUFFIXES "lib" ENV_FIREBIRD_HOME)
	get_filename_component(FB_ROOT_PATH "${Firebird3_INCLUDE_DIR}" DIRECTORY)
	execute_process(COMMAND "${FB_ROOT_PATH}/gbak.exe" -z OUTPUT_VARIABLE FB_VERSION ERROR_VARIABLE FB_ERROR)
	STRING(REGEX MATCH "([0-9]+[.])+[0-9]+" Firebird3_VERSION "${FB_VERSION}")
elseif(APPLE)
    find_path(Firebird3_INCLUDE_DIR NAMES ibase.h HINTS "/Library/Frameworks/Firebird.framework" PATH_SUFFIXES "Headers" ENV FIREBIRD_HOME)
    find_library(Firebird3_LIBRARY NAMES libfbclient.dylib HINTS "/Library/Frameworks/Firebird.framework/Libraries" PATH_SUFFIXES "Libraries" ENV_FIREBIRD_HOME)
    get_filename_component(FB_ROOT_PATH "${Firebird3_INCLUDE_DIR}" DIRECTORY)
    execute_process(COMMAND "${FB_ROOT_PATH}/Resources/bin/gbak" -z OUTPUT_VARIABLE FB_VERSION ERROR_VARIABLE FB_ERROR)
    STRING(REGEX MATCH "([0-9]+[.])+[0-9]+" Firebird3_VERSION "${FB_VERSION}")
else()
	find_path(Firebird3_INCLUDE_DIR NAMES ibase.h HINTS "/usr/lib/x86_64-linux-gnu" PATH_SUFFIXES "include" ENV FIREBIRD_HOME)
	find_library(Firebird3_LIBRARY NAMES libfbclient.so HINTS "/usr/lib/x86_64-linux-gnu" ENV_FIREBIRD_HOME)
	execute_process(COMMAND "gbak" -z OUTPUT_VARIABLE FB_VERSION ERROR_VARIABLE FB_ERROR)
	STRING(REGEX MATCH "([0-9]+[.])+[0-9]+" Firebird3_VERSION "${FB_VERSION}")
	if (NOT Firebird3_VERSION)
		# Extract version information from the library filename
		if(Firebird3_LIBRARY)	  
			STRING(REGEX REPLACE "^[^.]*[.][sS][oO][.]" "" Firebird3_VERSION ${Firebird3_LIBRARY})
		endif()
	endif()
endif()
mark_as_advanced(Firebird3_INCLUDE_DIR)
mark_as_advanced(Firebird3_LIBRARY)

find_package(PackageHandleStandardArgs REQUIRED)

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
