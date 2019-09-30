#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

#include "opencv2/opencv.hpp"
#include "BackgroundSubtraction.hpp"

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

    cv::Ptr<cv::BackgroundSubtractorMOG2> mog2Subtractor;
    mog2Subtractor = cv::createBackgroundSubtractorMOG2(500, 16.0, false);

    cv::Ptr<cv::BackgroundSubtractorKNN> knnSubtractor;
    knnSubtractor = cv::createBackgroundSubtractorKNN(500, 400, false);

    cv::Mat depthFrame;
    cv::Mat depthOutput;
    cv::Mat colorFrame;
    cv::Mat colorOutputMog2;
    cv::Mat colorOutputKnn;

    cv::Mat finalOutput;

    uint8_t smallErosionDilationSize = 5;
    cv::Mat smallAreaKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * smallErosionDilationSize + 1, 2 * smallErosionDilationSize + 1), cv::Point(smallErosionDilationSize, smallErosionDilationSize));


    uint8_t bigErosionDilationSize = 10;
    cv::Mat bigAreaKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * bigErosionDilationSize + 1, 2 * bigErosionDilationSize + 1), cv::Point(bigErosionDilationSize, bigErosionDilationSize));

    bool running = true;
    while(running)
    {
        bool readDepthFrame = depthVideo.read(depthFrame);

        if(readDepthFrame)
        {
            cv::inRange(depthFrame, cv::Scalar(5, 5, 5), cv::Scalar(130, 130, 130), depthOutput);
            
            cv::GaussianBlur(depthOutput, depthOutput, cv::Size(5, 5), 20);
            cv::threshold(depthOutput, depthOutput, 150, 255, cv::THRESH_BINARY_INV);
            cv::bitwise_not(depthOutput, depthOutput);
            cv::dilate(depthOutput, depthOutput, smallAreaKernel);
            cv::erode(depthOutput, depthOutput, smallAreaKernel);

            cv::imshow("Dummy video depth", depthFrame);
            cv::imshow("Dummy video depth mask", depthOutput);
        }
        else
        {
            printf("[MAIN_ERROR] There was an error with reading frame.\n");
            running = false;
        }

        bool readColorFrame = colorVideo.read(colorFrame);

        if(readColorFrame)
        {
            mog2Subtractor->apply(colorFrame, colorOutputMog2);
            cv::GaussianBlur(colorOutputMog2, colorOutputMog2, cv::Size(5, 5), 6.0);
            cv::threshold(colorOutputMog2, colorOutputMog2, 100, 255, cv::THRESH_BINARY);
            
            cv::dilate(colorOutputMog2, colorOutputMog2, bigAreaKernel);
            cv::erode(colorOutputMog2, colorOutputMog2, smallAreaKernel);

            knnSubtractor->apply(colorFrame, colorOutputKnn);
            cv::GaussianBlur(colorOutputKnn, colorOutputKnn, cv::Size(5, 5), 6.0);
            cv::threshold(colorOutputKnn, colorOutputKnn, 100, 255, cv::THRESH_BINARY);
            
            cv::dilate(colorOutputKnn, colorOutputKnn, bigAreaKernel);
            cv::erode(colorOutputKnn, colorOutputKnn, smallAreaKernel);

            //cv::morphologyEx(colorOutputKnn, colorOutputKnn, cv::MORPH_CLOSE, smallAreaKernel);

            cv::imshow("Dummy video color frame", colorFrame);
            cv::imshow("Dummy video bg sub MOG2", colorOutputMog2);
            cv::imshow("Dummy video bg sub KNN", colorOutputKnn);
        }
        else
        {
            printf("[ERROR_MAIN] There was an error with reading frame.\n");
            running = false;
        }

        if(readColorFrame && readDepthFrame)
        {
            cv::bitwise_and(colorOutputKnn, colorOutputMog2, colorOutputMog2);
            cv::bitwise_or(colorOutputMog2, depthOutput, depthOutput);

            finalOutput.setTo(0);
            colorFrame.copyTo(finalOutput, depthOutput);

            cv::imshow("Dummy video for final output", finalOutput);
        }

        cv::waitKey(100);
    }

    return EXIT_SUCCESS;
}

void PrintHelp()
{
    printf("Usage: TODO\n");
}
