# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/samy/BA2/pa/group-13/build/_deps/sheenbidi-src"
  "/home/samy/BA2/pa/group-13/build/_deps/sheenbidi-build"
  "/home/samy/BA2/pa/group-13/build/_deps/sheenbidi-subbuild/sheenbidi-populate-prefix"
  "/home/samy/BA2/pa/group-13/build/_deps/sheenbidi-subbuild/sheenbidi-populate-prefix/tmp"
  "/home/samy/BA2/pa/group-13/build/_deps/sheenbidi-subbuild/sheenbidi-populate-prefix/src/sheenbidi-populate-stamp"
  "/home/samy/BA2/pa/group-13/build/_deps/sheenbidi-subbuild/sheenbidi-populate-prefix/src"
  "/home/samy/BA2/pa/group-13/build/_deps/sheenbidi-subbuild/sheenbidi-populate-prefix/src/sheenbidi-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/samy/BA2/pa/group-13/build/_deps/sheenbidi-subbuild/sheenbidi-populate-prefix/src/sheenbidi-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/samy/BA2/pa/group-13/build/_deps/sheenbidi-subbuild/sheenbidi-populate-prefix/src/sheenbidi-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
