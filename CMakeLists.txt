cmake_minimum_required(VERSION 2.8.4)

include(ExternalProject)

#Options. Turn on with 'cmake -Dvarname=ON'.
#option(varname text defaultvalue)
option(test 　"Build all tests." OFF) #Makes boolean 'test' available.
option(quick "Quick build without check external projects." OFF)

set(PROJECT_NAME cg_test)
set(Suffix ".a")
set(Pthread "-pthread")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

project(${PROJECT_NAME})

add_executable(cg_test src/main.cpp)


#file(WRITE gtest.patch "")
# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

if(test)
	#add_executable(lib/gtest-1.7.0)
	if(quick)
		set(source_dir ${CMAKE_BINARY_DIR}/ThirdParty/src/googletest)
		set(binary_dir ${CMAKE_BINARY_DIR}/ThirdParty/src/googletest-build)
	else()
		ExternalProject_Add(
			googletest
			SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
			SVN_REVISION -r 668
			TIMEOUT 10
			PATCH_COMMAND ""
			#PATCH_COMMAND svn patch ${CMAKE_SOURCE_DIR}/gtest.patch ${CMAKE_BINARY_DIR}/ThirdParty/src/googletest
			# Force separate output paths for debug and release builds to allow easy
			# identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
			CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
				-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
				-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
				-Dgtest_force_shared_crt=ON
		    # Disable install step
		    INSTALL_COMMAND ""
		    # Wrap download, configure and build steps in a script to log output
		    LOG_DOWNLOAD ON
		    LOG_CONFIGURE ON
		    LOG_BUILD ON
		)

		# Specify include dir
		ExternalProject_Get_Property(googletest source_dir)
		ExternalProject_Get_Property(googletest binary_dir)
	endif()

	include_directories(${source_dir}/include)
	# Add test executable target
	add_executable(RunAllTests ${PROJECT_SOURCE_DIR}/src/run_all_tests.cpp)

	# Create dependency of MainTest on googletest
	add_dependencies(RunAllTests googletest)

	# Specify MainTest's link libraries
	target_link_libraries(
		RunAllTests
		debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${Suffix}
		optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${Suffix}
		${Pthread}
	)
endif()

