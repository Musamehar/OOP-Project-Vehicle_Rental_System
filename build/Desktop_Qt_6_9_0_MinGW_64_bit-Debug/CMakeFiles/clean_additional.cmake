# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\RoyalCarRental_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\RoyalCarRental_autogen.dir\\ParseCache.txt"
  "RoyalCarRental_autogen"
  )
endif()
