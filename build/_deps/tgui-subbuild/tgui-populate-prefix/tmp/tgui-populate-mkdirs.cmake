# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/mnt/c/Users/chinG/Documents/group-13/build/_deps/tgui-src")
  file(MAKE_DIRECTORY "/mnt/c/Users/chinG/Documents/group-13/build/_deps/tgui-src")
endif()
file(MAKE_DIRECTORY
  "/mnt/c/Users/chinG/Documents/group-13/build/_deps/tgui-build"
  "/mnt/c/Users/chinG/Documents/group-13/build/_deps/tgui-subbuild/tgui-populate-prefix"
  "/mnt/c/Users/chinG/Documents/group-13/build/_deps/tgui-subbuild/tgui-populate-prefix/tmp"
  "/mnt/c/Users/chinG/Documents/group-13/build/_deps/tgui-subbuild/tgui-populate-prefix/src/tgui-populate-stamp"
  "/mnt/c/Users/chinG/Documents/group-13/build/_deps/tgui-subbuild/tgui-populate-prefix/src"
  "/mnt/c/Users/chinG/Documents/group-13/build/_deps/tgui-subbuild/tgui-populate-prefix/src/tgui-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/mnt/c/Users/chinG/Documents/group-13/build/_deps/tgui-subbuild/tgui-populate-prefix/src/tgui-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/mnt/c/Users/chinG/Documents/group-13/build/_deps/tgui-subbuild/tgui-populate-prefix/src/tgui-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
