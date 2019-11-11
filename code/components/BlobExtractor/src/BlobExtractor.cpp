#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

#include "opencv2/opencv.hpp"
#include "BackgroundSubtraction.hpp"
#include "BlobFinder.hpp"

void PrintHelp();

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        PrintHelp();
        return EXIT_FAILURE;
    }

    char *colorFilePath;
    char *depthFilePath;

    int c;
    while( ( c = getopt (argc, argv, "hc:d:") ) != -1 ) 
    {
        switch(c)
        {
            case 'c':
            {
                if(optarg) 
                {
                    colorFilePath = optarg;
                }
                else
                {
                    PrintHelp();
                }
                break;
            }
            case 'd':
            {
                if(optarg)
                {
                    depthFilePath = optarg;
                }
                else
                {
                    PrintHelp();
                }
                
            }
            case 'h':
            default:
            {
                PrintHelp();
                break;
            }
        }
    }

    cv::VideoCapture colorVideo(colorFilePath);
    if(!colorVideo.isOpened())
    {
        printf("[ERROR_MAIN] Unable to open color video.\n");
        return EXIT_FAILURE;
    }

    cv::VideoCapture depthVideo(depthFilePath);
    if(!depthVideo.isOpened())
    {
        printf("[ERROR_MAIN] Unable to open depth video");
        return EXIT_FAILURE;
    }

    BackgroundSubtraction depthSubtractor(BackgroundSubtraction::DEPTH_THRESHOLD);

    BlobFinder blobFidner;

    //cv::Ptr<cv::BackgroundSubtractorMOG2> mog2Subtractor;
    //mog2Subtractor = cv::createBackgroundSubtractorMOG2(500, 16.0, false);

    //cv::Ptr<cv::BackgroundSubtractorKNN> knnSubtractor;
    //knnSubtractor = cv::createBackgroundSubtractorKNN(500, 400, false);

    cv::Mat depthFrame;
    cv::Mat depthOutput;
    cv::Mat colorFrame;

    cv::Mat finalOutput;

    cv::Mat maskedColor;

    uint8_t smallErosionDilationSize = 8;
    cv::Mat smallAreaKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * smallErosionDilationSize + 1, 2 * smallErosionDilationSize + 1), cv::Point(smallErosionDilationSize, smallErosionDilationSize));
    
    uint8_t bigErosionDilationSize = 10;
    cv::Mat bigAreaKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * bigErosionDilationSize + 1, 2 * bigErosionDilationSize + 1), cv::Point(bigErosionDilationSize, bigErosionDilationSize));

    bool running = true;
    while(running)
    {
        bool readDepthFrame = depthVideo.read(depthFrame);

        if(readDepthFrame)
        {
            cv::inRange(depthFrame, cv::Scalar(8, 8, 8), cv::Scalar(140, 140, 140), depthOutput);
            
            cv::GaussianBlur(depthOutput, depthOutput, cv::Size(5, 5), 20);
            cv::threshold(depthOutput, depthOutput, 150, 255, cv::THRESH_BINARY_INV);
            cv::bitwise_not(depthOutput, depthOutput);
            cv::erode(depthOutput, depthOutput, bigAreaKernel);
            cv::dilate(depthOutput, depthOutput, bigAreaKernel);

            cv::imshow("Depth frame", depthFrame);
            cv::imshow("Depth output", depthOutput);
        }
        else
        {
            printf("[MAIN_ERROR] There was an error with reading frame.\n");
            running = false;
        }

        bool readColorFrame = colorVideo.read(colorFrame);

        if(readColorFrame && readDepthFrame)
        {            
            finalOutput.setTo(0);
            depthFrame.copyTo(finalOutput, depthOutput);

            std::vector<Blob> blobs;
            blobFidner.FindBlob(finalOutput, blobs);
            if(blobs.size() > 0)
            {
                for(int i = 0; i < blobs.size(); i++)
                {
                    cv::circle(finalOutput, cv::Point(blobs[i].m_center.x, blobs[i].m_center.y), 3, cv::Scalar(4*i, 0, 0), 2);
                }
                cv::imshow("Final output", finalOutput);
                
                maskedColor.setTo(0);
                colorFrame.copyTo(maskedColor, depthOutput);
                cv::imshow("Something", maskedColor);
            }
        }

        cv::waitKey(1200);
    }

    return EXIT_SUCCESS;
}

void PrintHelp()
{
    printf("Usage: TODO\n");
}
