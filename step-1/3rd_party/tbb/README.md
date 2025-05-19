### Intel(R) Threading Building Blocks

[![Stable release](https://img.shields.io/badge/version-2019_U2-green.svg)](https://github.com/01org/tbb/releases/tag/2019_U2)
[![Apache License Version 2.0](https://img.shields.io/badge/license-Apache_2.0-green.svg)](LICENSE)
[![Build Status](https://travis-ci.org/wjakob/tbb.svg?branch=master)](https://travis-ci.org/wjakob/tbb)
[![Build status](https://ci.appveyor.com/api/projects/status/fvepmk5nxekq27r8?svg=true)](https://ci.appveyor.com/project/wjakob/tbb/branch/master)

This is git repository is currently based on TBB 2019 Update 6 and will be
updated from time to time to track the most recent release. The only
modification is the addition of a CMake-based build system.

This is convenient for other projects that use CMake and TBB because TBB can be
easily incorporated into their build process using git submodules and a simple
``add_subdirectory`` command.

Currently, the CMake-based build can create shared and static versions of
`libtbb`, `libtbbmalloc` and `libtbbmalloc_proxy` for the Intel `i386` and
`x86_64` architectures on Windows (Visual Studio, MinGW), MacOS (Clang) and
Linux (GCC & Clang). The `armv7` and `armv8` architectures are supported on
Linux (GCC & Clang). Other combinations may work but have not been tested.

See index.html for general directions and documentation regarding TBB.

See examples/index.html for runnable examples and directions.

See http://threadingbuildingblocks.org for full documentation
and software information.

Note: Intel, Thread Building Blocks, and TBB are either registered trademarks or
trademarks of Intel Corporation in the United States and/or other countries.

The CMake build contains the following additional/changed files that are not
part of the regular release: ``build/mingw_cross_toolchain.cmake``,
``build/version_string.ver.in``, ``.gitignore`` (modified), ``README.md`` (this
file), and ``Makefile.old`` (renamed from ``Makefile``).

    mkdir build
    cd build
    cmake ..
    make
    make install

` You will see .so file install in /usr/local/lib and .h file install in /usr/local/include/tbb`

    [ 15%] Built target tbb_static
    [ 16%] Built target tbb_def_files
    [ 30%] Built target tbb
    [ 33%] Built target tbbmalloc_static
    [ 35%] Built target tbbmalloc
    [ 36%] Built target tbbmalloc_proxy_static
    [ 38%] Built target tbbmalloc_proxy
    [ 39%] Built target tbb_test_aggregator
    [ 39%] Built target tbb_test_aligned_space
    [ 40%] Built target tbb_test_assembly
    [ 41%] Built target tbb_test_async_msg
    [ 42%] Built target tbb_test_async_node
    [ 43%] Built target tbb_test_blocked_range2d
    [ 44%] Built target tbb_test_blocked_range3d
    [ 44%] Built target tbb_test_blocked_range
    [ 45%] Built target tbb_test_broadcast_node
    [ 46%] Built target tbb_test_buffer_node
    [ 46%] Built target tbb_test_cache_aligned_allocator
    [ 47%] Built target tbb_test_cache_aligned_allocator_STL
    [ 48%] Built target tbb_test_cilk_dynamic_load
    [ 49%] Built target tbb_test_cilk_interop
    [ 49%] Built target tbb_test_combinable
    [ 50%] Built target tbb_test_composite_node
    [ 51%] Built target tbb_test_concurrent_hash_map
    [ 52%] Built target tbb_test_concurrent_lru_cache
    [ 53%] Built target tbb_test_concurrent_queue
    [ 53%] Built target tbb_test_concurrent_unordered_map
    [ 54%] Built target tbb_test_concurrent_vector
    [ 55%] Built target tbb_test_continue_node
    [ 56%] Built target tbb_test_critical_section
    [ 57%] Built target tbb_test_dynamic_link
    [ 57%] Built target tbb_test_eh_flow_graph
    [ 58%] Built target tbb_test_enumerable_thread_specific
    [ 59%] Built target tbb_test_examples_common_utility
    [ 60%] Built target tbb_test_flow_graph
    [ 60%] Built target tbb_test_flow_graph_whitebox
    [ 61%] Built target tbb_test_halt
    [ 62%] Built target tbb_test_handle_perror
    [ 63%] Built target tbb_test_indexer_node
    [ 64%] Built target tbb_test_inits_loop
    [ 64%] Built target tbb_test_intrusive_list
    [ 65%] Built target tbb_test_ittnotify
    [ 66%] Built target tbb_test_lambda
    [ 67%] Built target tbb_test_limiter_node
    [ 67%] Built target tbb_test_malloc_compliance
    [ 68%] Built target tbb_test_malloc_init_shutdown
    [ 69%] Built target tbb_test_malloc_pools
    [ 70%] Built target tbb_test_malloc_regression
    [ 71%] Built target tbb_test_model_plugin
    [ 71%] Built target tbb_test_mutex
    [ 72%] Built target tbb_test_mutex_native_threads
    [ 73%] Built target tbb_test_openmp
    [ 74%] Built target tbb_test_overwrite_node
    [ 74%] Built target tbb_test_parallel_for
    [ 75%] Built target tbb_test_parallel_for_each
    [ 76%] Built target tbb_test_parallel_for_vectorization
    [ 77%] Built target tbb_test_parallel_invoke
    [ 78%] Built target tbb_test_parallel_pipeline
    [ 78%] Built target tbb_test_parallel_reduce
    [ 79%] Built target tbb_test_parallel_scan
    [ 80%] Built target tbb_test_parallel_sort
    [ 81%] Built target tbb_test_parallel_while
    [ 82%] Built target tbb_test_pipeline
    [ 82%] Built target tbb_test_priority_queue_node
    [ 83%] Built target tbb_test_queue_node
    [ 84%] Built target tbb_test_reader_writer_lock
    [ 85%] Built target tbb_test_rwm_upgrade_downgrade
    [ 86%] Built target tbb_test_ScalableAllocator_STL
    [ 86%] Built target tbb_test_semaphore
    [ 87%] Built target tbb_test_source_node
    [ 88%] Built target tbb_test_split_node
    [ 89%] Built target tbb_test_static_assert
    [ 90%] Built target tbb_test_std_thread
    [ 90%] Built target tbb_test_tagged_msg
    [ 91%] Built target tbb_test_task_auto_init
    [ 92%] Built target tbb_test_task
    [ 93%] Built target tbb_test_task_group
    [ 93%] Built target tbb_test_task_scheduler_observer
    [ 94%] Built target tbb_test_task_steal_limit
    [ 95%] Built target tbb_test_tbb_condition_variable
    [ 96%] Built target tbb_test_tbb_fork
    [ 97%] Built target tbb_test_tbb_thread
    [ 97%] Built target tbb_test_tick_count
    [ 98%] Built target tbb_test_tuple
    [ 99%] Built target tbb_test_write_once_node
    [100%] Built target tbb_test_yield
    Install the project...
    -- Install configuration: "Release"
    -- Installing: /usr/local/lib/libtbb_static.a
    -- Installing: /usr/local/lib/libtbb.so
    -- Installing: /usr/local/lib/libtbbmalloc_static.a
    -- Installing: /usr/local/lib/libtbbmalloc.so
    -- Installing: /usr/local/lib/libtbbmalloc_proxy_static.a
    -- Installing: /usr/local/lib/libtbbmalloc_proxy.so
    -- Set non-toolchain portion of runtime path of "/usr/local/lib/libtbbmalloc_proxy.so" to ""
    -- Installing: /usr/local/include/tbb
    -- Installing: /usr/local/include/tbb/aggregator.h
    -- Installing: /usr/local/include/tbb/aligned_space.h
    -- Installing: /usr/local/include/tbb/atomic.h
    -- Installing: /usr/local/include/tbb/blocked_range.h
    -- Installing: /usr/local/include/tbb/blocked_range2d.h
    -- Installing: /usr/local/include/tbb/blocked_range3d.h
    -- Installing: /usr/local/include/tbb/blocked_rangeNd.h
    -- Installing: /usr/local/include/tbb/cache_aligned_allocator.h
    -- Installing: /usr/local/include/tbb/combinable.h
    -- Installing: /usr/local/include/tbb/compat
    -- Installing: /usr/local/include/tbb/compat/condition_variable
    -- Installing: /usr/local/include/tbb/compat/ppl.h
    -- Installing: /usr/local/include/tbb/compat/thread
    -- Installing: /usr/local/include/tbb/compat/tuple
    -- Installing: /usr/local/include/tbb/concurrent_hash_map.h
    -- Installing: /usr/local/include/tbb/concurrent_lru_cache.h
    -- Installing: /usr/local/include/tbb/concurrent_priority_queue.h
    -- Installing: /usr/local/include/tbb/concurrent_queue.h
    -- Installing: /usr/local/include/tbb/concurrent_unordered_map.h
    -- Installing: /usr/local/include/tbb/concurrent_unordered_set.h
    -- Installing: /usr/local/include/tbb/concurrent_vector.h
    -- Installing: /usr/local/include/tbb/critical_section.h
    -- Installing: /usr/local/include/tbb/enumerable_thread_specific.h
    -- Installing: /usr/local/include/tbb/flow_graph.h
    -- Installing: /usr/local/include/tbb/flow_graph_abstractions.h
    -- Installing: /usr/local/include/tbb/flow_graph_opencl_node.h
    -- Installing: /usr/local/include/tbb/global_control.h
    -- Installing: /usr/local/include/tbb/index.html
    -- Installing: /usr/local/include/tbb/internal
    -- Installing: /usr/local/include/tbb/internal/_aggregator_impl.h
    -- Installing: /usr/local/include/tbb/internal/_allocator_traits.h
    -- Installing: /usr/local/include/tbb/internal/_concurrent_queue_impl.h
    -- Installing: /usr/local/include/tbb/internal/_concurrent_unordered_impl.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_async_msg_impl.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_body_impl.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_cache_impl.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_impl.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_indexer_impl.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_item_buffer_impl.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_join_impl.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_node_impl.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_streaming_node.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_tagged_buffer_impl.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_trace_impl.h
    -- Installing: /usr/local/include/tbb/internal/_flow_graph_types_impl.h
    -- Installing: /usr/local/include/tbb/internal/_mutex_padding.h
    -- Installing: /usr/local/include/tbb/internal/_range_iterator.h
    -- Installing: /usr/local/include/tbb/internal/_tbb_hash_compare_impl.h
    -- Installing: /usr/local/include/tbb/internal/_tbb_strings.h
    -- Installing: /usr/local/include/tbb/internal/_tbb_trace_impl.h
    -- Installing: /usr/local/include/tbb/internal/_tbb_windef.h
    -- Installing: /usr/local/include/tbb/internal/_template_helpers.h
    -- Installing: /usr/local/include/tbb/internal/_x86_eliding_mutex_impl.h
    -- Installing: /usr/local/include/tbb/internal/_x86_rtm_rw_mutex_impl.h
    -- Installing: /usr/local/include/tbb/iterators.h
    -- Installing: /usr/local/include/tbb/machine
    -- Installing: /usr/local/include/tbb/machine/gcc_arm.h
    -- Installing: /usr/local/include/tbb/machine/gcc_generic.h
    -- Installing: /usr/local/include/tbb/machine/gcc_ia32_common.h
    -- Installing: /usr/local/include/tbb/machine/gcc_itsx.h
    -- Installing: /usr/local/include/tbb/machine/ibm_aix51.h
    -- Installing: /usr/local/include/tbb/machine/icc_generic.h
    -- Installing: /usr/local/include/tbb/machine/linux_common.h
    -- Installing: /usr/local/include/tbb/machine/linux_ia32.h
    -- Installing: /usr/local/include/tbb/machine/linux_ia64.h
    -- Installing: /usr/local/include/tbb/machine/linux_intel64.h
    -- Installing: /usr/local/include/tbb/machine/macos_common.h
    -- Installing: /usr/local/include/tbb/machine/mac_ppc.h
    -- Installing: /usr/local/include/tbb/machine/mic_common.h
    -- Installing: /usr/local/include/tbb/machine/msvc_armv7.h
    -- Installing: /usr/local/include/tbb/machine/msvc_ia32_common.h
    -- Installing: /usr/local/include/tbb/machine/sunos_sparc.h
    -- Installing: /usr/local/include/tbb/machine/windows_api.h
    -- Installing: /usr/local/include/tbb/machine/windows_ia32.h
    -- Installing: /usr/local/include/tbb/machine/windows_intel64.h
    -- Installing: /usr/local/include/tbb/memory_pool.h
    -- Installing: /usr/local/include/tbb/mutex.h
    -- Installing: /usr/local/include/tbb/null_mutex.h
    -- Installing: /usr/local/include/tbb/null_rw_mutex.h
    -- Installing: /usr/local/include/tbb/parallel_do.h
    -- Installing: /usr/local/include/tbb/parallel_for.h
    -- Installing: /usr/local/include/tbb/parallel_for_each.h
    -- Installing: /usr/local/include/tbb/parallel_invoke.h
    -- Installing: /usr/local/include/tbb/parallel_reduce.h
    -- Installing: /usr/local/include/tbb/parallel_scan.h
    -- Installing: /usr/local/include/tbb/parallel_sort.h
    -- Installing: /usr/local/include/tbb/parallel_while.h
    -- Installing: /usr/local/include/tbb/partitioner.h
    -- Installing: /usr/local/include/tbb/pipeline.h
    -- Installing: /usr/local/include/tbb/queuing_mutex.h
    -- Installing: /usr/local/include/tbb/queuing_rw_mutex.h
    -- Installing: /usr/local/include/tbb/reader_writer_lock.h
    -- Installing: /usr/local/include/tbb/recursive_mutex.h
    -- Installing: /usr/local/include/tbb/runtime_loader.h
    -- Installing: /usr/local/include/tbb/scalable_allocator.h
    -- Installing: /usr/local/include/tbb/spin_mutex.h
    -- Installing: /usr/local/include/tbb/spin_rw_mutex.h
    -- Installing: /usr/local/include/tbb/task.h
    -- Installing: /usr/local/include/tbb/task_arena.h
    -- Installing: /usr/local/include/tbb/task_group.h
    -- Installing: /usr/local/include/tbb/task_scheduler_init.h
    -- Installing: /usr/local/include/tbb/task_scheduler_observer.h
    -- Installing: /usr/local/include/tbb/tbb.h
    -- Installing: /usr/local/include/tbb/tbbmalloc_proxy.h
    -- Installing: /usr/local/include/tbb/tbb_allocator.h
    -- Installing: /usr/local/include/tbb/tbb_config.h
    -- Installing: /usr/local/include/tbb/tbb_disable_exceptions.h
    -- Installing: /usr/local/include/tbb/tbb_exception.h
    -- Installing: /usr/local/include/tbb/tbb_machine.h
    -- Installing: /usr/local/include/tbb/tbb_profiling.h
    -- Installing: /usr/local/include/tbb/tbb_stddef.h
    -- Installing: /usr/local/include/tbb/tbb_thread.h
    -- Installing: /usr/local/include/tbb/tick_count.h
