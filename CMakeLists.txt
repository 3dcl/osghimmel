#
#     _  _ _ |_ . _ _  _ _  _ |
#    (_)_\(_|| ||| | || | |(/_|
#          _|  
#
# Copyright (c) 2011-2012, Daniel Müller <dm@g4t3.de>
# Computer Graphics Systems Group at the Hasso-Plattner-Institute, Germany
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
#   * Redistributions of source code must retain the above copyright notice, 
#     this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above copyright 
#     notice, this list of conditions and the following disclaimer in the 
#     documentation and/or other materials provided with the distribution.
#   * Neither the name of the Computer Graphics Systems Group at the 
#     Hasso-Plattner-Institute (HPI), Germany nor the names of its 
#     contributors may be used to endorse or promote products derived from 
#     this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
# POSSIBILITY OF SUCH DAMAGE.
  
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# macros

macro(SOURCE_GROUP_BY_PATH parent_path)

    foreach(filename ${ARGV})
	
        get_filename_component(path "${filename}" REALPATH)
        file(RELATIVE_PATH path ${parent_path} ${path})
        get_filename_component(path "${path}" PATH)
        string(REPLACE "/" "\\" path "${path}")

        if(${filename} MATCHES "h$|hpp$|cpp$|c$|ui$|qrc$")
            source_group("${path}" FILES ${filename})
        endif()

    endforeach()

endmacro()


# config

set(PROJECT_NAME osgHimmel)
set(PROJECT_NAME osgHimmel)

project(${PROJECT_NAME} C CXX)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "limited configs" FORCE)

set(VERSION_MAJOR "0" CACHE STRING "${PROJECT_NAME} Major Version")
set(VERSION_MINOR "0" CACHE STRING "${PROJECT_NAME} Minor Version")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)

option(OPTION_MAKE_DEMOS "Make Demos" ON)
option(OPTION_MAKE_SKYBOX "Make SkyBox - Sandbox for osgHimmel (requires Qt)" ON)
option(OPTION_MAKE_TESTS "Make Tests" ON)

# copied from osg's CMakeLists.txt
IF(UNIX AND NOT WIN32 AND NOT APPLE)
  IF(CMAKE_SIZEOF_VOID_P MATCHES "8")
      SET(LIB_POSTFIX "64" CACHE STRING "suffix for 32/64 dir placement")
      MARK_AS_ADVANCED(LIB_POSTFIX)
  ENDIF()
ENDIF()
IF(NOT DEFINED LIB_POSTFIX)
    SET(LIB_POSTFIX "")
ENDIF()


# 3rdp and resources

find_package(OpenGL REQUIRED)
find_package(OpenSceneGraph REQUIRED 
    osgViewer 
    osgUtil 
    osgDB 
    osgGA
	osgText)

string(REPLACE "include" "bin" OSG_BINARY_DIR ${OSG_INCLUDE_DIR})

set(OSG_BINARY_DIR ${OSG_BINARY_DIR} CACHE PATH "Binary directory containing OSG dlls.")
set(OSG_SOVERSION "80" CACHE STRING "OSG binaries version (osg##.dll).")


# lib/bin copy

if(MSVC)
    set(OSG_PLUGIN_DIR "${OSG_BINARY_DIR}/osgPlugins-${OPENSCENEGRAPH_VERSION}")

    if(NOT EXISTS ${OSG_PLUGIN_DIR}/)
        message(WARNING "osgPlugins dir not found, please specify OSG_PLUGIN_DIR.")
    endif()

    if(NOT EXISTS ${OSG_BINARY_DIR}/osg${OSG_SOVERSION}-osgd.dll)
        message(WARNING "OSG_SOVERSION (${OSG_SOVERSION}) seems wrong. ${OSG_BINARY_DIR}/osg${OSG_SOVERSION}-osgd.dll not found.")
    endif()
endif()

# sources

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/3rdp
    ${PROJECT_SOURCE_DIR}/include
    ${OSG_INCLUDE_DIR})


add_subdirectory("src")
if(OPTION_MAKE_TESTS)
    add_subdirectory("tests")
endif()
if(OPTION_MAKE_DEMOS OR OPTION_MAKE_SKYBOX)
	add_subdirectory("examples")
endif()


# cpack deploy
option(BUILD_DEBIAN_PACKAGE  "Creates a debian package" ON)
if(BUILD_DEBIAN_PACKAGE) 
    # set project description for packaging
    set(META_PROJECT_NAME        "osghimmel")
    set(META_PROJECT_DESCRIPTION "osgHimmel is a OpenSceneGraph (OSG) extending lib that allows visualization of background imagery in arbitrary scenegraphs.")
    set(META_VERSION_MAJOR       ${VERSION_MAJOR})
    set(META_VERSION_MINOR       ${VERSION_MINOR})
    set(META_VERSION_PATCH       "0")
    set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
    set(META_AUTHOR_ORGANIZATION "3D Content Logistics")
    set(META_AUTHOR_DOMAIN       "https://github.com/3dcl/osghimmel.git")
    set(META_AUTHOR_MAINTAINER   "Marcel Pursche <marcel.pursche@3dcontentlogistics.com>")

    # Package information
    string(TOLOWER ${META_PROJECT_NAME}     package_name)          # Package name
    set(CPACK_PACKAGE_NAME                  "${META_PROJECT_NAME}")
    set(CPACK_PACKAGE_VENDOR                "${META_AUTHOR_ORGANIZATION}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY   "${META_PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_VERSION               "${META_VERSION}")
    set(CPACK_PACKAGE_VERSION_MAJOR         "${META_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR         "${META_VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH         "${META_VERSION_PATCH}")
    set(CPACK_PACKAGE_RELOCATABLE           OFF)

    # Debian package information
    set(CPACK_DEBIAN_PACKAGE_NAME           "${package_name}")
    set(CPACK_DEBIAN_PACKAGE_VERSION        "${CPACK_PACKAGE_VERSION}")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER     "${META_AUTHOR_MAINTAINER}")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION    "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
    set(CPACK_DEBIAN_PACKAGE_SECTION        "Development")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY       "optional")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS        "libc6 (>= 2.4), libgcc1 (>= 1:4.1), libstdc++6 (>= 4.6), openscenegraph-all (>= 3.3.0), libgl1-mesa-glx | libgl1")
    set(CPACK_DEBIAN_PACKAGE_CONFLICTS      "")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE       "${META_AUTHOR_DOMAIN}")
    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA  "")
        
    set(CPACK_GENERATOR "DEB")

    include(CPack)
else()    
    set(CPACK_GENERATOR "ZIP")
    set(CPACK_SOURCE_GENERATOR "ZIP")

    set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH 0)

    set(CPACK_INSTALL_DIRECTORIES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CPACK_IGNORE_FILES "/.pdb/;/.ilk/;/\\.svn/;/\\.hg/;/\\.git/;\\.swp$;\\.#;/#")
    set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_SOURCE_DIR})

    include(CPack)
endif()