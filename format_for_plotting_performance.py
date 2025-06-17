from collections import deque
from sys import argv
from aux import write_file, read_file

def give_format_for_plotting_performance(avgs: str, bests: str) -> deque[str]:
    output: deque[str] = deque()
    list_of_avgs: deque[str] = deque(avgs.strip().split(',')) 
    list_of_bests: deque[str] = deque(bests.strip().split(',')) 
    
    while list_of_bests and list_of_avgs:
        new_line = list_of_avgs.popleft() + ' ' + list_of_bests.popleft() + '\n'
        output.append(new_line)
    
    return output

if __name__ == "__main__":
    input_path = argv[1]
    output_file = argv[2]
    assert(input_path[-4:] == '.tsp')
    avgs_path = input_path + '.data_avgs'
    bests_path = input_path + '.data_bests'

    avgs: str = read_file(avgs_path)[-1]
    bests: str = read_file(bests_path)[-1]

    write_file(output_file, give_format_for_plotting_performance(avgs, bests))