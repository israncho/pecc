from os import makedirs
from os.path import dirname
from sys import argv
from traceback import print_exc
from collections import deque

def read_file(file_path: str) -> deque[str]:
    '''Reads a file using utf-8 encongind
    and returns its contents as a list of lines.'''

    list_of_lines = deque()

    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            for line in file:
                list_of_lines.append(line.strip())

        print(f"Successful reading of file '{file_path}'")
    except FileNotFoundError as e:
        print(f"The file '{file_path}' was not found:\n{e}")
    except PermissionError as e:
        print(f"You do not have permission to open this file:\n{e}")
    except Exception as e: # pylint: disable=broad-exception-caught
        print(f"An error occurred: {e}")
        print_exc()

    return list_of_lines


def write_file(file_path: str, lines_of_the_file: deque[str], mode: str = 'w') -> None:
    '''Writes a list of lines to a file.'''
    try:

        dirpath = dirname(file_path)
        if dirpath != '':
            makedirs(dirpath, exist_ok=True)
        with open(file_path, mode, encoding='utf-8') as file:
            for line in lines_of_the_file:
                file.write(line)

        print(f"Written in the file '{file_path}'")
    except PermissionError as e:
        print(f"You do not have permission to open this file:\n{e}")
    except Exception as e: # pylint: disable=broad-exception-caught
        print(f"An error occurred: {e}")
        print_exc()


def give_format(list_of_strs: deque[str]) -> deque[str]:
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
    write_file(output_file, give_format(list_of_strs))