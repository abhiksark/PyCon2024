# file: gil_bound.py
import time
from concurrent.futures import ThreadPoolExecutor
import math

def calculate_gil_bound(start, end):
    result = 0.0
    for i in range(start, end):
        result += math.sin(i)
        result += math.cos(i) 
        result += math.sin(i)


    return result

def main():
    start_time = time.time()
    
    total_iterations = 100_000_000
    num_threads = 4
    chunk_size = total_iterations // num_threads
    
    with ThreadPoolExecutor(max_workers=num_threads) as executor:
        futures = []
        for i in range(num_threads):
            start = i * chunk_size
            end = start + chunk_size if i < num_threads - 1 else total_iterations
            futures.append(executor.submit(calculate_gil_bound, start, end))
    
        total_result = sum(future.result() for future in futures)
    
    end_time = time.time()
    print(f"Python (GIL-bound) result:")
    print(f"Total result: {total_result}")
    print(f"Time taken: {end_time - start_time:.2f} seconds")

if __name__ == "__main__":
    main()