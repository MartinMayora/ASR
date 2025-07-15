#include "./include/audioProcessing.h"
#include "./include/wav_to_vector.h"

int main(){
    std::string test = "test.wav";
    int32_t samplingRate;
    std::vector<int16_t> wav;
    wav = readWav(test, samplingRate);
    std::cout << samplingRate << "   "; 
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
    int nMels = 30;
    std::vector<std::vector<double>> filterBank = createMelFilterbank(samplingRate, powerSpect[0].size(), nMels);
    std::cout << "Mel filter [12]:\n";
for (int k = 0; k < filterBank[12].size(); ++k) {
            if(filterBank[12][k]!=0){
        std::cout << "Bin " << k << ": " << filterBank[12][k] << "\n";}
}
    std::cout << "Mel filter [22]]:\n";
for (int k = 0; k < filterBank[22].size(); ++k) {
        if(filterBank[22][k]!=0){
        std::cout << "Bin " << k << ": " << filterBank[22][k] << "\n";
        }
}
    std::cout << "\n";

    return 0;
}