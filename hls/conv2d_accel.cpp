#include <ap_fixed.h>

typedef ap_fixed<16,8> data_t;

void conv2d_accel(
    data_t input[32][32],
    data_t kernel[3][3],
    data_t output[30][30]
) {

#pragma HLS INTERFACE m_axi port=input  offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=kernel offset=slave bundle=gmem
#pragma HLS INTERFACE m_axi port=output offset=slave bundle=gmem
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS ARRAY_PARTITION variable=kernel complete dim=0

    for (int i = 0; i < 30; i++) {
        for (int j = 0; j < 30; j++) {

#pragma HLS PIPELINE II=1

            data_t sum = 0;

            for (int ki = 0; ki < 3; ki++) {
#pragma HLS UNROLL
                for (int kj = 0; kj < 3; kj++) {
#pragma HLS UNROLL
                    sum += input[i + ki][j + kj] * kernel[ki][kj];
                }
            }

            output[i][j] = (sum > 0) ? sum : (data_t)0;
        }
    }
}
