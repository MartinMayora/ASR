#include <complex>
#include <vector>
#include <math.h>
#include <mutex>
#include <omp.h>
#include <algorithm>
#include "../include/wav_to_vector.h"

std::vector<std::vector<double>> frameSignal(const std::vector<int16_t>& audio, u_int16_t frameSize = 400,  int hopSize = 160 );

std::vector<double> hammingWindowing(std::vector<double> x);

std::vector<std::complex<double>> DFT(std::vector<double> x);

std::vector<double> powerSpectrum( std::vector<std::complex<double>> dft);

double mel2hz(double mel);

double hz2mel(double hz);

std::vector<std::vector<double>> createMelFilterbank(int sampleRate, int nDFT, int nMels);