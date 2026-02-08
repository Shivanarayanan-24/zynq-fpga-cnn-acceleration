#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// ReLU activation
float relu(float x) {
    return x > 0.0f ? x : 0.0f;
}

// Softmax function
vector<float> softmax(const vector<float>& input) {
    vector<float> output(input.size());
    float sum = 0.0f;

    for (float x : input)
        sum += exp(x);

    for (size_t i = 0; i < input.size(); i++)
        output[i] = exp(input[i]) / sum;

    return output;
}

int main() {
    // Load a single image from dataset
    Mat img = imread("../dataset/class 0/0 (1).jpg", IMREAD_GRAYSCALE);

    if (img.empty()) {
        cout << "Error: Image not found!" << endl;
        return -1;
    }

    // Resize and normalize
    resize(img, img, Size(32, 32));
    img.convertTo(img, CV_32F, 1.0 / 255.0);

    // Start timer
    auto start = chrono::high_resolution_clock::now();

    // Dummy CNN output (placeholder)
    vector<float> scores = {
        0.1f, 0.2f, 0.05f, 0.3f, 0.1f,
        0.05f, 0.05f, 0.05f, 0.05f, 0.0f
    };

    vector<float> probs = softmax(scores);

    // Stop timer
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> latency = end - start;

    int predicted_class =
        max_element(probs.begin(), probs.end()) - probs.begin();

    cout << "Predicted class: " << predicted_class << endl;
    cout << "Confidence: " << probs[predicted_class] * 100 << " %" << endl;
    cout << "Latency: " << latency.count() << " ms" << endl;

    return 0;
}
