#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    // 1. Load grayscale image
    Mat img = imread("../dataset/class 0/0 (1).jpg", IMREAD_GRAYSCALE);
    if (img.empty()) {
        cout << "Error: Image not found!" << endl;
        return -1;
    }

    // 2. Resize to 32x32 and normalize
    resize(img, img, Size(32, 32));
    img.convertTo(img, CV_32F, 1.0 / 255.0);

    // 3. Convert OpenCV Mat to raw array
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

    // 5. Output of convolution (30x30)
    float conv_out[30][30];

    // Start timing
    auto start = chrono::high_resolution_clock::now();

    // 6. Convolution + ReLU
    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {
            float sum = 0.0f;

            for (int ki = 0; ki < 3; ki++) {
                for (int kj = 0; kj < 3; kj++) {
                    sum += input[i + ki][j + kj] * kernel[ki][kj];
                }
            }

            // ReLU activation
            conv_out[i][j] = (sum > 0.0f) ? sum : 0.0f;
        }
    }

    // 7. Max Pooling (2x2, stride 2) -> 15x15
    float pooled[15][15];

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            float m = conv_out[i * 2][j * 2];
            m = max(m, conv_out[i * 2 + 1][j * 2]);
            m = max(m, conv_out[i * 2][j * 2 + 1]);
            m = max(m, conv_out[i * 2 + 1][j * 2 + 1]);
            pooled[i][j] = m;
        }
    }

    // 8. Flatten (15x15 -> 225)
    float flat[225];
    int idx = 0;

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            flat[idx++] = pooled[i][j];
        }
    }

    // 9. Simple classifier (dummy fully connected layer)
    float scores[10] = {0};

    for (int c = 0; c < 10; c++) {
        for (int i = 0; i < 225; i++) {
            scores[c] += flat[i] * (0.01f * (c + 1));
        }
    }

    // 10. Find predicted class
    int predicted = 0;
    float max_score = scores[0];

    for (int c = 1; c < 10; c++) {
        if (scores[c] > max_score) {
            max_score = scores[c];
            predicted = c;
        }
    }

    // Stop timing
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> latency = end - start;

    // 11. Output results
    cout << "CPU CNN inference completed successfully." << endl;
    cout << "Predicted class: " << predicted << endl;
    cout << "Total CPU latency (Conv + Pool + FC): "
         << latency.count() << " ms" << endl;

    return 0;
}
