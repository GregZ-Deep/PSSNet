# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

cmake_minimum_required(VERSION 3.1...3.15)
project( Polygon_mesh_processing_Examples )

# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()  

endif()

# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()


find_package( OpenMesh QUIET )

if ( OpenMesh_FOUND )
include( UseOpenMesh )
else()
  message(STATUS "Examples that use OpenMesh will not be compiled.")
endif()

# include for local directory

# include for local package
find_package(Eigen3 3.2.0) #(requires 3.2.0 or greater)


# Creating entries for all .cpp/.C files with "main" routine
# ##########################################################

create_single_source_cgal_program( "hausdorff_distance_remeshing_example.cpp")

if (EIGEN3_FOUND)
  # Executables that require Eigen 3.2
  include( ${EIGEN3_USE_FILE} )

  create_single_source_cgal_program( "hole_filling_example.cpp" )
  create_single_source_cgal_program( "hole_filling_example_SM.cpp" )
  create_single_source_cgal_program( "refine_fair_example.cpp")
  create_single_source_cgal_program( "shape_smoothing_example.cpp")

endif(EIGEN3_FOUND)

create_single_source_cgal_program( "self_intersections_example.cpp" )
create_single_source_cgal_program( "stitch_borders_example.cpp" )
create_single_source_cgal_program( "compute_normals_example_Polyhedron.cpp"  CXX_FEATURES cxx_range_for )
create_single_source_cgal_program( "compute_normals_example.cpp" CXX_FEATURES cxx_range_for cxx_auto_type )
create_single_source_cgal_program( "point_inside_example.cpp")
create_single_source_cgal_program( "triangulate_faces_example.cpp")
create_single_source_cgal_program( "connected_components_example.cpp")
create_single_source_cgal_program( "face_filtered_graph_example.cpp")
create_single_source_cgal_program( "orient_polygon_soup_example.cpp")
create_single_source_cgal_program( "triangulate_polyline_example.cpp")
create_single_source_cgal_program( "mesh_slicer_example.cpp")
#create_single_source_cgal_program( "remove_degeneracies_example.cpp")
create_single_source_cgal_program( "isotropic_remeshing_example.cpp")
create_single_source_cgal_program( "isotropic_remeshing_of_patch_example.cpp")
create_single_source_cgal_program( "surface_mesh_intersection.cpp")
create_single_source_cgal_program( "corefinement_SM.cpp")
create_single_source_cgal_program( "corefinement_consecutive_bool_op.cpp" )
create_single_source_cgal_program( "corefinement_difference_remeshed.cpp" )
create_single_source_cgal_program( "corefinement_mesh_union.cpp" )
create_single_source_cgal_program( "corefinement_mesh_union_and_intersection.cpp" )
create_single_source_cgal_program( "corefinement_mesh_union_with_attributes.cpp" )
create_single_source_cgal_program( "corefinement_polyhedron_union.cpp" )
create_single_source_cgal_program( "random_perturbation_SM_example.cpp" )
create_single_source_cgal_program( "corefinement_LCC.cpp")
create_single_source_cgal_program( "hole_filling_example_LCC.cpp" )
create_single_source_cgal_program( "detect_features_example.cpp" )
create_single_source_cgal_program( "manifoldness_repair_example.cpp" )
create_single_source_cgal_program( "repair_polygon_soup_example.cpp" )
create_single_source_cgal_program( "mesh_smoothing_example.cpp")

if(OpenMesh_FOUND)
create_single_source_cgal_program( "compute_normals_example_OM.cpp" )
target_link_libraries( compute_normals_example_OM PRIVATE ${OPENMESH_LIBRARIES} )

create_single_source_cgal_program( "hole_filling_example_OM.cpp" )
target_link_libraries( hole_filling_example_OM PRIVATE ${OPENMESH_LIBRARIES} )

create_single_source_cgal_program( "point_inside_example_OM.cpp")
target_link_libraries( point_inside_example_OM PRIVATE ${OPENMESH_LIBRARIES} )

create_single_source_cgal_program( "stitch_borders_example_OM.cpp" )
target_link_libraries( stitch_borders_example_OM PRIVATE ${OPENMESH_LIBRARIES} )

#create_single_source_cgal_program( "remove_degeneracies_example_OM.cpp")
#target_link_libraries( remove_degeneracies_example_OM PRIVATE ${OPENMESH_LIBRARIES} )

create_single_source_cgal_program( "triangulate_faces_example_OM.cpp")
target_link_libraries( triangulate_faces_example_OM PRIVATE ${OPENMESH_LIBRARIES} )
endif(OpenMesh_FOUND)

find_package( TBB )
if( TBB_FOUND )
  CGAL_target_use_TBB(hausdorff_distance_remeshing_example)
else()
  message( STATUS "NOTICE: Intel TBB was not found. Sequential code will be used." )
endif()

find_package(Ceres QUIET)
if(TARGET ceres)
  target_compile_definitions( mesh_smoothing_example PRIVATE CGAL_PMP_USE_CERES_SOLVER )
  target_link_libraries( mesh_smoothing_example PRIVATE ceres )
endif(TARGET ceres)

