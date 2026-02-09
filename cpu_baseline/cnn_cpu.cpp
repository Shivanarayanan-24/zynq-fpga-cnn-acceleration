#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    // 1. Load image (adjust path if needed)
    Mat img = imread("../dataset/class 0/0 (1).jpg", IMREAD_GRAYSCALE);
    if (img.empty()) {
        cout << "Error: Image not found!" << endl;
        return -1;
    }

    // 2. Resize and normalize
    resize(img, img, Size(32, 32));
    img.convertTo(img, CV_32F, 1.0 / 255.0);

    // 3. Convert Mat to raw array
    float input[32][32];
    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 32; j++) {
            input[i][j] = img.at<float>(i, j);
        }
    }

    // 4. Define 3x3 convolution kernel
    float kernel[3][3] = {
        { 1,  0, -1},
        { 1,  0, -1},
        { 1,  0, -1}
    };

    // 5. Output feature map (30x30)
    float output[30][30];

    // 6. Start timing
    auto start = chrono::high_resolution_clock::now();

    // 7. Convolution + ReLU
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            float sum = 0.0f;

            for (int ki = 0; ki < 3; ki++) {
                for (int kj = 0; kj < 3; kj++) {
                    sum += input[i + ki][j + kj] * kernel[ki][kj];
                }
            }

            // ReLU activation
            output[i][j] = (sum > 0.0f) ? sum : 0.0f;
        }
    }

    // 8. Stop timing
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> latency = end - start;

    // 9. Print results
    cout << "Convolution completed successfully." << endl;
    cout << "Convolution latency: " << latency.count() << " ms" << endl;
    cout << "Sample output value [10][10]: " << output[10][10] << endl;

    return 0;
}
