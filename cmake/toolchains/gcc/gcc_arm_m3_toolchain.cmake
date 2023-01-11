cmake_minimum_required(VERSION 3.23)

add_compile_options(
  "-mcpu=cortex-m3"
  # "-mfloat-abi=soft" #not needed, as this core only has software floating
  "-mthumb" # Generate code in thumb state (or -marm for arm state)
)

add_link_options(
  "-mcpu=cortex-m3"
  # "-mfloat-abi=soft" #not needed, as this core only has software floating
  "-mthumb" # Generate code in thumb state (or -marm for arm state)
)

include("${CMAKE_CURRENT_LIST_DIR}/gcc_arm_toolchain.cmake")