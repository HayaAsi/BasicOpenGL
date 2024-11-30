#pragma once
#include <iostream>
#include <vector>
using namespace std;

unsigned char* convertToGrayscale(const unsigned char* data, int width, int height);

// Canny_Edge_Detector (Improved)
unsigned char* Canny_Edge_Detector(unsigned char* data, int width, int height);

// Halftone
unsigned char* convertToHalftone(unsigned char* data, int width, int height);

// Floyd_Steinberg_Algorithm
unsigned char* convertToFloyd(unsigned char* data, int width, int height);