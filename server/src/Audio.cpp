#include "../include/Audio.hpp"

#define DR_MP3_IMPLEMENTATION
#include "../include/dr_mp3.h"

using namespace std;
using namespace Threads;
using namespace Clients;

void Audio::run(Playlist* pl, unordered_map<int, Client*>& clients) {

    const int CHUNK = 4096;
    int16_t pcmBuffer[CHUNK];

    while(1) {
        string fileName = pl->currentBlocking();

        drmp3 mp3;
        if (!drmp3_init_file(&mp3, fileName.c_str(), NULL)) {
            continue;
        }

        while(1) {
            size_t samples = drmp3_read_pcm_frames_s16(&mp3, CHUNK, pcmBuffer);
            if (samples == 0)
                break;

            vector<uint8_t> frame(
                (uint8_t*)pcmBuffer,
                (uint8_t*)pcmBuffer + samples * sizeof(int16_t)
            );

            for (auto& [fd, cl] : clients) {
                lock_guard<mutex> lock(cl->cm);
                cl->q.push(frame);
                cl->write = true;
            }
        }

        drmp3_uninit(&mp3);
    }
}
