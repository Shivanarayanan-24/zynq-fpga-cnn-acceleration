#include <iostream>
#include <cmath>
#include <ap_fixed.h>

using namespace std;

typedef ap_fixed<16,8> data_t;

void conv2d_accel(
    data_t input[32][32],
    data_t kernel[3][3],
    data_t output[30][30]
);

int main() {

    float input_float[32][32];
    float kernel_float[3][3];
    float output_float[30][30];

    data_t input_fixed[32][32];
    data_t kernel_fixed[3][3];
    data_t output_fixed[30][30];

    for(int i=0;i<32;i++){
        for(int j=0;j<32;j++){
            input_float[i][j] = (float)(i*j % 255) / 255.0;
            input_fixed[i][j] = input_float[i][j];
        }
    }

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            kernel_float[i][j] = 0.1 * (i + j + 1);
            kernel_fixed[i][j] = kernel_float[i][j];
        }
    }

    for(int i=0;i<30;i++){
        for(int j=0;j<30;j++){

            float sum = 0;

            for(int ki=0;ki<3;ki++){
                for(int kj=0;kj<3;kj++){
                    sum += input_float[i+ki][j+kj] * kernel_float[ki][kj];
                }
            }

            output_float[i][j] = (sum > 0) ? sum : 0;
        }
    }

    conv2d_accel(input_fixed, kernel_fixed, output_fixed);

    float total_error = 0;
    float total_ref = 0;

    for(int i=0;i<30;i++){
        for(int j=0;j<30;j++){

            float hw = (float)output_fixed[i][j];
            float ref = output_float[i][j];

            total_error += fabs(ref - hw);
            total_ref += fabs(ref);
        }
    }

    float accuracy = 100.0 * (1.0 - (total_error / total_ref));

    cout << "Accuracy = " << accuracy << " %" << endl;

    if(accuracy > 90)
        cout << "TEST PASSED" << endl;
    else
        cout << "TEST FAILED" << endl;

    return 0;
}
