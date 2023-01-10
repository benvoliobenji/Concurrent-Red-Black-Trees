cmake_minimum_required(VERSION 3.19)

# function to find MSBuild.exe, call with no argument for default install paths or provide the Path to the MSBuild location
function(find_msbuild exe_path)
  find_program(
    ${exe_path} 
    NAMES MSBuild
    HINTS 
    ${ARGV1}
    "C:/Program Files/Microsoft Visual Studio/2022/Professional"
    "C:/Program Files/Microsoft Visual Studio/2022/Community"
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional"
    "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community"
    "C:/Program Files (x86)/Microsoft Visual Studio/2017/Professional"
    "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community"
    "C:/Program Files (x86)/Microsoft Visual Studio/2015/Professional"
    "C:/Program Files (x86)/Microsoft Visual Studio/2015/Community"
    PATH_SUFFIXES 
    Bin 
    Msbuild/Current/Bin
    MSBuild/15.0/Bin
    MSBuild/14.0/Bin
  )
  
  if("${${exe_path}}" STREQUAL "${exe_path}-NOTFOUND")
    message(FATAL_ERROR "MSBuild.exe not found!")
  endif()
endfunction()