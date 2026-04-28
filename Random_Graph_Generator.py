import random
import itertools

n_values = [1, 2, 5, 8, 10, 12, 15, 20, 100, 1000, 10000, 20000]

def create_random_test_file(n):
    filename = f"n_is_{n}.txt"
    
    if n <= 1:
        with open(filename, 'w') as f:
            f.write(f"{n} 0\n")
        return

    max_possible_edges = n * (n - 1) // 2
    

    upper_bound = min(n * 2, max_possible_edges) 
    
    num_edges = random.randint(min(n - 1, max_possible_edges), upper_bound)
    
    all_possible_edges = list(itertools.combinations(range(n), 2))
    
    edges = random.sample(all_possible_edges, num_edges)

    with open(filename, 'w') as f:
        f.write(f"{n} {len(edges)}\n")
        for u, v in edges:
            f.write(f"{u} {v}\n")
            
    print(f"Created {filename} with {len(edges)} unique edges (Max possible: {max_possible_edges}).")

if __name__ == "__main__":
    print("Generating random input files...")
    for n in n_values:
        create_random_test_file(n)
    print("\nAll files ready.")
