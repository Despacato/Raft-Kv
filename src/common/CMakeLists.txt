#获取目录下源文件的绝对路径
get_filename_component(COMMON_DIR "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

# 将该目录下所有源文件添加到 COMMON_LIST 变量中
#aux_source_directory(${COMMON_DIR} COMMON_LIST)
file(GLOB_RECURSE COMMON_LIST ${COMMON_DIR}/*.cpp)
# 将common文件夹下的所有源文件添加到src_common变量中,并且设置src_common为CACHE INTERNAL类型。
set(src_common ${COMMON_LIST} CACHE INTERNAL "Description of the variable")
#COMMON_LIST是一个变量，用于存储common文件夹下的所有源文件，这样在其他文件夹的CMakeLists.txt中就可以通过set(src_common ${COMMON_LIST} CACHE

