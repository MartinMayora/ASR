#include "../include/audioProcessing.h"

std::mutex mtx;
std::vector<std::vector<double>> frameSignal(const std::vector<int16_t>& audio, int frameSize, int hopSize) {
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

std::vector<std::complex<double>> DFT(std::vector<double> x){
    int m = x.size();
    if ((m & (m - 1)) == 0) {  // Check if n is power of 2
        return FFT(x);
    }
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
