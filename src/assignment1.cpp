#include "./assignment1.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

unsigned char* convertToGrayscale(const unsigned char* data, int width, int height) {
    // Allocate memory for grayscale data (RGBA format: 4 bytes per pixel)
    unsigned char* grayScale = (unsigned char*)malloc(width * height * 4);
    if (!grayScale) {
        fprintf(stderr, "Failed to allocate memory for grayscale image\n");
        return NULL;
    }

    // Convert each pixel to grayscale
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = 4 * (i * width + j); // Index for RGBA components
            unsigned char grayValue = (unsigned char)(
                data[index + 0] * 0.3 +   // Red component
                data[index + 1] * 0.58 +  // Green component
                data[index + 2] * 0.11    // Blue component
            );
            // Set grayscale value for all channels (R, G, B, A)
            grayScale[index + 0] = grayValue; // Red
            grayScale[index + 1] = grayValue; // Green
            grayScale[index + 2] = grayValue; // Blue
            grayScale[index + 3] = data[index + 3]; // Preserve alpha
        }
    }

    return grayScale;
}

unsigned char* greyScaleImageConverter(unsigned char* data, int width, int height) {
    // Allocate memory for the grayscale image
    unsigned char* greyScaleImage = (unsigned char*)malloc(width * height);
    if (!greyScaleImage) {
        fprintf(stderr, "Failed to allocate memory for grayscale image\n");
        return NULL;
    }
    FILE* file = fopen("../gray.txt", "w");
    // Convert the image to grayscale
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = 4 * (i * width + j); // Original image index
            // Calculate the mean value of RGB
            unsigned char grayValue = (unsigned char)(data[index + 0] * 0.3 + data[index + 1] * 0.58 + data[index + 2] * 0.11);
            greyScaleImage[i * width + j] = grayValue;
            fprintf(file, "%d\n", greyScaleImage[i * width + j]);
        }
    }
    return greyScaleImage;
}

unsigned char* convertToHalftone(unsigned char* data, int width, int height) {
    // Convert the original image to grayscale
    unsigned char* grey_scale_matrix = greyScaleImageConverter(data, width, height);
    if (!grey_scale_matrix) {
        fprintf(stderr, "Failed to convert image to grayscale\n");
        return NULL;
    }

    // Halftone dimensions: double the original width and height
    int newWidth = width * 2;
    int newHeight = height * 2;

    // Allocate memory for the halftone image
    unsigned char* halftone = (unsigned char*)malloc(newWidth * newHeight * 4);
    if (!halftone) {
        fprintf(stderr, "Failed to allocate memory for halftone image\n");
        free(grey_scale_matrix);
        return NULL;
    }

    // Initialize the halftone image
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            int idx = (i * newWidth + j) * 4;
            halftone[idx] = 0;       // R
            halftone[idx + 1] = 0;   // G
            halftone[idx + 2] = 0;   // B
            halftone[idx + 3] = 255; // A
        }
    }

    // Fill the halftone image based on grayscale intensity
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char grayValue = grey_scale_matrix[i * width + j];

            // Determine the halftone pattern based on intensity levels
            if (grayValue <= 50) {
                 // All black
                // (No changes needed, as halftone is already initialized to black)
            } else if (grayValue <= 101) {
                // Bottom-left pixel is white
                int idx = ((i * 2 + 1) * newWidth + j * 2) * 4;
                halftone[idx] = 255;
                halftone[idx + 1] = 255;
                halftone[idx + 2] = 255;
            } else if (grayValue <= 152) {
                // Bottom-left and top-right pixels are white
                int idx1 = ((i * 2 + 1) * newWidth + j * 2) * 4; // Bottom-left
                int idx2 = (i * 2 * newWidth + (j * 2 + 1)) * 4; // Top-right
                halftone[idx1] = halftone[idx2] = 255;
                halftone[idx1 + 1] = halftone[idx2 + 1] = 255;
                halftone[idx1 + 2] = halftone[idx2 + 2] = 255;
            } else if (grayValue <= 203) {
                // All pixels except top-left are white
                int idx1 = ((i * 2) * newWidth + (j * 2 + 1)) * 4; // Top-right
                int idx2 = ((i * 2 + 1) * newWidth + j * 2) * 4;   // Bottom-left
                int idx3 = ((i * 2 + 1) * newWidth + (j * 2 + 1)) * 4; // Bottom-right
                halftone[idx1] = halftone[idx2] = halftone[idx3] = 255;
                halftone[idx1 + 1] = halftone[idx2 + 1] = halftone[idx3 + 1] = 255;
                halftone[idx1 + 2] = halftone[idx2 + 2] = halftone[idx3 + 2] = 255;
            } else {
                // All pixels are white
                int idx1 = ((i * 2) * newWidth + j * 2) * 4;         // Top-left
                int idx2 = ((i * 2) * newWidth + (j * 2 + 1)) * 4;   // Top-right
                int idx3 = ((i * 2 + 1) * newWidth + j * 2) * 4;     // Bottom-left
                int idx4 = ((i * 2 + 1) * newWidth + (j * 2 + 1)) * 4; // Bottom-right
                halftone[idx1] = halftone[idx2] = halftone[idx3] = halftone[idx4] = 255;
                halftone[idx1 + 1] = halftone[idx2 + 1] = halftone[idx3 + 1] = halftone[idx4 + 1] = 255;
                halftone[idx1 + 2] = halftone[idx2 + 2] = halftone[idx3 + 2] = halftone[idx4 + 2] = 255;
            }
        }
    }
    free(grey_scale_matrix);
    return halftone;
}

