import pyaudio
import socket

CHUNK = 4096 
CHANNELS = 2
RATE = 44100
BYTES_PER_SAMPLE = 2 
FRAME_SIZE = CHUNK * CHANNELS * BYTES_PER_SAMPLE

def audio_thread(fd) -> None:
    print("test")
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
            buffer.extend(data)

            #print(f"data: {data}")

            #while len(buffer) >= FRAME_SIZE:
                #frame = buffer[:FRAME_SIZE]
            stream.write(data)
                #buffer = buffer[FRAME_SIZE:]
        except:
            continue  

    stream.stop_stream()
    stream.close()
    p.terminate()