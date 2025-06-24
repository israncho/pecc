from typing import List, Callable
from pathlib import Path
from os import makedirs
from os.path import dirname
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


def is_number(str_test: str, transform_func: Callable) -> bool:
    '''Checks if a given string can be converted to a number using
    the specified transformation function.'''
    try:
        transform_func(str_test)
        return True
    except ValueError:
        return False


def to_number(num_as_str: str) -> float | int:
    '''Converts a string to either an integer or a float. '''
    if is_number(num_as_str, int):
        return int(num_as_str)
    if is_number(num_as_str, float):
        return float(num_as_str)
    raise ValueError(f"'{num_as_str}' is not a valid number")