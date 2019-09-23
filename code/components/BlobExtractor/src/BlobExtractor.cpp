#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

#include "opencv2/opencv.hpp"

void PrintHelp();

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        PrintHelp();
        return EXIT_FAILURE;
    }

    char* filePath;

    int c;
    while( ( c = getopt (argc, argv, "hf:") ) != -1 ) 
    {
        switch(c)
        {
            case 'f':
            {
                if(optarg) 
                {
                    filePath = optarg;
                }
                else
                {
                    PrintHelp();
                }
                break;
            }
            case 'h':
            default:
            {
                PrintHelp();
                break;
            }
        }
    }

    cv::VideoCapture video(filePath);
    if(!video.isOpened())
    {
        printf("[MAIN_ERROR] Unable to open given video.\n");
        return EXIT_FAILURE;
    }

    cv::namedWindow("Dummy video", cv::WINDOW_NORMAL);

    bool running = true;
    while(running)
    {
        cv::Mat frame;
        bool readFrame = video.read(frame);
        if(readFrame)
        {
            cv::imshow("Dummy video", frame);
            cv::waitKey(0);
        }
        else
        {
            printf("[MAIN_ERROR] There was an error with reading frame.\n");
        }
    }

    return EXIT_SUCCESS;
}

void PrintHelp()
{
    printf("Usage: TODO\
    \n-f is for file\n");
}
