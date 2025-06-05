from pathlib import Path
from typing import List
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


def list_files(path: str, pattern: str) -> List[str]:
    path = Path(path)
    return [str(file_path) for file_path in path.glob(pattern)]


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