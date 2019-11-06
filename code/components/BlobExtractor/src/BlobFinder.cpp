#include "BlobFinder.hpp"

void BlobFinder::FindBlob(cv::Mat image, std::vector<Blob> &blobs)
{
    blobs.clear();
    regions.create(image.rows, image.cols, CV_32S);
    
    std::vector<DepthPoint> points;

    cv::cvtColor(image, image, cv::COLOR_RGB2GRAY);

    for(int y = 0; y < image.rows; y++)
    {
        uchar *imageRow = image.ptr<uchar>(y);
        int *regionsRow = regions.ptr<int32_t>(y);

        for(int x = 0; x < image.cols; x++)
        {
            // uchar currentDepth = image.at<uchar>(cv::Point(x, y));
            // if(currentDepth != BACKGROUND && currentDepth < DEPTH_THRESHOLD)
            // {
            //     points.push_back(DepthPoint(currentDepth, x, y));
            // }

            // Add viable points to vector of points
            if((BACKGROUND != imageRow[x]) && (DEPTH_THRESHOLD > imageRow[x]))
            {
                points.push_back(DepthPoint(imageRow[x], x, y));
            }

            // Initialize regions
            regionsRow[x] = REGION_UNDEFINED;
        }
    }
    // Sort all viable points to get the highest point first
    std::sort(points.begin(), points.end());


    int REGION_COUNTER = REGION_INITIAL;
    for(size_t i = 0; i < points.size(); i++)
    {
        // If Current point is already defined (belongs to some flooded region), ignore it
        if(REGION_UNDEFINED != regions.at<int32_t>(points[i].m_y, points[i].m_x))
        {
            continue;
        }

        Blob outputBlob = FloodRegion(image, points[i], REGION_COUNTER);
        ++REGION_COUNTER;

        if((REGION_UNDEFINED != outputBlob.m_regionId) && (outputBlob.m_area > Blob::MIN_AREA) && (outputBlob.m_area < Blob::MAX_AREA))
        {
            blobs.push_back(outputBlob);
        }
    }

    // if(points.size() > 0)
    // {
    //     Blob retBlob;
    //     retBlob.m_center = points[0];
    //     blobs.push_back(retBlob);
    // }

}

Blob FloodRegion(cv::Mat &image, DepthPoint &startingPoint, int regionId)
{
    // Max accepted degrees between 2 values
    float baseAngle = 30;

    bool ignoreRegion = false;
    std::vector<DepthPoint> regionPoints;
    float depthSum = 0;

    std::deque<DepthPoint> frontier;
    frontier.push_back(startingPoint);

    while(!frontier.empty())
    {
        DepthPoint currentPoint = frontier.front();
        frontier.pop_front();

        // Calculate max angle from all angles towards neighbours (5 px difference?)
        float currentAngle = calculateMaxAngleTowardsNeighbrous();

        if(currentAngle)
    }
}

