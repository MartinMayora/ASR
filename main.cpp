#include "./include/audioProcessing.h"
#include "./include/wav_to_vector.h"

int main(){
    std::string test = "test.wav";
    int16_t samplingRate;
    std::vector<int16_t> wav;
    wav = readWav(test, samplingRate);

    std::cout<< wav.size() << "\n";
    std::vector<std::vector<double>> frames = frameSignal(wav);
    std::vector<std::vector<double>> powerSpect; //could be optimized, one vector with strides
    powerSpect.reserve(frames.size());
    std::cout << frames.size();
    for(auto& frame: frames){
        auto windowed = hammingWindowing(frame);
        auto dft = DFT(windowed);
        auto pSpecturm = powerSpectrum(dft);
        powerSpect.push_back(pSpecturm);
    }
    int nMels = 40;
    std::vector<std::vector<double>> filterBank = createMelFilterbank(samplingRate, powerSpect.size(), nMels);
    std::cout<< "\n size of filterBank[0]: " << filterBank[0].size();

    return 0;
}