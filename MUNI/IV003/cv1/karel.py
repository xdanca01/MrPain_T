#!/usr/bin/env python3

import sys


class City:
    _dir_names = "nesw"
    _dirs = [(-1, 0), (0, 1), (1, 0), (0, -1)]

    def __init__(self, f):
        # increase size for sentinel walls
        self.rows, self.cols = map(lambda x: int(x) + 2, f.readline().split())

        pos_dir = f.readline().split()
        self.y, self.x = map(lambda x: int(x) + 1, pos_dir[:2])
        self.dir = City._dir_names.find(pos_dir[2])
        if self.dir == -1:
            raise RuntimeError("Bad direction specification")

        self.city = [[0] * self.cols for _ in range(self.rows)]

        # sentinel walls
        for row in range(self.rows):
            self.city[row][0] = self.city[row][self.cols - 1] = "#"
        for col in range(self.cols):
            self.city[0][col] = self.city[self.rows - 1][col] = "#"

        for row, line in enumerate(f):
            for col, place in enumerate(line.split()):
                self.city[row + 1][col + 1] = \
                    "#" if place == "#" else int(place)

        if self.city[self.y][self.x] == "#":
            raise RuntimeError("The starting position of Karel is a wall")

    def step(self):
        dy, dx = City._dirs[self.dir]
        self.y += dy
        self.x += dx
        if self.city[self.y][self.x] == "#":
            raise RuntimeError("Karel crashed into a wall")

    def turn(self, course):
        self.dir = (self.dir + course) % 4

    def wall(self):
        dy, dx = City._dirs[self.dir]
        return self.city[self.y + dy][self.x + dx] == "#"

    def mark(self):
        return self.city[self.y][self.x] > 0

    def change_marks(self, diff):
        if self.city[self.y][self.x] == 0 and diff < 0:
            raise RuntimeError("No marks here: Karel is confused")

        self.city[self.y][self.x] += diff

    def draw(self):
        def line():
            print("\n+" + "---+" * (self.cols - 2))

        line()
        for row in range(1, self.rows - 1):
            print("|", end="")
            for col in range(1, self.cols - 1):
                place = self.city[row][col]
                print("###" if place == "#" else "{}{:2d}".format(
                    "^>v<"[self.dir] if (row, col) == (self.y, self.x)
                    else " ",
                    place), end="|")
            line()


class Program:
    _tokens_expected = {"IFWALL": 3, "IFMARK": 3, "DEFINE": 2, "RUN": 2}
    _simple_commands = {"SKIP": lambda x: None,
                        "STEP": City.step,
                        "LEFT": lambda city: city.turn(-1),
                        "RIGHT": lambda city: city.turn(+1),
                        "TAKE": lambda city: city.change_marks(-1),
                        "PUT": lambda city: city.change_marks(+1)}
    _if_commands = {"IFWALL": City.wall,
                    "IFMARK": City.mark}

    def __init__(self, f, city, pause):
        self.main = self.current = self.stack = None
        self.city = city
        self.pause = pause
        self.proc = {}
        for lineno, line in enumerate(f, 1):
            self._parse_line(lineno, line)

    def _parse_line(self, lineno, line):
        # discard comments
        pos = line.find('#')
        if pos != -1:
            line = line[:pos]

        tokens = line.split()
        if not tokens:  # empty line
            return

        if len(tokens) != Program._tokens_expected.get(tokens[0], 1):
            raise RuntimeError(
                "line {}: wrong number of tokens".format(lineno))

        if self.current is None:
            self._parse_decl(lineno, tokens)
        else:
            self._parse_command(lineno, tokens)

    def _parse_decl(self, lineno, decl):
        if decl[0] == "DEFINE":
            if decl[1] in self.proc:
                raise RuntimeError(
                    "line {}: multiple definitions of {}".format(
                        lineno, decl[1]))

            self.current = []
            self.proc[decl[1]] = self.current
        elif decl[0] == "RUN":
            if self.main:
                raise RuntimeError(
                    "line {}: multiple RUN declarations".format(lineno))

            self.main = decl[1]
        else:
            raise RuntimeError(
                "line {}: unknown declaration {}".format(lineno, decl[0]))

    def _parse_command(self, lineno, command):
        if command[0] == "END":
            if not self.current:
                raise RuntimeError(
                    "line {}: procedure cannot be empty".format(lineno))

            self.current = None
        elif command[0] in Program._if_commands:
            self.current.append(tuple(command))
        else:
            self.current.append(command[0])

    def run(self):
        def display(command):
            return " ".join(command) if isinstance(command, tuple) else command

        if self.main is None:
            raise RuntimeError("no RUN declaration")
        if self.main not in self.proc:
            raise RuntimeError("invalid RUN declaration: " + self.main)

        # use explicit call stack to avoid Python stack overflow
        self.stack = [[self.main, 0]]

        self._draw()

        while self.stack:
            top = self.stack[-1]
            proc = self.proc[top[0]]
            command = proc[top[1]]
            print("Executing: {} ({}:{}, stack size: {})".format(
                display(command), *top, len(self.stack)))

            top[1] += 1  # advance program counter

            # optimisation: tail recursion (pop early)
            if top[1] == len(proc):
                self.stack.pop()

            self._exec(command)
            self._draw()

    def _exec(self, command):
        if command in Program._simple_commands:
            Program._simple_commands[command](self.city)
        elif isinstance(command, tuple):
            self._exec(
                command[1 if Program._if_commands[command[0]](self.city)
                        else 2])
        else:  # procedure call
            if command not in self.proc:
                raise RuntimeError("procedure not found: " + command)

            self.stack.append([command, 0])

    def _draw(self):
        self.city.draw()
        if self.pause:
            input()
        else:
            print()


def main():
    if len(sys.argv) < 3:
        print("Usage: {} city program [-p]".format(sys.argv[0]))
        print("\t-p: pause and wait for enter after each step")
        return

    pause = len(sys.argv) > 3 and sys.argv[3] == "-p"

    try:
        with open(sys.argv[1]) as f_city, open(sys.argv[2]) as f_prog:
            city = City(f_city)
            prog = Program(f_prog, city, pause)

        prog.run()
    except RuntimeError as err:
        print(err)


if __name__ == "__main__":
    main()
