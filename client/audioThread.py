import pyaudio
import socket

CHUNK = 4096 
CHANNELS = 2
RATE = 44100
BYTES_PER_SAMPLE = 2 
FRAME_SIZE = CHUNK * CHANNELS * BYTES_PER_SAMPLE

def audio_thread(fd) -> None:
    p = pyaudio.PyAudio()
    stream = p.open(format=pyaudio.paInt16,
                    channels=CHANNELS,
                    rate=RATE,
                    output=True)

    buffer = bytearray()

    while True:
        try:
            data = fd.recv(FRAME_SIZE)
            if not data:
                continue
            if b"FLUSH\n" in data:
                print("FLUSH")

                parts = data.split(b"FLUSH\n", 1)

                buffer.clear()
                stream.stop_stream()
                stream.start_stream()

                if len(parts) > 1 and parts[1]:
                    stream.write(parts[1])   

                continue

            #buffer.extend(data)

            stream.write(data)
        except:
            continue 

    stream.stop_stream()
    stream.close()
    p.terminate()