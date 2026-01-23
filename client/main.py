from audioThread import audio_thread
from RadioClient import RadioClient
import socket
import threading
import sys
import struct 

def main() -> None:

    if len(sys.argv) < 2:
        print(f"Usage: python3 {sys.argv[0]} <ip> <port>")
        sys.exit(1)

    SERVER_HOST = sys.argv[1]
    SERVER_PORT = 0

    try:
        SERVER_PORT = int(sys.argv[2])
    except ValueError:
        print("<port> is not a integer")
        sys.exit(1)

    fdAudio = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    fdAudio.connect((SERVER_HOST, SERVER_PORT))
    fdGUI = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    fdGUI.connect((SERVER_HOST, SERVER_PORT))
    fdAudio.setblocking(False)
    #fdGUI.setblocking(False)

    threading.Thread(target=audio_thread, args=(fdAudio, ), daemon=True).start()
 
    app: RadioClient = RadioClient(fdGUI)

    threading.Thread(target=app.text_thread, daemon = True).start()
    app.mainloop()

if __name__ == "__main__":
    main()