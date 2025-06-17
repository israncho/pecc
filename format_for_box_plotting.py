from typing import List
from sys import argv
from collections import deque
from aux import read_file, write_file, list_files


def give_format(files: List[str]) -> deque[str]:
    output: deque[str] = deque()
    list_of_deques: List[deque[str]] = []

    for file in files:
        list_of_deques.append(read_file(file))
    
    n: int = len(list_of_deques[0])

    tmp: List[str] = []
    for _ in range(n):
        for deque_ in list_of_deques:
            if len(deque_) != 0:
                tmp.append(deque_.popleft().strip())
            else:
                tmp.append('NaN')
        output.append(' '.join(tmp + ['\n']))
        tmp = []

    return output


if __name__ == "__main__":
    input_path = argv[1]
    output_file = argv[2]
    pattern = argv[3]
    print('input_path: ', input_path)
    print('output_file: ', output_file)
    print('pattern: ', pattern)
    list_of_files = sorted(list_files(input_path, pattern))

    write_file(output_file, give_format(list_of_files)) 