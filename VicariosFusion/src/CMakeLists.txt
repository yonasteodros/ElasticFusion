cmake_minimum_required(VERSION 2.6.0)

project(VicariosFusion)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

if(WIN32)
  macro(CANONIFY_BOOL var)
    if(${var})
      set(${var} TRUE)
    else()
      set(${var} FALSE)
    endif()
  endmacro()
endif()

if(WIN32)
  find_package(JPEG REQUIRED)
endif()

find_package(LAPACK REQUIRED)
find_package(BLAS REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Pangolin 0.1 REQUIRED)
find_package(CUDA REQUIRED)
find_package(efusion REQUIRED)
find_package(SuiteSparse REQUIRED)
find_package(OpenCV)
find_package(RealSense2 QUIET)
find_package(cwipc_util REQUIRED )
find_package(cwipc_codec REQUIRED )
find_package(PCL 1.8 REQUIRED COMPONENTS common io octree)


add_definitions(${PCL_DEFINITIONS})


message(STATUS "librealsense found: ${REALSENSE_FOUND}")
option(WITH_REALSENSE "Build with Intel RealSense2 support?" ${REALSENSE2_FOUND})

if(WITH_REALSENSE)
    include_directories(${REALSENSE2_INCLUDE_DIR})
    add_definitions(-DWITH_REALSENSE)
    set(EXTRA_LIBS ${EXTRA_LIBS} ${REALSENSE2_LIBRARY})
endif()


if(WIN32)
  include_directories(${JPEG_INCLUDE_DIR})
endif()

option(WITH_ZED "Build with ZED support?" ${REALSENSE2_FOUND})


if(WITH_ZED)
  find_package(ZED REQUIRED)
  find_package(CUDA ${ZED_CUDA_VERSION} EXACT REQUIRED)
  include_directories(${ZED_INCLUDE_DIRS})
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
  add_definitions(-DWITH_ZED)
  set(EXTRA_LIBS ${EXTRA_LIBS} ${ZED_LIBRARY})
endif()




link_directories(${ZED_LIBRARY_DIR})
link_directories(${CUDA_LIBRARY_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
link_directories(${cwipc_util_LIBRARY_DIRS})
link_directories(${cwipc_util_LIBRARY_DIRS})

include_directories(${ZLIB_INCLUDE_DIR})
include_directories(${EIGEN_INCLUDE_DIRS})
include_directories(${Pangolin_INCLUDE_DIRS})
include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${EFUSION_INCLUDE_DIR})
include_directories(${OPENNI2_INCLUDE_DIR})
include_directories(${PCL_INCLUDE_DIRS})
include_directories(${cwipc_codec_INCLUDE_DIRS})

add_definitions(${cwipc_util_PYTHON_DIRS})


file(GLOB srcs *.cpp)
file(GLOB tools_srcs Tools/*.cpp)

if(WIN32)
  file(GLOB hdrs *.h)
  file(GLOB tools_hdrs Tools/*.h)
endif()

if(WIN32)
  add_definitions(-Dlinux=0)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  add_definitions(-DNOMINMAX)
  set(ADDITIONAL_CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(EXTRA_WINDOWS_LIBS ${EXTRA_WINDOWS_LIBS} ws2_32 ${JPEG_LIBRARY})
else()
  add_definitions(-Dlinux=1)
endif()

set(CMAKE_CXX_FLAGS ${ADDITIONAL_CMAKE_CXX_FLAGS} "-O3 -Wall -std=c++11")

add_executable(VicariosFusion
               ${srcs}
               ${tools_srcs}
               ${hdrs}
               ${tools_hdrs}
)

target_link_libraries(VicariosFusion
					  ${EXTRA_WINDOWS_LIBS}
                      ${ZLIB_LIBRARY}
                      ${Pangolin_LIBRARIES}
                      ${CUDA_LIBRARIES}
                      ${EXTRA_LIBS}
                      ${EFUSION_LIBRARY}
                      ${OPENNI2_LIBRARY}
                      ${SUITESPARSE_LIBRARIES}
                      ${BLAS_LIBRARIES}
                      ${ZED_LIBRARIES}                     
                      ${LAPACK_LIBRARIES}
                      ${OpenCV_LIBRARIES}
                      ${CUDA_CUDA_LIBRARY}
                      ${CUDA_CUDART_LIBRARY}
                      ${CUDA_NPP_LIBRARIES_ZED}
                      ${PCL_LIBRARIES}
                      ${cwipc_util_LIBRARIES}
                      ${cwipc_codec_LIBRARIES}

)


INSTALL(TARGETS VicariosFusion
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

