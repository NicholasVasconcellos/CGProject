#!/usr/bin/env python3
import os
import shutil
import sys

def move_log_files():
    # Define source and destination directories
    source_dir = "images"
    dest_dir = "logImages"
    
    # Check if source directory exists
    if not os.path.isdir(source_dir):
        print(f"Error: Source directory '{source_dir}' does not exist.")
        sys.exit(1)
    
    # Create destination directory if it doesn't exist
    if not os.path.exists(dest_dir):
        try:
            os.makedirs(dest_dir)
            print(f"Created destination directory: {dest_dir}")
        except OSError as e:
            print(f"Error: Could not create destination directory: {e}")
            sys.exit(1)
    
    # Counter for moved files
    moved_count = 0
    
    # Iterate through files in source directory
    for filename in os.listdir(source_dir):
        # Check if file contains "_log" in its name
        if "_log" in filename:
            source_path = os.path.join(source_dir, filename)
            dest_path = os.path.join(dest_dir, filename)
            
            # Skip if not a file
            if not os.path.isfile(source_path):
                continue
                
            try:
                # Move the file
                shutil.move(source_path, dest_path)
                print(f"Moved: {filename}")
                moved_count += 1
            except Exception as e:
                print(f"Error moving {filename}: {e}")
    
    print(f"\nTotal files moved: {moved_count}")

if __name__ == "__main__":
    move_log_files()