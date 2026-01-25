import tkinter as tk
from tkinter import simpledialog
from tkinter import filedialog
from SongList import SongList
from queue import Queue
import threading
import socket

class RadioClient(tk.Tk):
    def __init__(self, fd) -> None:
        super().__init__()
        self.fd = fd 
        self.title("Radio client")
        self.geometry("400x300")
        self.playlist_box = tk.Listbox(self)
        self.playlist_box.pack(fill=tk.BOTH, expand=True)
        self.entry = tk.Entry(self)
        self.entry.pack(fill=tk.X)
        self.song_list: SongList = SongList()
        #self.fd.setblocking(False)
        self.cmd_queue = Queue()
        
        btn_f = tk.Frame(self)
        btn_f.pack(fill=tk.X)

        '''tk.Button(btn_f, text = "UPLOAD", command = self.upload_song).pack(side=tk.LEFT)
        tk.Button(btn_f, text = "REMOVE", command = self.remove_song).pack(side=tk.LEFT)
        tk.Button(btn_f, text = "NEXT", command=self.next_song).pack(side=tk.LEFT)
        tk.Button(btn_f, text = "LIST", command=self.list_song).pack(side=tk.LEFT)'''

        tk.Button(btn_f, text = "UPLOAD", command = self.upload_song).pack(side=tk.LEFT)
        tk.Button(btn_f, text = "REMOVE", command = lambda: self.cmd_queue.put("REMOVE")).pack(side=tk.LEFT)
        tk.Button(btn_f, text = "NEXT", command=lambda: self.cmd_queue.put("NEXT")).pack(side=tk.LEFT)
        tk.Button(btn_f, text = "LIST", command=lambda: self.cmd_queue.put("LIST")).pack(side=tk.LEFT)

        self.after(100, self.list)
        threading.Thread(target=self.cmd_thread, daemon=True).start()

    def send_command(self, cmd) -> None:
        try:
            self.fd.sendall((cmd + "\n").encode())
        except:
            print("Command failure")
    def upload_song(self) -> None:
        path = filedialog.askopenfilename(title="Wybierz plik audio", filetypes=[("MP3 files", "*.mp3"), ("All files", "*.*")])
        song = self.entry.get()
        if not path:
            return 
        filename = path.split('/')[-1]

        data = None

        with open(path, "rb") as file:
            data = file.read()
        
        size = len(data)

        header = f"UPLOAD {filename} {size}\n".encode()
        try:
            self.fd.sendall(header)
        except:
            print("Header failure")
            return

        try:
            self.fd.sendall(data)
            self.entry.delete(0, tk.END)
            print("Uploaded")
        except:
            print("Upload failure")

    def remove_song(self) -> None:
        self.send_command(f"REMOVE")

    def next_song(self) -> None:
        self.send_command("NEXT")
    def list_song(self) -> None:
        self.song_list.clear()
        self.send_command("LIST")
    def list(self) -> None:
        songs = self.song_list.show()
        self.playlist_box.delete(0, tk.END)
        for s in songs:
            self.playlist_box.insert(tk.END, s)
    def cmd_thread(self) -> None:
        while True:
            cmd = self.cmd_queue.get()  
            if not cmd:
                continue
            try:
                self.fd.sendall((cmd + "\n").encode())
                print(cmd)
            except BlockingIOError:
                import time
                time.sleep(0.01)
                self.cmd_queue.put(cmd)
            except Exception as e:
                print("Command send error:", e)

    def text_thread(self) -> None:
        buffer = ""
        while True:
            try:
                data = self.fd.recv(4096).decode()
                if not data:
                    continue
                buffer += data
                while "\n" in buffer:
                    line, buffer = buffer.split("\n", 1)
                    if line.endswith("PLAYLIST"):
                        song_name = line[:-8]
                        self.song_list.add(song_name)
                        self.after(0, self.list)
            except:
                continue
