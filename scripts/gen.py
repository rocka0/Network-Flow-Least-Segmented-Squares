from sys import argv
from secrets import randbelow


def genInt(a, b):
    return randbelow(b - a + 1) + a


POINT_LIM = 1000


def genPoint():
    x = genInt(-POINT_LIM, POINT_LIM)
    y = genInt(-POINT_LIM, POINT_LIM)
    return (x, y)


def main():
    if len(argv) != 2:
        print("Usage: python3 gen.py <n>")
        exit(-1)

    # Get the number of points
    n = int(argv[1])
    print(n)

    for _ in range(n):
        # Generate a point
        x, y = genPoint()
        print(x, y)

    # Generate the segment cost
    print(genInt(1, POINT_LIM*POINT_LIM))


if __name__ == "__main__":
    main()
