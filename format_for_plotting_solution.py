from sys import argv
from collections import deque
from aux import read_file, write_file


def give_format_for_plotting_solution(list_of_strs: deque[str]) -> deque[str]:
    '''Gives format to the list of strings to generate a file with
    format to use GNUPlot from a TSPLIB file.'''
    str_ = list_of_strs.popleft()

    while str_ != 'NODE_COORD_SECTION':
        str_ = list_of_strs.popleft()

    if list_of_strs[-1] == 'EOF':
        list_of_strs.pop()

    n = len(list_of_strs)
    for _ in range(n):
        str_ = list_of_strs.popleft()
        tokens = str_.split()
        str_ = ' '.join(tokens[1:]) + '\n'
        list_of_strs.append(str_)

    list_of_strs.append(list_of_strs[0]) 

    return list_of_strs


if __name__ == "__main__":
    input_file = argv[1]
    output_file = argv[2]
    list_of_strs = read_file(input_file)
    write_file(output_file, give_format_for_plotting_solution(list_of_strs))