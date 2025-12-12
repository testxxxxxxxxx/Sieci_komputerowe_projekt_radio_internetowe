import pyaudio
import socket

CHUNK = 4096

def audio_thread(fd) -> None:
    p = pyaudio.PyAudio()
    stream = p.open(format=pyaudio.paInt16, channels=2, rate=44100, output=True)
    
    while True:
        try:
            data = fd.recv(CHUNK*2*2)
            if not data:
                break
            stream.write(data)
        except:
            break
    stream.start_stream()
    stream.close()
    p.terminate()
