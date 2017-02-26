//
//  main.cpp
//  EtchASketch
//
//  Created by Bliss Chapman on 2/7/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#include <cstdio>
#include <iostream>
#include <string>
#include <unistd.h>
#include "EtchASketch.hpp"
#include "motor.h"

using std::cout;
using std::endl;
using std::string;

/// Print usage and exit.
__attribute__((noreturn))
static void
usage(void)
{
    cout << "Usage: etch -i /path/to/input/image.bmp -w 800 -h 600" << endl;
    exit(1);
}

static void
validateArgs(const string &inFile, long imgWidth, long imgHeight)
{
    if (inFile.size() <= 0) {
        goto fail;
    }
    if (imgWidth <= 0) {
        goto fail;
    }
    if (imgHeight <= 0) {
        goto fail;
    }
    // Everything checks out.
    return;

fail:
    usage();
}

int
main(int argc, char * const argv[])
{
    motor_initialize();
    
    cout << "Welcome to etch ❤️" << endl;

    // Parse arguments.
    string inFile;
    long imgWidth = -1, imgHeight = -1;
    int ch;
    while ((ch = getopt(argc, argv, "i:w:h:")) != -1) {
        switch (ch) {
        case 'i':
            inFile = string(optarg);
            break;
        case 'w':
            imgWidth = strtol(optarg, nullptr, 0);
            break;
        case 'h':
            imgHeight = strtol(optarg, nullptr, 0);
            break;
        case '?':
        default:
            usage();
        }
    }
    validateArgs(inFile, imgWidth, imgHeight);

    // Make sure we can open the input file.
    FILE *inImageFd = fopen(inFile.c_str(), "r");
    if (!inImageFd) {
        perror("Can't open input image");
        exit(1);
    }

    // Read in the input image.
    etchasketch::Image::Pixel *rawInputImage = new etchasketch::Image::Pixel[imgWidth * imgHeight];
    if (!rawInputImage) {
        perror("Out of memory");
        exit(1);
    }
    if (fread(rawInputImage, sizeof(etchasketch::Image::Pixel), imgWidth * imgHeight, inImageFd) != imgWidth * imgHeight) {
        perror("Unable to read input image");
        exit(1);
    }

    // Copy the input image into an etchasketch::Image.
    etchasketch::Image inputImg = etchasketch::Image(imgWidth, imgHeight, rawInputImage);
    delete [] rawInputImage;
    rawInputImage = nullptr;

    // Create an ImageFlow.
    etchasketch::ImageFlow inputImgFlow = etchasketch::ImageFlow(inputImg);
    inputImgFlow.convertToGrayscale();
    inputImgFlow.detectEdges();
    inputImgFlow.generateEdgePoints();
    inputImgFlow.orderEdgePointsForDrawing();
    std::vector<etchasketch::KDPoint<2>> points = inputImgFlow.getOrderedEdgePoints();
    cout << "ImageFlow completed its run." << endl;
/*    etchasketch::utils::writeOrderedEdgePointsToFile(
        "lena_ordered_edge_points.png",
        points,
        imgWidth,
        imgHeight);
*/

    // Set up a motor.
    motor_t mx;
    motor_t my;

    if (motor_init(&mx)) {
        fprintf(stderr, "Error creating motor x.\n");
        return 1;
    }
    if (motor_init(&my)) {
        fprintf(stderr, "Error creating motor y.\n");
        return 1;
    }

    size_t curr_x = 0;
    size_t curr_y = 0;

    for(size_t i = 0; i < points.size(); i++) {
      etchasketch::KDPoint<2> target = points[i];

      // MOVE X TO TARGET
      while(curr_x != target[0]) {
        if(curr_x < target[0]) {
          motor_move(&mx, DIR_CW);
          curr_x++;
        } else {
          motor_move(&mx, DIR_CCW);
          curr_x--;
        }
      }

      // MOVE Y TO TARGET
      while(curr_y != target[1]) {
        if(curr_y < target[1]) {
          motor_move(&my, DIR_CW);
          curr_y++;
        } else {
          motor_move(&my, DIR_CCW);
          curr_y--;
        }
      }
    }

    return 0;
}
