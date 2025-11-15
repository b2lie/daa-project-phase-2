import os
import random

# make sure the folder exists
os.makedirs("karatsubas_inputs", exist_ok=True)

# different digit sizes to test
sizes = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]

for digits in sizes:
    a = random.randint(10**(digits - 1), 10**digits - 1)
    b = random.randint(10**(digits - 1), 10**digits - 1)

    filename = f"karatsubas_inputs/karatsuba{digits}.txt"
    with open(filename, "w") as f:
        f.write(f"{a}\n{b}\n")
    print(f"✅ generated {filename} with {digits}-digit numbers")