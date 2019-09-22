#include <stdio.h>

#include "opencv2/opencv.hpp"

int main()
{
    printf("OpenCV: %s", cv::getBuildInformation().c_str());
    return 0;
}

