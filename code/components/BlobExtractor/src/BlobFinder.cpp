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
    // TODO: increment by some x
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
            printf("Added blob %d with area: %d\n", outputBlob.m_regionId, outputBlob.m_area);
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

Blob BlobFinder::FloodRegion(cv::Mat &image, DepthPoint &startingPoint, int regionId)
{
    float baseAngle = 70;

    bool ignoreRegion = false;
    std::vector<DepthPoint> regionPoints;
    float depthSum = 0;

    std::deque<DepthPoint> frontier;
    frontier.push_back(startingPoint);

    while(!frontier.empty())
    {
        DepthPoint currentPoint = frontier.front();
        frontier.pop_front();

        int region = regions.at<int32_t>(currentPoint.m_y, currentPoint.m_x);

        if(REGION_UNDEFINED == region)
        {
            regions.at<int32_t>(currentPoint.m_y, currentPoint.m_x) = regionId;
            // moved down somewhere: regionPoints.push_back(currentPoint);
            // moved down somwhere: depthSum += currentPoint.m_depth;

            std::vector<DepthPoint> neighbours;
            GetNeighbours(image, currentPoint, neighbours);

            for(int i = 0; i < neighbours.size(); i++)
            {
                if(regions.at<int32_t>(neighbours[i].m_y, neighbours[i].m_x) != REGION_UNDEFINED)
                {
                    continue;
                }
                // If angle with neighbour is good, add it to frontier, else discard it
                float currentAngle = CalculateAngle(currentPoint, neighbours[i]);

                if(currentAngle > baseAngle)
                {
                    // All points from current to neighbour should be set accordingly
                    for(int i = 0; i < INCREMENT; i++)
                    {
                        regions.at<int32_t>(currentPoint.m_y + i, currentPoint.m_x + i) = regionId;
                    }
                    depthSum += currentPoint.m_depth;
                    regionPoints.push_back(currentPoint);
                    frontier.push_back(neighbours[i]);
                }
            }
        }
        else if(region == regionId)
        {
            // Already belongs to this region, ignore it
        }
        else
        {
            ignoreRegion = true;
        }
    }

    Blob outputBlob;
    outputBlob.m_regionId = REGION_UNDEFINED;

    if(0 == depthSum || 100 > regionPoints.size())
    {
        return outputBlob;
    }

    if(!ignoreRegion)
    {
        int length = regionPoints.size();

        long xSum = 0;
        long ySum = 0;

        for(int i = 0; i < regionPoints.size(); i++)
        {
            DepthPoint regionPoint = regionPoints[i];

            xSum += regionPoint.m_depth * regionPoint.m_x;
            ySum += regionPoint.m_depth * regionPoint.m_y;
        }

        int x = 0;
        int y = 0;

        if(0 != depthSum)
        {
            x = xSum / depthSum;
            y = ySum / depthSum;
        }

        outputBlob.m_regionId = regionId;
        outputBlob.m_area = length;
        outputBlob.m_center = cv::Point(x, y);
    }
}



void BlobFinder::GetNeighbours(cv::Mat &image, DepthPoint point, std::vector<DepthPoint> &neighbours)
{
    for(int i = 0; i < 4; i++)
    {
        int x = point.m_x + indexes[i][0];
        int y = point.m_y + indexes[i][1];

        if(0 > x || x >= image.cols) {
            continue;
        }

        if(0 > y || y >= image.rows)
        {
            continue;
        }

        uchar depth = image.at<uchar>(y, x);

        if((BACKGROUND == depth) || (DEPTH_THRESHOLD < depth))
        {
            continue;
        }

        neighbours.push_back(DepthPoint(depth, x, y));
    }
}

float BlobFinder::CalculateAngle(DepthPoint first, DepthPoint second)
{
    float depthDifference = std::abs(first.m_depth - second.m_depth);
    if(0 == depthDifference)
    {
        return 90;
    }

    float rad = atan(INCREMENT / depthDifference);
    float degrees = rad * 180 / 3.14;
    return degrees;
}

