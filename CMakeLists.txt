cmake_minimum_required(VERSION 4.2.1)

project(Graphs)

add_executable(Main)

target_sources(Main
	PRIVATE 
	Graphs/MinDegree/main.cpp
)

target_link_libraries(Main
    PRIVATE
	mmio
	MinDegree
)

add_library(mmio)
target_sources(mmio
	PRIVATE
	    mtx_read/mmio.cpp
	PUBLIC
	    FILE_SET HEADERS
            BASE_DIRS mtx_read
	    FILES
	        mtx_read/mmio.h
	        mtx_read/matrix.h
)

add_library(MinDegree)
target_sources(MinDegree
	PRIVATE
	    Graphs/MinDegree/MinDegree2.cpp
	PUBLIC
	    FILE_SET HEADERS
            BASE_DIRS Graphs/MinDegree
	    FILES
	        Graphs/MinDegree/MinDegree2.h
)
