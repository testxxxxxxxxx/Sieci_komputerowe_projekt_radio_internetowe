from audioThread import audio_thread
from RadioClient import RadioClient
import socket
import threading
import struct 

SERVER_HOST = "127.0.0.1"
SERVER_PORT = 12345

def main() -> None:

    fd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    fd.connect((SERVER_HOST, SERVER_PORT))

    threading.Thread(target=audio_thread, args=(fd, ), daemon=True).start()

    app: RadioClient = RadioClient(fd)
    app.mainloop()

if __name__ == "__main__":
    main()