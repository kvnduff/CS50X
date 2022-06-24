# Prints mario blocks

from cs50 import get_int

height = 0

# Prompt user for height of pyramid
while True:
    height = get_int("Height: ")
    if height > 0 and height <= 8:
        break

# Print pyramid of specified height
for h in range(height):
    # Determine left, center, right pyramid components
    left = " " * (height - h - 1) + ("#" * (h + 1))
    center = " " * 2
    right = "#" * (h + 1)
    # Print pyramid
    print(f"{left}{center}{right}")