
#ifndef BLOB_HPP
#define BLOB_HPP

#include "DepthPoint.hpp"

struct Blob
{
    int m_regionId;
    int m_area;
    DepthPoint m_center;
    cv::Rect m_boundingBox;

    // TODO: set as configurable param with constructor
    const static int MIN_AREA = 1000;
    const static int MAX_AREA = 50000;

    Blob()
    {
    }
};

#endif //BLOB_HPP
