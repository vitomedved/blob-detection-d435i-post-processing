
#ifndef BLOB_HPP
#define BLOB_HPP

#include "DepthPoint.hpp"

struct Blob
{
    cv::Rect m_boundingBox;
    DepthPoint m_center;

    Blob()
    {
    }
};

#endif //BLOB_HPP
