import os
import shutil

# Source directory
src_dir = "./tester/tests/spec"

# Destination directory
dest_dir = "./tester/tests/res"

# Loop through all files in the source directory
for root, dirs, files in os.walk(src_dir):
    for filename in files:
        # Get the full path of the source file
        src_file = os.path.join(root, filename)

        # Get the relative path of the file
        rel_path = os.path.relpath(src_file, start=src_dir)

        # Create the destination directory if it does not exist
        dest_path = os.path.join(dest_dir, rel_path)
        os.makedirs(os.path.dirname(dest_path), exist_ok=True)

        # Create the empty file in the destination directory
        open(dest_path, 'w').close()