// Truncate the pixel value to the nearest multiple of 16
unsigned char truncateToNearest16(unsigned char pixel_value) {
    return pixel_value - (pixel_value % 16);
}

// Floyd-Steinberg Algorithm
unsigned char* convertToFloyd(unsigned char* data, int width, int height) {
    // Allocate a 2D integer matrix for grayscale values
    int** grayMatrix = (int**)malloc(height * sizeof(int*));
    if (!grayMatrix) {
        fprintf(stderr, "Failed to allocate memory for grayscale matrix\n");
        return NULL;
    }
    for (int i = 0; i < height; i++) {
        grayMatrix[i] = (int*)malloc(width * sizeof(int));
        if (!grayMatrix[i]) {
            fprintf(stderr, "Failed to allocate memory for grayscale row\n");
            for (int j = 0; j < i; j++) free(grayMatrix[j]);
            free(grayMatrix);
            return NULL;
        }
    }

    // Convert data to grayscale and populate the matrix
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int idx = 4 * (y * width + x);
            grayMatrix[y][x] = (int)(data[idx] * 0.3 + data[idx + 1] * 0.58 + data[idx + 2] * 0.11);
        }
    }

    // Allocate the result data array
    unsigned char* result = (unsigned char*)malloc(4 * width * height);
    if (!result) {
        fprintf(stderr, "Failed to allocate memory for result image\n");
        for (int i = 0; i < height; i++) free(grayMatrix[i]);
        free(grayMatrix);
        return NULL;
    }

    // Perform Floyd-Steinberg dithering
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char originalValue = (unsigned char)fmin(grayMatrix[y][x], 255);
            unsigned char newValue = truncateToNearest16(originalValue);
            int error = grayMatrix[y][x] - newValue;

            // Update the current pixel in the result array
            int idx = 4 * (y * width + x);
            result[idx] = result[idx + 1] = result[idx + 2] = newValue;
            result[idx + 3] = 255; // Alpha channel

            // Distribute the error to neighboring pixels
            if (x + 1 < width) grayMatrix[y][x + 1] += (int)(error * (7.0 / 16));
            if (x - 1 >= 0 && y + 1 < height) grayMatrix[y + 1][x - 1] += (int)(error * (3.0 / 16));
            if (x + 1 < width && y + 1 < height) grayMatrix[y + 1][x + 1] += (int)(error * (1.0 / 16));
        }
    }

    // Free grayscale matrix memory
    for (int i = 0; i < height; i++) {
        free(grayMatrix[i]);
    }
    free(grayMatrix);

    return result;
}

void setPixel(unsigned char* data, int i, int j, int value, int width) {
    for (int k = 0; k < 3; k++)
        data[4 * (i + j * width) + k] = value;
    data[4 * (i + j * width) + 3] = 255; // Alpha
}

int getPixel(int* data, int i, int j, int width) {
    return data[i + j * width];
}

void setPixelInt(int* data, int i, int j, int value, int width) {
    data[i + j * width] = value;
}

int* pictureToIntArray(unsigned char* data, int width, int height) {
    int* n_data = (int*)malloc(sizeof(int) * width * height);
    for (int i = 0; i < width * height; i++)
        n_data[i] = (data[i * 4] + data[i * 4 + 1] + data[i * 4 + 2]) / 3;
    return n_data;
}

int* applyFilter(int* data, int* filter, int filter_size, int width, int height) {
    int* filtered_data = (int*)malloc(sizeof(int) * width * height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int filter_sum = 0;
            for (int m = -1; m <= 1; m++) {
                for (int n = -1; n <= 1; n++) {
                    if ((i + m >= 0) && (i + m < height) && (j + n >= 0) && (j + n < width)) {
                        filter_sum += getPixel(data, i + m, j + n, width) * filter[(m + 1) + 3 * (n + 1)];
                    }
                }
            }
            filtered_data[i + j * width] = filter_sum / filter_size;
        }
    }
    return filtered_data;
}

void computeMagnitudeAndAngles(int* magnitude, double* angles, int* x_div, int* y_div, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int x = getPixel(x_div, i, j, width);
            int y = getPixel(y_div, i, j, width);
            magnitude[i + j * width] = sqrt(x * x + y * y);
            angles[i + j * width] = atan2(y, x) * 180 / M_PI;
        }
    }
}

