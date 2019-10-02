
#ifndef DEPTH_POINT_H
#define DEPTH_POINT_H

#include <stdint.h>

#include "opencv2/opencv.hpp"

struct DepthPoint
{
    uint8_t m_depth;
    uint16_t m_x;
    uint16_t m_y;

    DepthPoint()
    {
    }

    DepthPoint(uint8_t depth, uint16_t x, uint16_t y)
    {
        m_depth = depth;
        m_x = x;
        m_y = y;
    }

    bool operator<(const DepthPoint& another) const
    {
        return m_depth < another.m_depth;
    }
};

#endif //DEPTH_POINT_H
