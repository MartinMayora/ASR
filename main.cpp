#include "./include/DFT.h"
#include "./include/wav_to_vector.h"

int main(){
std::string test = "test.wav";
int16_t samplingRate;
std::vector<int16_t> wav;
wav = readWav(test, samplingRate);

std::cout<< wav.size() << "\n";
std::vector<std::vector<double>> frames = frameSignal(wav);

std::cout << frames.size();
for(auto& frame: frames){
    auto windowed = hammingWindowing(frame);
    auto dft = DFT(windowed);
}

return 0;
}