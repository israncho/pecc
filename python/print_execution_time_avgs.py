from typing import List
from sys import argv
from collections import deque
from aux import read_file, to_number, list_files


def print_avgs(files: List[str]) -> None:
    list_of_deques: deque[deque[str]] = deque()

    for file in files:
        list_of_deques.append(read_file(file))
    
    n: int = len(list_of_deques[0])

    data: List = []

    i = 0
    avg_sequential_time = 1.0
    while list_of_deques:
        i += 1
        label: str = 'n_threads: ' + str(i)
        list_of_numbers: List[float] = list(map(to_number, list_of_deques.popleft()))

        if data == []:
            avg_sequential_time = sum(list_of_numbers) / n
            data.append((label, avg_sequential_time, 'speedup:', 1.0))
        else:
            avg_time = sum(list_of_numbers) / n
            speed_up = avg_sequential_time / avg_time 
            data.append((label, avg_time , 'speedup:', speed_up))

    for elem in data:
        print(elem)


if __name__ == "__main__":
    input_path = argv[1]
    pattern = argv[2]
    print('input_path: ', input_path)
    print('pattern: ', pattern)
    list_of_files = sorted(list_files(input_path, pattern))
    print_avgs(list_of_files)
