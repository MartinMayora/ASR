#include "./include/audioProcessing.h"
#include "./include/wav_to_vector.h"

int main(){
    std::string test = "test.wav";
    int32_t samplingRate;
    std::vector<int16_t> wav;
    wav = readWav(test, samplingRate);

    std::vector<std::vector<double>> frames = frameSignal(wav);
    std::vector<std::vector<double>> powerSpect; //could be optimized, one vector with strides
    powerSpect.reserve(frames.size());
    
    for(auto& frame: frames){
        auto windowed = hammingWindowing(frame);
        auto dft = DFT(windowed);
        auto pSpecturm = powerSpectrum(dft);
        powerSpect.push_back(pSpecturm);
    }
    
    int nMels;
    std::vector<std::vector<double>> filterBank = createMelFilterbank(samplingRate, powerSpect[0].size(), nMels = 30);
    std::vector<std::vector<double>> melEnergies = computeMelEnergies(powerSpect, filterBank);
    
    return 0;
}