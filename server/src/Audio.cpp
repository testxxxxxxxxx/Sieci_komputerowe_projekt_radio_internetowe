#include "../include/Audio.hpp"
#include "../include/Multiplexer.hpp"

#define DR_MP3_IMPLEMENTATION
#include "../include/dr_mp3.h"

using namespace std;
using namespace Threads;
using namespace Clients;

void Audio::run(Playlist* pl, unordered_map<int, Client*>& clients, Multiplexing::Multiplexer* mux) {

    const int CHUNK = 4096;
    int16_t pcmBuffer[CHUNK * 2];

    while(1) {
        string fileName = pl->currentBlocking();

        drmp3 mp3;
        if (!drmp3_init_file(&mp3, fileName.c_str(), NULL)) {
            continue;
        }

        //pl->changed.store(false);
        pl->ch = false;

        int count = 0;

        cout<<fileName<<endl;

        for(auto& s : pl->list()) {
            cout<<s<<endl;
        }

        while(1) {
            size_t samples = drmp3_read_pcm_frames_s16(&mp3, CHUNK, pcmBuffer);
 
            if(pl->changed.exchange(false)) {
                cout<<"co"<<endl;
                break;
            } 

            if (samples == 0) {
                if (count >= 3) {
                    cout<<"dlaczego"<<endl;
                    count++;
                    pl->next(); 
                    break;
                }
                continue;
            }

            count = 0;

            /*if(pl->changed.exchange(false)) {
                cout<<"co"<<endl;
                break;
            }*/

            size_t bytes = samples * mp3.channels * sizeof(int16_t);

            vector<uint8_t> frame(
                (uint8_t*)pcmBuffer,
                (uint8_t*)pcmBuffer + bytes
            );
            
            for (auto& [fd, cl] : clients) {
                lock_guard<mutex> lock(cl->cm);
                cl->q.push(frame);
                cl->write = true;
                mux->notifyWritable(fd);
            }
        }

        drmp3_uninit(&mp3);
    }
}
