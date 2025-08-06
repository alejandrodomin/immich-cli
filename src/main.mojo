from pathlib.path import cwd
from collections.deque import Deque
from utils import constants


def main():
    curr_path = cwd()

    dirs = Deque(curr_path)
    while dirs:
        dir = dirs.pop()

        for entry in dir.listdir():
            if entry.is_dir():
                dirs.append(entry)
            elif entry.suffix() in video_types:
                print("found accepted type:", entry)

        print("Searching through directory", dir)