void nonMaxSuppression(int* output, int* magnitude, double* angles, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int current_pixel = getPixel(magnitude, i, j, width);

            // If the pixel is on the boundary, set it to 0
            if (i == 0 || j == 0 || i == 255 || j == 255) {
                setPixelInt(output, i, j, 0, width);
                continue;
            }
            // Determine the gradient direction
            int direction = ((int)(angles[i + j * width] + 22.5 + 180) / 45) % 4;

            // Perform non-maximum suppression based on the direction
            switch (direction) {
                case 0: // Horizontal edge
                    if (current_pixel > getPixel(magnitude, i-1, j, width) &&
                        current_pixel > getPixel(magnitude, i+1, j, width)) {
                        setPixelInt(output, i, j, current_pixel, width);
                    } else {
                        setPixelInt(output, i, j, 0, width);
                    }
                    break;

                case 1: // Diagonal edge (bottom-left to top-right)
                    if (current_pixel > getPixel(magnitude, i + 1, j - 1, width) &&
                        current_pixel > getPixel(magnitude, i - 1, j + 1, width)) {
                        setPixelInt(output, i, j, current_pixel, width);
                    } else {
                        setPixelInt(output, i, j, 0, width);
                    }
                    break;

                case 2: // Vertical edge
                    if (current_pixel > getPixel(magnitude, i, j-1, width) &&
                        current_pixel > getPixel(magnitude, i, j+1, width)) {
                        setPixelInt(output, i, j, current_pixel, width);
                    } else {
                        setPixelInt(output, i, j, 0, width);
                    }
                    break;

                case 3: // Diagonal edge (top-left to bottom-right)
                    if (current_pixel > getPixel(magnitude, i - 1, j - 1, width) &&
                        current_pixel > getPixel(magnitude, i + 1, j + 1, width)) {
                        setPixelInt(output, i, j, current_pixel, width);
                    } else {
                        setPixelInt(output, i, j, 0, width);
                    }
                    break;

                default:
                    setPixelInt(output, i, j, 0, width);
                    break;
            }
        }
    }
}

void doubleThresholdAndHysteresis(int* output, int* input, int width, int height, int low, int high) {
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int pixel = getPixel(input, i, j, width);
            if (pixel >= high) {
                setPixelInt(output, i, j, 255, width);
            } else if (pixel >= low) {
                for (int m = -1; m <= 1; m++) {
                    for (int n = -1; n <= 1; n++) {
                        if (getPixel(input, i + m, j + n, width) >= high) {
                            setPixelInt(output, i, j, 255, width);
                            break;
                        }
                    }
                }
            } else {
                setPixelInt(output, i, j, 0, width);
            }
        }
    }
}

unsigned char* Canny_Edge_Detector(unsigned char* data, int width, int height) {
    int filter1[] = {1, 2, 1, 2, 4, 2, 1, 2, 1};   // Gaussian Blur
    int filter2[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1}; // Sobel X
    int filter3[] = {1, 2, 1, 0, 0, 0, -1, -2, -1}; // Sobel Y

    // Step 1: Convert image to grayscale
    int* grayData = pictureToIntArray(data, width, height);

    // Step 2: Apply Gaussian blur
    int* blurredData = applyFilter(grayData, filter1, 16, width, height);

    // Step 3: Compute gradients (Sobel)
    int* xGradient = applyFilter(blurredData, filter2, 1, width, height);
    int* yGradient = applyFilter(blurredData, filter3, 1, width, height);

    // Step 4: Compute magnitude and angles
    int* magnitude = (int*)malloc(sizeof(int) * width * height);
    double* angles = (double*)malloc(sizeof(double) * width * height);
    computeMagnitudeAndAngles(magnitude, angles, xGradient, yGradient, width, height);

    // Step 5: Non-Max Suppression
    int* nonMaxSuppressed = (int*)malloc(sizeof(int) * width * height);
    nonMaxSuppression(nonMaxSuppressed, magnitude, angles, width, height);

    // Step 6: Double Threshold and Hysteresis
    int* finalEdges = (int*)malloc(sizeof(int) * width * height);
    doubleThresholdAndHysteresis(finalEdges, nonMaxSuppressed, width, height, 60, 160);

    // Convert back to picture format
    unsigned char* finalImage = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel = finalEdges[i + j * width];
            if (pixel < 0) pixel = 0;
            else if (pixel > 255) pixel = 255;
            setPixel(finalImage, i, j, pixel, width);
        }
    }
    // Free allocated memory
    free(grayData);
    free(blurredData);
    free(xGradient);
    free(yGradient);
    free(magnitude);
    free(angles);
    free(nonMaxSuppressed);
    free(finalEdges);

    return finalImage;
}