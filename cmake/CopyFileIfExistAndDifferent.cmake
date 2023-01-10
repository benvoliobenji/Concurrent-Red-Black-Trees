cmake_minimum_required(VERSION 3.19.0)

if(EXISTS ${FileToCheck})
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${FileToCheck}
      ${PathToCopy}
  )
else()
  message(WARNING "${FileToCheck} does not exist. Skipping copy")
endif()