
set (COMPONENT_NAME "BlobExtractor.out")

project(${COMPONENT_NAME})

# Output dir should be set, TODO: add error or some kind of indication if it's not set
if( OUTPUT_DIR )
    message("OUTPUT_DIR: ${OUTPUT_DIR}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}/bin)
endif()

set (OPENCV_DIR ../../third-party/opencv)

# Sources
set (LOCAL_SOURCE_DIR   src/BlobExtractor.cpp
                        src/BackgroundSubtraction.cpp
)

# Includes
set (LOCAL_INCLUDE_DIR include)
set (OPENCV_INCLUDE_DIR ${OPENCV_DIR}/include/opencv4)

# Lib dirs
set (OPENCV_LIB_DIR ${OPENCV_DIR}/lib)

set (INCLUDES   ${LOCAL_INCLUDE_DIR}
                ${OPENCV_INCLUDE_DIR}
)

set (SOURCES ${LOCAL_SOURCE_DIR})

# Find required libraries
find_library(LIB_OPENCV_CORE
        NAMES opencv_core
        HINTS "${OPENCV_LIB_DIR}")

find_library(LIB_OPENCV_IMGCODECS
        NAMES opencv_imgcodecs
        HINTS "${OPENCV_LIB_DIR}")

find_library(LIB_OPENCV_VIDEOIO
        NAMES opencv_videoio
        HINTS "${OPENCV_LIB_DIR}")

find_library(LIB_OPENCV_HIGHGUI
        NAMES opencv_highgui opencv_video
        HINTS "${OPENCV_LIB_DIR}")

        find_library(LIB_OPENCV_CALIB3D
        NAMES opencv_calib3d
        HINTS "${OPENCV_LIB_DIR}")

        find_library(LIB_OPENCV_DNN
        NAMES opencv_dnn
        HINTS "${OPENCV_LIB_DIR}")

        find_library(LIB_OPENCV_FEATURES2D
        NAMES opencv_features2d
        HINTS "${OPENCV_LIB_DIR}")

        find_library(LIB_OPENCV_FLANN
        NAMES opencv_flann
        HINTS "${OPENCV_LIB_DIR}")

        find_library(LIB_OPENCV_IMGPROC
        NAMES opencv_imgproc
        HINTS "${OPENCV_LIB_DIR}")

        find_library(LIB_OPENCV_ML
        NAMES opencv_ml
        HINTS "${OPENCV_LIB_DIR}")

        find_library(LIB_OPENCV_OBJDETECT
        NAMES opencv_objdetect
        HINTS "${OPENCV_LIB_DIR}")

        find_library(LIB_OPENCV_PHOTO
        NAMES opencv_photo
        HINTS "${OPENCV_LIB_DIR}")

        find_library(LIB_OPENCV_STITCHING
        NAMES opencv_stitching
        HINTS "${OPENCV_LIB_DIR}")


include_directories(${INCLUDES})

add_executable(${COMPONENT_NAME} ${SOURCES})

target_link_libraries(${COMPONENT_NAME} 
        ${LIB_OPENCV_CORE} 
        ${LIB_OPENCV_IMGCODECS} 
        ${LIB_OPENCV_VIDEOIO} 
        ${LIB_OPENCV_HIGHGUI}
        ${LIB_OPENCV_IMGPROC}

        ${LIB_OPENCV_VIDEO})
