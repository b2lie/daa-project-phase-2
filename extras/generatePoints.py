import os
import random

# make sure the folder exists
os.makedirs("pairs_inputs", exist_ok=True)

sizes = [100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]

for n in sizes:
    filename = f"pairs_inputs/closest{n}.txt"
    with open(filename, "w") as f:
        for _ in range(n):
            x = random.randint(0, 1100)
            y = random.randint(0, 600)
            f.write(f"{x} {y}\n")
    print(f"✅ generated {filename} with {n} points")