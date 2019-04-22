cmake_minimum_required(VERSION 3.10)
project(DelaunayIncremental)

set(CMAKE_CXX_STANDARD 11)

add_executable(DelaunayIncremental main.cpp src/utils.h src/HalfEdge.cpp src/HalfEdge.h src/Point.cpp src/Point.h src/Vector3D.cpp src/Vector3D.h src/PointRepository.cpp src/PointRepository.h src/PointDataGenerator.cpp src/PointDataGenerator.h src/TestCaseGenerator.cpp src/TestCaseGenerator.h src/avl.cpp src/avl.h src/Matrix.cpp src/Matrix.h src/triangulation.cpp src/triangulation.h src/vertex_avl_tree.cpp src/vertex_avl_tree.h src/TriangulationTestCaseGenerator.cpp src/TriangulationTestCaseGenerator.h src/DrawTriangulation.cpp src/DrawTriangulation.h src/HalfEdgeTestCaseGenerator.cpp src/HalfEdgeTestCaseGenerator.h src/Polygon.cpp src/Polygon.h)

include_directories(GL/)

target_link_libraries(DelaunayIncremental -lglut -lGL -lGLU)

target_link_libraries(${PROJECT_NAME} )