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

    cv::namedWindow("Dummy video", cv::WINDOW_NORMAL);

    BackgroundSubtraction depthSubtractor(BackgroundSubtraction::DEPTH_THRESHOLD);

    cv::Ptr<cv::BackgroundSubtractorMOG2> mog2Subtractor;
    mog2Subtractor = cv::createBackgroundSubtractorMOG2(500, 16.0, false);

    bool running = true;
    while(running)
    {
        cv::Mat depthFrame;
        bool readDepthFrame = depthVideo.read(depthFrame);

        cv::Mat depthOutput;

        if(readDepthFrame)
        {
            cv::inRange(depthFrame, cv::Scalar(5, 5, 5), cv::Scalar(130, 130, 130), depthOutput);
            

            uint8_t erosionDilationSize = 5;
            cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * erosionDilationSize + 1, 2 * erosionDilationSize + 1), cv::Point(erosionDilationSize, erosionDilationSize));
            cv::GaussianBlur(depthOutput, depthOutput, cv::Size(5, 5), 20);
            cv::threshold(depthOutput, depthOutput, 150, 255, cv::THRESH_BINARY_INV);
            cv::bitwise_not(depthOutput, depthOutput);
            cv::dilate(depthOutput, depthOutput, kernel);
            cv::erode(depthOutput, depthOutput, kernel);

            cv::imshow("Dummy video depth", depthFrame);
            cv::imshow("Dummy video depth mask", depthOutput);
        }
        else
        {
            printf("[MAIN_ERROR] There was an error with reading frame.\n");
            running = false;
        }

        cv::Mat colorFrame;
        bool readColorFrame = colorVideo.read(colorFrame);
        cv::Mat colorOutput;

        if(readColorFrame)
        {
            mog2Subtractor->apply(colorFrame, colorOutput);
            cv::GaussianBlur(colorOutput, colorOutput, cv::Size(5, 5), 6.0);
            cv::threshold(colorOutput, colorOutput, 100, 255, cv::THRESH_BINARY);
            
            uint8_t erosionDilationSize = 4;
            cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2 * erosionDilationSize + 1, 2 * erosionDilationSize + 1), cv::Point(erosionDilationSize, erosionDilationSize));
            cv::dilate(depthOutput, depthOutput, kernel);
            cv::erode(depthOutput, depthOutput, kernel);
    
            cv::imshow("Dummy video color frame", colorFrame);
            cv::imshow("Dummy video bg sub", colorOutput);
        }
        else
        {
            printf("[ERROR_MAIN] There was an error with reading frame.\n");
            running = false;
        }

        cv::Mat output;
        if(readColorFrame && readDepthFrame)
        {
            cv::bitwise_and(colorOutput, depthOutput, output);

            cv::imshow("Dummy video for final output", output);
        }

        cv::waitKey(100);
    }

    return EXIT_SUCCESS;
}

void PrintHelp()
{
    printf("Usage: TODO\
    \n-f is for file\n");
}
