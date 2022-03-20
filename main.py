N_ROWS = 3
N_COLS = 4


def main():
    matrix = []
    for i in range(3):
        row = []
        for j in range(4):
            num = i * N_COLS + j;
            row.append(num)
        matrix.append(row)

    for row in matrix:
        print("[ ", end='')
        for num in row:
            print(num, end=' ')
        print("]")


if __name__ == "__main__":
    main()
