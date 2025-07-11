#include <complex>
#include <vector>
#include <math.h>
#include <mutex>
#include <omp.h>
#include "../include/wav_to_vector.h"

std::vector<std::vector<double>> frameSignal(const std::vector<int16_t>& audio, int frameSize = 1024,  int hopSize = 512 );

std::vector<double> hammingWindowing(std::vector<double> x);

std::vector<std::complex<double>> DFT(std::vector<double> x);