cmake_minimum_required(VERSION 3.5...3.27)

if (POLICY CMP0048)
  # cmake warns if loaded from a min-3.0-required parent dir, so silence the warning:
  cmake_policy(SET CMP0048 NEW)
endif()

project (tbb CXX)

include(CheckCXXCompilerFlag)
include(CheckCXXSourceRuns)

if(POLICY CMP0058)
  cmake_policy(SET CMP0058 NEW)
endif()

if(POLICY CMP0068)
  cmake_policy(SET CMP0068 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

if(NOT TBB_INSTALL_RUNTIME_DIR)
  set(TBB_INSTALL_RUNTIME_DIR bin)
endif()
if(NOT TBB_INSTALL_LIBRARY_DIR)
  set(TBB_INSTALL_LIBRARY_DIR lib)
endif()
if(NOT TBB_INSTALL_ARCHIVE_DIR)
  set(TBB_INSTALL_ARCHIVE_DIR lib)
endif()
if(NOT TBB_INSTALL_INCLUDE_DIR)
  set(TBB_INSTALL_INCLUDE_DIR include)
endif()

include_directories(include src src/rml/include ${CMAKE_CURRENT_BINARY_DIR})

option(TBB_BUILD_SHARED          "Build TBB shared library" ON)
option(TBB_BUILD_STATIC          "Build TBB static library" ON)
option(TBB_BUILD_TBBMALLOC       "Build TBB malloc library" ON)
option(TBB_BUILD_TBBMALLOC_PROXY "Build TBB malloc proxy library" ON)
option(TBB_BUILD_TESTS           "Build TBB tests and enable testing infrastructure" ON)
option(TBB_CI_BUILD              "Is this a continuous integration build?" OFF)
option(TBB_NO_DATE               "Do not save the configure date in the version string" OFF)

if(APPLE)
  set(CMAKE_MACOSX_RPATH ON)
endif()

file(GLOB tbb_src "${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/old/*.cpp")
list(APPEND tbb_src ${CMAKE_CURRENT_SOURCE_DIR}/src/rml/client/rml_tbb.cpp)
file(GLOB to_remove "${CMAKE_CURRENT_SOURCE_DIR}/src/old/test*.cpp")
list(REMOVE_ITEM tbb_src ${to_remove})

set(tbbmalloc_static_src
  src/tbbmalloc/backend.cpp
  src/tbbmalloc/large_objects.cpp
  src/tbbmalloc/backref.cpp
  src/tbbmalloc/tbbmalloc.cpp
  src/tbbmalloc/frontend.cpp
  src/tbb/itt_notify.cpp)

set(tbbmalloc_src ${tbbmalloc_static_src})

set(tbbmalloc_proxy_src
  src/tbbmalloc/proxy.cpp
  src/tbbmalloc/tbb_function_replacement.cpp)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(i386|x86_64)")
  if (NOT APPLE AND NOT MINGW)
    add_definitions(-DDO_ITT_NOTIFY)
  endif()
endif()

if (APPLE)
  # Disable annoying "has no symbols" warnings
  set(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
  set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
  set(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
  set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
endif()

macro(CHECK_CXX_COMPILER_AND_LINKER_FLAGS _RESULT _CXX_FLAGS _LINKER_FLAGS)
  set(CMAKE_REQUIRED_FLAGS ${_CXX_FLAGS})
  set(CMAKE_REQUIRED_LIBRARIES ${_LINKER_FLAGS})
  set(CMAKE_REQUIRED_QUIET TRUE)
  check_cxx_source_runs("#include <iostream>\nint main(int argc, char **argv) { std::cout << \"test\"; return 0; }" ${_RESULT})
  set(CMAKE_REQUIRED_FLAGS "")
  set(CMAKE_REQUIRED_LIBRARIES "")
endmacro()

# Prefer libc++ in conjunction with Clang
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if (CMAKE_CXX_FLAGS MATCHES "-stdlib=libc\\+\\+")
    message(STATUS "TBB: using libc++.")
  else()
    CHECK_CXX_COMPILER_AND_LINKER_FLAGS(HAS_LIBCPP "-stdlib=libc++" "-stdlib=libc++")
    if (HAS_LIBCPP)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -D_LIBCPP_VERSION")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
      set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
      message(STATUS "TBB: using libc++.")
    else()
      message(STATUS "TBB: NOT using libc++.")
    endif()
  endif()
endif()

if (UNIX)
  add_definitions (-DUSE_PTHREAD)

  check_cxx_compiler_flag ("-std=c++11" SUPPORTS_STDCXX11)
  if (SUPPORTS_STDCXX11)
    set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
  endif ()

  check_cxx_compiler_flag ("-mrtm -Werror" SUPPORTS_MRTM)
  if (SUPPORTS_MRTM)
    set (CMAKE_CXX_FLAGS "-mrtm ${CMAKE_CXX_FLAGS}")
  endif ()

elseif(WIN32)
  if (MSVC)
    enable_language(ASM_MASM)
    set(CMAKE_CXX_FLAGS "/GS- /Zc:wchar_t /Zc:forScope /DUSE_WINTHREAD ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "/D_CRT_SECURE_NO_DEPRECATE /D_WIN32_WINNT=0x0600 ${CMAKE_CXX_FLAGS}")
    check_cxx_compiler_flag ("/volatile:iso" SUPPORTS_VOLATILE_FLAG)
    if (SUPPORTS_VOLATILE_FLAG)
      set(CMAKE_CXX_FLAGS "/volatile:iso ${CMAKE_CXX_FLAGS}")
    endif ()
    set(CMAKE_CXX_FLAGS "/wd4267 /wd4800 /wd4146 /wd4244 /wd4577 /wd4018 ${CMAKE_CXX_FLAGS}")
    if (NOT CMAKE_SIZEOF_VOID_P)
       message(FATAL_ERROR "'CMAKE_SIZEOF_VOID_P' is undefined. Please delete your build directory and rerun CMake again!")
    endif()

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
      list(APPEND tbb_src src/tbb/intel64-masm/atomic_support.asm
        src/tbb/intel64-masm/itsx.asm src/tbb/intel64-masm/intel64_misc.asm)
      list(APPEND tbbmalloc_src src/tbb/intel64-masm/atomic_support.asm)
      set(CMAKE_ASM_MASM_FLAGS "/DEM64T=1 ${CMAKE_ASM_MASM_FLAGS}")
    else()
      list(APPEND tbb_src src/tbb/ia32-masm/atomic_support.asm
        src/tbb/ia32-masm/itsx.asm src/tbb/ia32-masm/lock_byte.asm)
      # Enable SAFESEH feature for assembly (x86 builds only).
      set(CMAKE_ASM_MASM_FLAGS "/safeseh ${CMAKE_ASM_MASM_FLAGS}")
    endif()
  elseif (MINGW)
    add_definitions(-DUSE_WINTHREAD)
    add_definitions(-D_WIN32_WINNT=0x0502)
    set(CMAKE_CXX_FLAGS "-mthreads ${CMAKE_CXX_FLAGS}")
  endif ()
endif()

if (MSVC)
  set(ENABLE_RTTI "/EHsc /GR ")
  set(DISABLE_RTTI "/EHs- /GR- ")
elseif (UNIX)
  set(ENABLE_RTTI "-frtti -fexceptions ")
  set(DISABLE_RTTI "-fno-rtti -fno-exceptions ")
endif ()

##--------
#   - Added TBB_USE_GLIBCXX_VERSION macro to specify the version of GNU
#     libstdc++ when it cannot be properly recognized, e.g. when used
#     with Clang on Linux* OS. Inspired by a contribution from David A.
if (NOT TBB_USE_GLIBCXX_VERSION AND UNIX AND NOT APPLE)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using Clang
    string(REPLACE "." "0" TBB_USE_GLIBCXX_VERSION ${CMAKE_CXX_COMPILER_VERSION})
  endif()
endif()

if (TBB_USE_GLIBCXX_VERSION)
   add_definitions(-DTBB_USE_GLIBCXX_VERSION=${TBB_USE_GLIBCXX_VERSION})
endif()

##-------

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   check_cxx_compiler_flag ("-flifetime-dse=1" SUPPORTS_FLIFETIME)
   if (SUPPORTS_FLIFETIME)
     add_definitions(-flifetime-dse=1)
   endif()
endif()

# Linker export definitions
if (APPLE)
  set (ARCH_PREFIX "mac")
elseif(WIN32)
  set (ARCH_PREFIX "win")
else()
  set (ARCH_PREFIX "lin")
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH_PREFIX "${ARCH_PREFIX}64")
else()
  set(ARCH_PREFIX "${ARCH_PREFIX}32")
endif()

if (MINGW)
  set (ARCH_PREFIX "${ARCH_PREFIX}-gcc")
  # there's no win32-gcc-tbb-export.def, use lin32-tbb-export.def
  execute_process (COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/lin32-tbb-export.def ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/win32-gcc-tbb-export.def)
endif()

if (MSVC)
  add_custom_command(OUTPUT tbb.def
    COMMAND ${CMAKE_CXX_COMPILER} /TC /EP ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/${ARCH_PREFIX}-tbb-export.def  -I ${CMAKE_CURRENT_SOURCE_DIR}/include > tbb.def
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/${ARCH_PREFIX}-tbb-export.def
    COMMENT "Preprocessing tbb.def"
  )

  add_custom_command(OUTPUT tbbmalloc.def
    COMMAND ${CMAKE_CXX_COMPILER} /TC /EP ${CMAKE_CURRENT_SOURCE_DIR}/src/tbbmalloc/${ARCH_PREFIX}-tbbmalloc-export.def  -I ${CMAKE_CURRENT_SOURCE_DIR}/include >   tbbmalloc.def
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/src/tbbmalloc/${ARCH_PREFIX}-tbbmalloc-export.def
    COMMENT "Preprocessing tbbmalloc.def"
  )
else()
  add_custom_command(OUTPUT tbb.def
    COMMAND ${CMAKE_CXX_COMPILER} -xc++ -E ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/${ARCH_PREFIX}-tbb-export.def  -I ${CMAKE_CURRENT_SOURCE_DIR}/include -o tbb.def
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/${ARCH_PREFIX}-tbb-export.def
    COMMENT "Preprocessing tbb.def"
  )

  add_custom_command(OUTPUT tbbmalloc.def
    COMMAND ${CMAKE_CXX_COMPILER} -xc++ -E ${CMAKE_CURRENT_SOURCE_DIR}/src/tbbmalloc/${ARCH_PREFIX}-tbbmalloc-export.def  -I ${CMAKE_CURRENT_SOURCE_DIR}/include -o   tbbmalloc.def
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/src/tbbmalloc/${ARCH_PREFIX}-tbbmalloc-export.def
    COMMENT "Preprocessing tbbmalloc.def"
  )
endif()

add_custom_target(tbb_def_files DEPENDS tbb.def tbbmalloc.def)

# TBB library
if (TBB_BUILD_STATIC)
  add_library(tbb_static STATIC ${tbb_src})
  target_include_directories(tbb_static INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
  set_property(TARGET tbb_static APPEND PROPERTY COMPILE_DEFINITIONS "__TBB_BUILD=1")
  set_property(TARGET tbb_static APPEND PROPERTY COMPILE_DEFINITIONS "__TBB_DYNAMIC_LOAD_ENABLED=0")
  set_property(TARGET tbb_static APPEND PROPERTY COMPILE_DEFINITIONS "__TBB_SOURCE_DIRECTLY_INCLUDED=1")
  set_property(TARGET tbb_static APPEND_STRING PROPERTY COMPILE_FLAGS ${ENABLE_RTTI})
  install(TARGETS tbb_static ARCHIVE DESTINATION ${TBB_INSTALL_ARCHIVE_DIR})
  if (MSVC)
    target_compile_definitions(tbb_static PUBLIC __TBB_NO_IMPLICIT_LINKAGE=1)
  endif()

  if (UNIX AND NOT APPLE)
    target_link_libraries(tbb_static PUBLIC pthread dl)
  endif()
endif()

if (TBB_BUILD_SHARED)
  add_library(tbb SHARED ${tbb_src})
  target_include_directories(tbb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
  set_property(TARGET tbb APPEND PROPERTY COMPILE_DEFINITIONS "__TBB_BUILD=1")
  set_property(TARGET tbb APPEND_STRING PROPERTY COMPILE_FLAGS ${ENABLE_RTTI})
  add_dependencies(tbb tbb_def_files)

  if (APPLE)
    set_property(TARGET tbb APPEND PROPERTY LINK_FLAGS "-Wl,-exported_symbols_list,\"${CMAKE_CURRENT_BINARY_DIR}/tbb.def\"")
  elseif (MSVC)
    set_property(TARGET tbb APPEND PROPERTY LINK_FLAGS "/DEF:\"${CMAKE_CURRENT_BINARY_DIR}/tbb.def\"")
  else ()
    set_property(TARGET tbb APPEND PROPERTY LINK_FLAGS "-Wl,-version-script,\"${CMAKE_CURRENT_BINARY_DIR}/tbb.def\"")
  endif()

  install(TARGETS tbb
          LIBRARY DESTINATION ${TBB_INSTALL_LIBRARY_DIR}
          ARCHIVE DESTINATION ${TBB_INSTALL_ARCHIVE_DIR}
          RUNTIME DESTINATION ${TBB_INSTALL_RUNTIME_DIR})
  if (UNIX AND NOT APPLE)
    target_link_libraries(tbb PUBLIC pthread dl)
  endif()
  if (MSVC)
    target_compile_definitions(tbb PUBLIC __TBB_NO_IMPLICIT_LINKAGE=1)
  endif()
endif()


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # Quench a warning on GCC
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/governor.cpp COMPILE_FLAGS "-Wno-missing-field-initializers ")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # Quench a warning on Clang
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/itt_notify.cpp COMPILE_FLAGS "-Wno-varargs ")
elseif(MSVC)
  # Quench a warning on MSVC
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/scheduler.cpp COMPILE_FLAGS "/wd4458 ")
endif()

if(TBB_BUILD_TBBMALLOC)
  # TBB malloc library
  if (TBB_BUILD_STATIC)
    add_library(tbbmalloc_static STATIC ${tbbmalloc_static_src})
    set_property(TARGET tbbmalloc_static APPEND PROPERTY COMPILE_DEFINITIONS "__TBBMALLOC_BUILD=1")
    set_property(TARGET tbbmalloc_static APPEND PROPERTY COMPILE_DEFINITIONS "__TBB_DYNAMIC_LOAD_ENABLED=0")
    set_property(TARGET tbbmalloc_static APPEND PROPERTY COMPILE_DEFINITIONS "__TBB_SOURCE_DIRECTLY_INCLUDED=1")
    set_property(TARGET tbbmalloc_static APPEND_STRING PROPERTY COMPILE_FLAGS ${DISABLE_RTTI})
    if (MSVC)
      target_compile_definitions(tbbmalloc_static PUBLIC __TBB_NO_IMPLICIT_LINKAGE=1 __TBBMALLOC_NO_IMPLICIT_LINKAGE=1)
    endif()
    install(TARGETS tbbmalloc_static ARCHIVE DESTINATION ${TBB_INSTALL_ARCHIVE_DIR})
  endif()

  if (TBB_BUILD_SHARED)
    add_library(tbbmalloc SHARED ${tbbmalloc_src})
    set_property(TARGET tbbmalloc APPEND PROPERTY COMPILE_DEFINITIONS "__TBBMALLOC_BUILD=1")
    set_property(TARGET tbbmalloc APPEND_STRING PROPERTY COMPILE_FLAGS ${DISABLE_RTTI})
    add_dependencies(tbbmalloc tbb_def_files)
    if (APPLE)
      set_property(TARGET tbbmalloc APPEND PROPERTY LINK_FLAGS "-Wl,-exported_symbols_list,\"${CMAKE_CURRENT_BINARY_DIR}/tbbmalloc.def\"")
    elseif (MSVC)
      set_property(TARGET tbbmalloc APPEND PROPERTY LINK_FLAGS "/DEF:\"${CMAKE_CURRENT_BINARY_DIR}/tbbmalloc.def\"")
    else ()
      set_property(TARGET tbbmalloc APPEND PROPERTY LINK_FLAGS "-Wl,-version-script,\"${CMAKE_CURRENT_BINARY_DIR}/tbbmalloc.def\"")
    endif()
    if (MSVC)
      target_compile_definitions(tbbmalloc PUBLIC __TBB_NO_IMPLICIT_LINKAGE=1 __TBBMALLOC_NO_IMPLICIT_LINKAGE=1)
    endif()
    install(TARGETS tbbmalloc
            LIBRARY DESTINATION ${TBB_INSTALL_LIBRARY_DIR}
            ARCHIVE DESTINATION ${TBB_INSTALL_ARCHIVE_DIR}
            RUNTIME DESTINATION ${TBB_INSTALL_RUNTIME_DIR})
    if (UNIX AND NOT APPLE)
      target_link_libraries(tbbmalloc PUBLIC pthread dl)
    endif()
  endif()
endif()

if(TBB_BUILD_TBBMALLOC_PROXY)
  # TBB malloc proxy library
  if (TBB_BUILD_STATIC)
    add_library(tbbmalloc_proxy_static STATIC ${tbbmalloc_proxy_src})
    set_property(TARGET tbbmalloc_proxy_static APPEND PROPERTY COMPILE_DEFINITIONS "__TBBMALLOC_BUILD=1")
    set_property(TARGET tbbmalloc_proxy_static APPEND PROPERTY COMPILE_DEFINITIONS "__TBB_DYNAMIC_LOAD_ENABLED=0")
    set_property(TARGET tbbmalloc_proxy_static APPEND PROPERTY COMPILE_DEFINITIONS "__TBB_SOURCE_DIRECTLY_INCLUDED=1")
    set_property(TARGET tbbmalloc_proxy_static APPEND_STRING PROPERTY COMPILE_FLAGS ${DISABLE_RTTI})
    install(TARGETS tbbmalloc_proxy_static ARCHIVE DESTINATION ${TBB_INSTALL_ARCHIVE_DIR})
  endif()

  if (TBB_BUILD_SHARED)
    add_library(tbbmalloc_proxy SHARED ${tbbmalloc_proxy_src})
    set_property(TARGET tbbmalloc_proxy APPEND PROPERTY COMPILE_DEFINITIONS "__TBBMALLOC_BUILD=1")
    set_property(TARGET tbbmalloc_proxy APPEND_STRING PROPERTY COMPILE_FLAGS ${DISABLE_RTTI})
    target_link_libraries(tbbmalloc_proxy PUBLIC tbbmalloc)
    install(TARGETS tbbmalloc_proxy
            LIBRARY DESTINATION ${TBB_INSTALL_LIBRARY_DIR}
            ARCHIVE DESTINATION ${TBB_INSTALL_ARCHIVE_DIR}
            RUNTIME DESTINATION ${TBB_INSTALL_RUNTIME_DIR})
    if (UNIX AND NOT APPLE)
      target_link_libraries(tbbmalloc_proxy PUBLIC pthread dl)
    endif()
  endif()
endif()

install(DIRECTORY include/tbb DESTINATION ${TBB_INSTALL_INCLUDE_DIR})

# version_string.ver
if (UNIX AND NOT TBB_NO_DATE)
  execute_process (COMMAND date "+%a, %d %b %Y %H:%M:%S %z"
                   OUTPUT_VARIABLE _configure_date
                   OUTPUT_STRIP_TRAILING_WHITESPACE)
elseif (WIN32 AND NOT TBB_NO_DATE)
  execute_process (COMMAND cmd " /C date /T"
                   OUTPUT_VARIABLE _configure_date
                   OUTPUT_STRIP_TRAILING_WHITESPACE)
else ()
  set (_configure_date "Unknown")
endif()
set (TBB_CONFIG_DATE "${_configure_date}" CACHE STRING "First time that TBB was configured")
set (_configure_date "${TBB_CONFIG_DATE}")
include_directories (${CMAKE_BINARY_DIR})
configure_file (version_string.ver.in version_string.ver @ONLY)

if (TBB_BUILD_TESTS)
  enable_language (C)
  enable_testing ()

  find_library (LIBRT_LIBRARIES rt)
  find_library (LIDL_LIBRARIES dl)
  find_package (Threads)
  if (NOT APPLE)
    find_package (OpenMP)
  endif()

  macro (tbb_add_test testname)
    set (full_testname tbb_test_${testname})
    add_executable (${full_testname} src/test/test_${testname}.cpp)
    if (TBB_BUILD_SHARED)
      target_link_libraries (${full_testname} PRIVATE tbb tbbmalloc)
      target_compile_definitions (${full_testname} PRIVATE __TBB_LIB_NAME=tbb)
    else ()
      target_link_libraries (${full_testname} PRIVATE tbb_static tbbmalloc_static)
      target_compile_definitions (${full_testname} PRIVATE __TBB_LIB_NAME=tbb_static)
    endif ()
    if (LIBRT_LIBRARIES)
      target_link_libraries (${full_testname} PRIVATE ${LIBRT_LIBRARIES})
    endif ()
    if (LIDL_LIBRARIES)
      target_link_libraries (${full_testname} PRIVATE ${LIDL_LIBRARIES})
    endif ()
    if (Threads_FOUND)
      target_link_libraries (${full_testname} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
    endif ()
    if (OPENMP_FOUND AND "${testname}" MATCHES "openmp")
      set_target_properties (${full_testname} PROPERTIES COMPILE_FLAGS "${OpenMP_CXX_FLAGS}")
      set_target_properties (${full_testname} PROPERTIES LINK_FLAGS "${OpenMP_CXX_FLAGS}")
    endif()
    if (MINGW)
      target_link_libraries (${full_testname} PRIVATE psapi)
    endif ()
    add_test (NAME ${full_testname} COMMAND ${full_testname})
  endmacro ()

  tbb_add_test (aggregator)
  tbb_add_test (aligned_space)
  tbb_add_test (assembly)
  if (NOT WIN32)
    tbb_add_test (async_msg) # msvc64/debug timeouts
  endif()
  tbb_add_test (async_node)
  # tbb_add_test (atomic) # msvc64/debug timeouts: Compile-time initialization fails for static tbb::atomic variables
  tbb_add_test (blocked_range2d)
  tbb_add_test (blocked_range3d)
  tbb_add_test (blocked_range)
  tbb_add_test (broadcast_node)
  tbb_add_test (buffer_node)
  tbb_add_test (cache_aligned_allocator)
  if (NOT WIN32)
    tbb_add_test (cache_aligned_allocator_STL)
  endif()
  tbb_add_test (cilk_dynamic_load)
  tbb_add_test (cilk_interop)
  tbb_add_test (combinable)
  tbb_add_test (composite_node)
  tbb_add_test (concurrent_hash_map)
  tbb_add_test (concurrent_lru_cache)
  # tbb_add_test (concurrent_monitor) # too long
  # tbb_add_test (concurrent_priority_queue)
  if (NOT WIN32)
    tbb_add_test (concurrent_queue) # msvc64/debug timeouts
  endif()
  # tbb_add_test (concurrent_queue_whitebox)
  tbb_add_test (concurrent_unordered_map)
  # tbb_add_test (concurrent_unordered_set)
  tbb_add_test (concurrent_vector)
  tbb_add_test (continue_node)
  tbb_add_test (critical_section)
  tbb_add_test (dynamic_link)
  # tbb_add_test (eh_algorithms)
  tbb_add_test (eh_flow_graph)
  # tbb_add_test (eh_tasks)
  tbb_add_test (enumerable_thread_specific)
  tbb_add_test (examples_common_utility)
  # tbb_add_test (fast_random)
  tbb_add_test (flow_graph)
  tbb_add_test (flow_graph_whitebox)
  # tbb_add_test (fp) # mingw: harness_fp.h:66, assertion !checkConsistency || (ctl.mxcsr & SSE_RND_MODE_MASK) >> 3 == (ctl.x87cw & FE_RND_MODE_MASK): failed
  # tbb_add_test (function_node) # mingw:random timeout
  # tbb_add_test (global_control)
  # tbb_add_test (global_control_whitebox)
  tbb_add_test (halt)
  tbb_add_test (handle_perror)
  # tbb_add_test (hw_concurrency)
  tbb_add_test (indexer_node)
  tbb_add_test (inits_loop)
  tbb_add_test (intrusive_list)
  tbb_add_test (ittnotify)
  # tbb_add_test (join_node) #msvc/64: fatal error C1128: number of sections exceeded object file format limit: compile with /bigob
  tbb_add_test (lambda)
  tbb_add_test (limiter_node)
  # tbb_add_test (malloc_atexit)
  tbb_add_test (malloc_compliance)
  tbb_add_test (malloc_init_shutdown)
  # tbb_add_test (malloc_lib_unload)
  # tbb_add_test (malloc_overload)
  tbb_add_test (malloc_pools)
  tbb_add_test (malloc_regression)
  # tbb_add_test (malloc_used_by_lib)
  # tbb_add_test (malloc_whitebox)
  tbb_add_test (model_plugin)
  # tbb_add_test (multifunction_node) # too long
  tbb_add_test (mutex)
  tbb_add_test (mutex_native_threads)
  # tbb_add_test (opencl_node)
  if (OPENMP_FOUND)
    tbb_add_test (openmp)
  endif ()
  tbb_add_test (overwrite_node)
  # tbb_add_test (parallel_do)
  if (NOT TBB_CI_BUILD)
    # This seems to fail on CI platforms (AppVeyor/Travis), perhaps because the VM exposes just 1 core?
    tbb_add_test (parallel_for)
  endif()
  tbb_add_test (parallel_for_each)
  tbb_add_test (parallel_for_vectorization)
  tbb_add_test (parallel_invoke)
  tbb_add_test (parallel_pipeline)
  tbb_add_test (parallel_reduce)
  tbb_add_test (parallel_scan)
  tbb_add_test (parallel_sort)
  tbb_add_test (parallel_while)
  # tbb_add_test (partitioner_whitebox) # too long
  tbb_add_test (pipeline)
  # tbb_add_test (pipeline_with_tbf) # takes forever on appveyor
  tbb_add_test (priority_queue_node)
  tbb_add_test (queue_node)
  tbb_add_test (reader_writer_lock)
  # tbb_add_test (runtime_loader) # LINK : fatal error LNK1104: cannot open file 'tbbproxy.lib' [C:\projects\tbb\test_runtime_loader.vcxproj]
  tbb_add_test (rwm_upgrade_downgrade)
  # tbb_add_test (ScalableAllocator)
  if (NOT WIN32)
    tbb_add_test (ScalableAllocator_STL)
  endif()
  tbb_add_test (semaphore)
  # tbb_add_test (sequencer_node) # msvc: timeout
  tbb_add_test (source_node)
  tbb_add_test (split_node)
  tbb_add_test (static_assert)
  tbb_add_test (std_thread)
  tbb_add_test (tagged_msg)
  # tbb_add_test (task_arena) # LINK : fatal error LNK1104: cannot open file '__TBB_LIB_NAME.lib' [C:\projects\tbb\test_task_arena.vcxproj]
  # tbb_add_test (task_assertions)
  tbb_add_test (task_auto_init)
  tbb_add_test (task)
  # tbb_add_test (task_enqueue) # too long
  tbb_add_test (task_group)
  # tbb_add_test (task_leaks)
  # tbb_add_test (task_priority)
  # tbb_add_test (task_scheduler_init) # msvc: test_task_scheduler_init.cpp:68, assertion !test_mandatory_parallelism || Harness::CanReachConcurrencyLevel(threads): failed
  tbb_add_test (task_scheduler_observer)
  tbb_add_test (task_steal_limit)
  tbb_add_test (tbb_condition_variable)
  tbb_add_test (tbb_fork)
  # tbb_add_test (tbb_header)
  tbb_add_test (tbb_thread)
  # tbb_add_test (tbb_version)
  tbb_add_test (tick_count)
  tbb_add_test (tuple)
  tbb_add_test (write_once_node)
  tbb_add_test (yield)
endif ()
