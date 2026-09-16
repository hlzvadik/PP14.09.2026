import subprocess
import re
import statistics
import matplotlib.pyplot as plt
import os
import sys

EXECUTABLE = "./a.out"
THREAD_COUNTS = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]
NUM_MEASUREMENTS = 5

def run_test():
    if not os.path.exists(EXECUTABLE):
        sys.exit(f"Error: Executable {EXECUTABLE} not found.")

    medians = []
    
    for threads in THREAD_COUNTS:
        times = []
        for _ in range(NUM_MEASUREMENTS):
            try:
                res = subprocess.run([EXECUTABLE, str(threads)], capture_output=True, text=True, check=True)
                match = re.search(r"Total time:\s*([0-9.]+)", res.stdout)
                if match:
                    times.append(float(match.group(1)))
            except subprocess.CalledProcessError:
                pass

        median_time = statistics.median(times) if times else 0
        medians.append(median_time)
        print(f"Threads: {threads:<4} | Median time: {median_time:.2f} ms")

    return THREAD_COUNTS, medians

def plot_results(threads, times):
    plt.plot(threads, times, marker='o')
    plt.xscale('log', base=2)
    plt.xticks(threads, threads)
    plt.xlabel('Number of threads')
    plt.ylabel('Time (ms)')
    plt.grid(True, linestyle="--")
    plt.savefig('graph.png')
    plt.show()

if __name__ == "__main__":
    t, m = run_test()
    if any(m):
        plot_results(t, m)
