import tkinter as tk
from tkinter import simpledialog
from tkinter import filedialog
from SongList import SongList
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
        
        btn_f = tk.Frame(self)
        btn_f.pack(fill=tk.X)

        tk.Button(btn_f, text = "UPLOAD", command = self.upload_song).pack(side=tk.LEFT)
        tk.Button(btn_f, text = "REMOVE", command = self.remove_song).pack(side=tk.LEFT)
        tk.Button(btn_f, text = "NEXT", command=self.next_song).pack(side=tk.LEFT)

        #self.after(100, self.list)

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
        song = self.entry.get()
        if song:
            self.send_command(f"REMOVE {song}")
            self.entry.delete(0, tk.END)

    def next_song(self) -> None:
        self.send_command("NEXT")
    def list(self) -> None:
        songs = self.song_queue.show()
        self.playlist_box.delete(0, tk.END)
        for s in songs:
            self.playlist_box.insert(tk.END, s)

    def text_thread(self) -> None:
        while True:
            try:
                data = self.fd.recv(4096).decode()
                if "PLAYLIST\n" not in data:
                    continue
                self.song_list.add(data[:len(data) - 9])
            except:
                continue