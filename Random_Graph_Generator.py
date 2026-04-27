import random
import os

n_values = [1, 2, 5, 8, 10, 12, 15]

def create_random_test_file(n):
    filename = f"n_is_{n}.txt"
    
    if n <= 1:
        with open(filename, 'w') as f:
            f.write(f"{n} 0\n")
        return

    max_safe_edges = n * 2 
    num_edges = random.randint(n - 1, max_safe_edges)
    
    edges = set()
    
    while len(edges) < num_edges:
        u = random.randint(0, n - 1)
        v = random.randint(0, n - 1)
        
        if u != v:
            edge = tuple(sorted((u, v)))
            edges.add(edge)

    with open(filename, 'w') as f:
        f.write(f"{n} {len(edges)}\n")
        for u, v in edges:
            f.write(f"{u} {v}\n")
            
    print(f"Created {filename} with {len(edges)} random edges.")

if __name__ == "__main__":
    print("Generating random input files...")
    for n in n_values:
        create_random_test_file(n)
    print("\nAll files ready.")