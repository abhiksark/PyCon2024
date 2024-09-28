# file: _nogil.py
import pyximport
pyximport.install()

from nogil_calc import calculate_nogil
import time
from concurrent.futures import ThreadPoolExecutor

def main():
    start_time = time.time()
    
    total_iterations = 1000_000_000
    num_threads = 8
    chunk_size = total_iterations // num_threads
    
    with ThreadPoolExecutor(max_workers=num_threads) as executor:
        futures = []
        for i in range(num_threads):
            start = i * chunk_size
            end = start + chunk_size if i < num_threads - 1 else total_iterations
            futures.append(executor.submit(calculate_nogil, start, end))
    
        total_result = sum(future.result() for future in futures)
    
    end_time = time.time()
    print(f"Cython (nogil) result:")
    print(f"Total result: {total_result}")
    print(f"Time taken: {end_time - start_time:.2f} seconds")

if __name__ == "__main__":
    main()