#include "../include/audioProcessing.h"

std::mutex mtx;
std::vector<std::vector<double>> frameSignal(const std::vector<int16_t>& audio, u_int16_t frameSize, int hopSize) {
    std::vector<std::vector<double>> frames;
    frames.reserve(audio.size() / hopSize);
    size_t numFrames = (audio.size() >= frameSize) ? (audio.size() - frameSize) / hopSize + 1 : 0;
#pragma omp parallel for
    for (size_t idx = 0; idx< numFrames; ++idx) {
        size_t i = idx*hopSize;
        std::vector<double> frame(audio.begin() + i, audio.begin() + i + frameSize);
        std::lock_guard<std::mutex> lock(mtx);
        frames.emplace_back(std::move(frame));
}
    return frames;
}


std::vector<double> hammingWindowing(std::vector<double> x){
    double n = x.size();
    std::vector<double> toReturn;
    toReturn.reserve(n); //to reserve the memory that is going to be used

    for(size_t i = 0; i<n; i++){
        double w = 0.54 - 0.46 * cos((2 * M_PI * i)/n);
        toReturn.push_back(w*x[i]);
    }
    return toReturn;
}


std::vector<std::complex<double>> FFT(std::vector<double> x) {
    int n = x.size();
    
    // Base case
    if (n == 1) {
        return std::vector<std::complex<double>>(1, std::complex<double>(x[0], 0));
    }

    std::vector<double> even(n/2);
    std::vector<double> odd(n/2);
    for (int i = 0; i < n/2; i++) {
        even[i] = x[2*i];
        odd[i] = x[2*i + 1];
    }
    std::vector<std::complex<double>> evenFFT = FFT(even);
    std::vector<std::complex<double>> oddFFT = FFT(odd);

    std::vector<std::complex<double>> result(n);
    for (int k = 0; k < n/2; k++) {
        double angle = -2 * M_PI * k / n;
        std::complex<double> twiddle = std::complex<double>(cos(angle), sin(angle));
        
        result[k] = evenFFT[k] + twiddle * oddFFT[k];
        result[k + n/2] = evenFFT[k] - twiddle * oddFFT[k];
    }

    return result;
}

std::vector<double> padding(std::vector<double> x){
    u_int i=1;
    while (i<x.size()){
        i=i*2;
    }
    std::vector<double> padding(i, 0.0);
    std::copy(x.begin(), x.end(), padding.begin());
    return padding;
}
std::vector<std::complex<double>> DFT(std::vector<double> x){
    std::vector<double> padded = padding(x);
    int m = padded.size();
    if ((m & (m - 1)) == 0) {  // Check if n is power of 2
        return FFT(padded);
    }
    //NORMAL DFT IF IT CAN NOT DO FFT
    double n = x.size();
    double k = n;

    std::vector<std::complex<double>> toReturn;
    toReturn.reserve(k); //to reserve the memory that is going to be used

    for(int i = 0; i<k; i++){
        std::complex<double> sum (0,0);
        for(int j = 0 ; j<n; j++){
            //will calculate using sin and cosine notaion, not exp
            double realPart = cos((2*M_PI*i*j)/n);
            double imPart = -sin((2*M_PI*i*j)/n);
            std::complex<double> toSum (realPart, imPart);
            sum += x[j] * toSum;
        }
        toReturn.push_back(sum);
    }
    return toReturn;
};
std::vector<double> powerSpectrum( std::vector<std::complex<double>> dft){
    int nPositive = dft.size() / 2 + 1;
    std::vector<double> toReturn;
    toReturn.reserve(nPositive);

    for (int i = 0; i < nPositive; ++i) {
        toReturn.push_back(std::norm(dft[i]));
    }

    return toReturn;
}


double hz2mel(double hz){
    return 1127 * log(1+(hz)/700);
}

double mel2hz(double mel){
    return 700*(pow(10,mel/2595)-1);
}

std::vector<std::vector<double>> createMelFilterbank(int sampleRate, int nPowerSpecturm, int nMels){
    double melMin = 0;
    double melMax = hz2mel(sampleRate/2);

    std::vector<std::vector<double>> filterBank(nMels, std::vector(nPowerSpecturm, 0.0)); //initialize the structure

    std::vector<double> melPoints(nMels+2);
    for (int i = 0; i < nMels + 2; ++i) {
        melPoints[i] = melMin + i * (melMax - melMin) / (nMels + 1);
    }
    
    std::vector<double> hzPoints(nMels+2);
    for (int j=0; j< nMels+2; j++){
        hzPoints[j] = mel2hz(melPoints[j]);
    }

    std::vector<int> binIndices(nMels + 2);
    for (int k = 0; k < nMels + 2; k++) {
        binIndices[k] = int(hzPoints[k] * nPowerSpecturm / sampleRate);
        binIndices[k] = std::min(std::max(binIndices[k], 0), nPowerSpecturm - 1);
    }

    for (int m = 1; m <= nMels; ++m) {
        int left = binIndices[m - 1];
        int center = binIndices[m];
        int right = binIndices[m + 1];

    // skip if they are not increasing
    if (left == center || center == right || left == right) {
        continue;
    }
        for (int k = left; k < center; ++k) {
            filterBank[m - 1][k] = static_cast<double>(k - left) / (center - left);
        }
        for (int k = center; k < right; ++k) {
            filterBank[m - 1][k] = 1.0 - static_cast<double>(k - center) / (right - center);
        }
    }

    return filterBank;
}

std::vector<std::vector<double>> computeMelEnergies(std::vector<std::vector<double>> powerSpectrum, std::vector<std::vector<double>> filterBank) {
    int nFrames = powerSpectrum.size();
    int nMels = filterBank.size();
    std::vector<std::vector<double>> melEnergies(nFrames, std::vector<double>(nMels, 0.0));
 
    for (int frame = 0; frame < nFrames; ++frame) {
        for (int m = 0; m < nMels; ++m) {
            for (int bin = 0; bin < powerSpectrum[frame].size(); ++bin) {
                melEnergies[frame][m] += filterBank[m][bin] * powerSpectrum[frame][bin];
            }
        }
    }
    return melEnergies;
}