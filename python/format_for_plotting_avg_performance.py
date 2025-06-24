from typing import List
from sys import argv
from collections import deque
from aux import read_file, write_file, list_files, to_number

def give_format_for_plotting_avg_performance(files_for_bests: List[str], files_for_avgs: List[str]) -> deque[str]:
    output: deque[str] = deque()
    list_with_bests: list[float] = None 
    list_with_avgs: list[float] = None 
    n_executions: int = 0

    for file_b in files_for_bests:
        for line in read_file(file_b):
            n_executions += 1
            list_with_data: list[float] = list(map(to_number, line.split(','))) 
            if list_with_bests is None:
                list_with_bests = list_with_data
                continue
            for i, e in enumerate(list_with_data):
                list_with_bests[i] += e

    print('Number of executions:', n_executions)

    deque_with_bests: deque[float] = deque(map(lambda x : x / n_executions, list_with_bests))

    n_executions = 0 
    for file_avg in files_for_avgs:
        for line in read_file(file_avg):
            n_executions += 1
            list_with_data: list[float] = list(map(to_number, line.split(','))) 
            if list_with_avgs is None:
                list_with_avgs = list_with_data
                continue
            for i, e in enumerate(list_with_data):
                list_with_avgs[i] += e

    deque_with_avgs: deque[float] = deque(map(lambda x : x / n_executions, list_with_avgs))

    while deque_with_avgs and deque_with_bests:
        new_line: str = str(deque_with_avgs.popleft()) + ' ' + str(deque_with_bests.popleft()) + '\n'
        output.append(new_line)

    return output


if __name__ == "__main__":
    input_path = argv[1]
    output_file = argv[2]
    print('input_path: ', input_path)
    print('output_file: ', output_file)
    list_of_files_bests = sorted(list_files(input_path, '*.data_bests'))
    list_of_files_avgs = sorted(list_files(input_path, '*.data_avgs'))

    write_file(output_file, give_format_for_plotting_avg_performance(list_of_files_bests, list_of_files_avgs)) 