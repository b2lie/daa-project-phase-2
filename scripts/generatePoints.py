import random

with open("closest100.txt", "w") as f:
    for _ in range(100):
        x = random.randint(0,800)
        y = random.randint(0,600)
        f.write(f"{x} {y}\n")