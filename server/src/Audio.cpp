#include <iostream>
#include "../include/Audio.hpp"
#include "../include/minimp3_ex.h"
#include "../include/Multiplexer.hpp"
#include <mutex>
#include <unordered_map>

using namespace std;
using namespace Threads;
using namespace Clients;

void Audio::run(Playlist* pl, unordered_map<int, Clients::Client*>& clients) {

    mp3dec_ex_t dec;
    const int CHUNK = 4096;
    int16_t buffer[CHUNK];

    while(1) {
        string fileName = pl->currentBlocking();
        if (mp3dec_ex_open(&dec, fileName.c_str(), MP3D_SEEK_TO_SAMPLE)) 
            continue;

        this->sendClients(dec, buffer, CHUNK, clients);

        mp3dec_ex_close(&dec);
    }
}
void Audio::sendClients(mp3dec_ex_t& dec, int16_t* buffer, int size, unordered_map<int, Clients::Client*>& clients) {
    while (true) {
      size_t samples = mp3dec_ex_read(&dec, buffer, size);
      if (samples == 0) 
          break;
      vector<uint8_t> frame((uint8_t*)buffer, (uint8_t*)buffer + samples * sizeof(int16_t));

      for (auto &[fd, cl] : clients) {
           lock_guard<std::mutex> lock(cl->cm);
           cl->q.push(frame);
           cl->write = true;
      }
   }
}
