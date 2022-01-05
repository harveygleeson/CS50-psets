from cs50 import get_int

# get input
height = 9
while (height > 8 or height < 1):  # input parameters
    height = get_int("Height: ")

for i in range(height):
    print(" " * (height - 1 - i), end="")  # first spaces
    print("#" * (i + 1), end="  ")  # first blocks and spaces in between
    print("#" * (i + 1), end="\n")  # second blocks
   