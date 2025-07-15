#include "../include/wav_to_vector.h"

struct WavHeader {
    uint8_t     riff[4];        // "RIFF"
    uint32_t    fileSize;       // File size - 8 bytes
    uint8_t     wave[4];        // "WAVE"
    uint8_t     fmt[4];         // "fmt "
    uint32_t    fmtSize;        // 16 for PCM
    uint16_t    audioFormat;    // 1 for PCM
    uint16_t    numChannels;    // 1 (mono) or 2 (stereo)
    uint32_t    sampleRate;     // 8000, 16000, etc.
    uint32_t    byteRate;       // sampleRate * numChannels * bitsPerSample/8
    uint16_t    blockAlign;     // numChannels * bitsPerSample/8
    uint16_t    bitsPerSample;  // 8, 16, etc.
    uint8_t     data[4];        // "data"
    uint32_t    dataSize;       // Raw data size
};

std::vector<int16_t> readWav(std::string fileName, int32_t& samplingRate) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open WAV file.");
    }

    char chunkId[4];
    uint32_t chunkSize;

    // Check RIFF
    file.read(chunkId, 4);
    if (std::string(chunkId, 4) != "RIFF") {
        throw std::runtime_error("Not a RIFF file.");
    }
    file.ignore(4); // skip file size

    // Check WAVE
    file.read(chunkId, 4);
    if (std::string(chunkId, 4) != "WAVE") {
        throw std::runtime_error("Not a WAVE file.");
    }

    uint16_t audioFormat = 0, numChannels = 0, bitsPerSample = 0; uint32_t sampleRate = 0;
    
    while (file.read(chunkId, 4)) {
        file.read(reinterpret_cast<char*>(&chunkSize), 4);
        std::string id(chunkId, 4);

        if (id == "fmt ") {
            file.read(reinterpret_cast<char*>(&audioFormat), 2);
            file.read(reinterpret_cast<char*>(&numChannels), 2);
            file.read(reinterpret_cast<char*>(&sampleRate), 4);
            file.ignore(6); // byteRate + blockAlign
            file.read(reinterpret_cast<char*>(&bitsPerSample), 2);
            if (chunkSize > 16) file.ignore(chunkSize - 16); // skip extra fmt
        }
        else if (id == "data") {
            if (bitsPerSample != 16) {
                throw std::runtime_error("Only 16-bit PCM is supported.");
            }

            size_t numSamples = chunkSize / sizeof(int16_t);
            std::vector<int16_t> samples(numSamples);
            file.read(reinterpret_cast<char*>(samples.data()), chunkSize);

            samplingRate = sampleRate;

            // Convert stereo to mono (if needed)
            if (numChannels == 2) {
                std::vector<int16_t> monoSamples(numSamples / 2);
                for (size_t i = 0; i < monoSamples.size(); ++i) {
                    int32_t left = samples[2 * i];
                    int32_t right = samples[2 * i + 1];
                    monoSamples[i] = static_cast<int16_t>((left + right) / 2);
                }
                return monoSamples;
            }

            return samples;
        }
        else {
            file.ignore(chunkSize); // skip irrelevant chunk
        }
    }

    throw std::runtime_error("No 'data' chunk found in WAV file.");
}
