from typing import List

class SongList:

    def __init__(self) -> None:
        self.__songs: List[str] = []

    def add(self, song: str) -> None:
        self.__songs.append(song)
    def show(self) -> List[str]:
        return self.__songs
    def removeAll(self) -> None:
        self.__songs = []