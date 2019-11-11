
#ifndef BLOB_FINDER_HPP
#define BLOB_FINDER_HPP

#include "Blob.hpp"

#include <math.h>

class BlobFinder
{
public:
    void FindBlob(cv::Mat image, std::vector<Blob> &blobs);

private:
    const int8_t REGION_UNDEFINED = -1;
    const uint8_t REGION_INITIAL = 0;
    const uint8_t BACKGROUND = 0;
    const uint8_t INCREMENT = 1;
    const uint8_t DEPTH_THRESHOLD = 150; // 150 - 255 are white colors - far away 0-150 black colors - closer
    
    const int indexes[4][2] = { {0, -INCREMENT}, {-INCREMENT, 0}, {INCREMENT, 0}, {0, INCREMENT} };

    cv::Mat regions;

    Blob FloodRegion(cv::Mat &image, DepthPoint &startingPoint, int regionId);
    void GetNeighbours(cv::Mat &image, DepthPoint point, std::vector<DepthPoint> &neighbours);
    float CalculateAngle(DepthPoint first, DepthPoint second);
};

#endif // BLOB_FINDER_HPP
