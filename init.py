#!/usr/bin/env python3


import os, sys
from enum import StrEnum


class EPuzzleType(StrEnum):
    CODEWARS = "codewars"

class PuzzleInitiator:
    def __init__(self, puzzle_type: EPuzzleType):
        self.puzzle_type: EPuzzleType = puzzle_type
        self.name: str = ""
        self.slug: str = ""
        self.level: str = ""

    def slugify(self, name: str) -> str:
        return name.lower().replace(" ", "_")

class CodewarsPuzzleInitiator(PuzzleInitiator):
    def __init__(self):
        super().__init__(EPuzzleType.CODEWARS)

    def initiate(self):
        print(f"Initializing {self.puzzle_type} puzzle...")

        name = input("Enter puzzle name: ")
        slug = self.slugify(name)
        level = input("Enter puzzle level: ")

        self.name: str = name
        self.slug: str = slug
        self.level: str = level

        try:
            os.makedirs(f"codewars/{level}/{slug}-{level}")
        except FileExistsError:
            print(f"Directory already exists: codewars/{level}/{slug}-{level}")
            return


def main():
    if len(sys.argv) != 2:
        print("Usage: puzzle <puzzle_type>")
        sys.exit(1)

    match sys.argv[1]:
        case "codewars":
            initiator = CodewarsPuzzleInitiator()
        case _:
            print(f"Unknown puzzle type: {sys.argv[1]}")
            sys.exit(1)
    initiator.initiate()


if __name__ == "__main__":
    main()
