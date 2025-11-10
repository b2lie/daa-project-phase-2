import random

# number of digits in each number
digits = 100  

# two large random integers
a = random.randint(10**(digits-1), 10**digits - 1)
b = random.randint(10**(digits-1), 10**digits - 1)

with open("karatsubaMult.txt", "w") as f:
    f.write(f"{a}\n{b}\n")