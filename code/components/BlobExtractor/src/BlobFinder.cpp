#include "BlobFinder.hpp"

void BlobFinder::FindBlob(cv::Mat image, std::vector<Blob> &blobs)
{
    // given image must already be cut off with a mask and only focus on body

    std::vector<DepthPoint> points;

    cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);

    for(int y = 0; y < image.rows; y++)
    {
        for(int x = 0; x < image.cols; x++)
        {
            uchar currentDepth = image.at<uchar>(cv::Point(x, y));
            if(currentDepth != BACKGROUND && currentDepth < DEPTH_THRESHOLD)
            {
                points.push_back(DepthPoint(currentDepth, x, y));
            }
        }
    }

    std::sort(points.begin(), points.end());

    if(points.size() > 0)
    {
        Blob retBlob;
        retBlob.m_center = points[0];
        printf("Size of points for blob: %d\n", points.size());
        blobs.push_back(retBlob);
    }

}
