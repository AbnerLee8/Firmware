# ________________________________________________________________________________________________
#
# This file is to be used for Glorious Left MCU project.
#
# It generates a .bin file from a .hex file.
#
# Input: hex_file='file_1.hex'
#
#   The .hex file for Left MCU Glorious project, with its Application Start Address is 0xC000.
#
# What the script does:
#
#   Creates a minimal .bin file, only using the code, so it trims the unused program space after 
#   the last code line.
#   It also removes the first 0xC000 bytes from the resulting .bin file, as these will be all 
#   zeros (because the Application Start Address is 0xC000.
#
# To use: Open a command window in this directory, and type "py hex_to_bin.py"
#           (with some Python installs you might have to type "py3 hex_to_bin.py" or similar)
#
#   At the end of the script below, set the values for:
#
#   hex_file        The name of the hex file to work with (this file should be in the same 
#                   directory as this script)
#   start_address   Leave this at 0xC000.
#   max_address     The maximum address to include. Leave this at 0x100000 for SAMD51N20A.
#
# Outputs:
#
#   bin_file        The desired name of the resulting .bin file. This is the .bin file to be 
#                   sent by the Core app to the Left MCU.
#   metadata_file   JSON metadata, indicating the start/end addresses of all code converted
#                   from .hex to .bin (for info. purposes only)
# ________________________________________________________________________________________________

#import os
 
# 原始文件名
#old_name = 'left_samd51n20a_bootloader.X.production.hex'
# 新文件名
#new_name = 'new_file.txt'
 
# 获取当前目录的绝对路径
#current_directory = os.getcwd()
# 完整的原始文件路径
#old_path = os.path.join(current_directory, old_name)
# 完整的新的文件路径
#new_path = os.path.join(current_directory, new_name)
 
# 重命名文件
#os.rename(old_path, new_path)

from intelhex import IntelHex
import json

def hex_to_sparse_bin_with_metadata(hex_file, bin_file, metadata_file, start_address=0x0000, max_address=0x100000):
    """
    Converts HEX file to sparse BIN file with metadata, skipping addresses before start_address.
    
    Args:
    - hex_file (str): Input HEX file path.
    - bin_file (str): Output BIN file path.
    - metadata_file (str): Output metadata JSON file path.
    - start_address (int): The application start address to skip leading zeros.
    - max_address (int): The maximum address to include.
    """
    ih = IntelHex(hex_file)
    
    metadata = []  # To store metadata of valid segments

    with open(bin_file, 'wb') as f:
        for start_addr, end_addr in ih.segments():
            # Skip segments below the start_address
            if end_addr <= start_address:
                print(f"Skipping segment {start_addr}-{end_addr} (below start address {start_address}).")
                continue

            # Filter out segments above the max_address
            if start_addr > max_address:
                print(f"Skipping segment {start_addr}-{end_addr} (above threshold {max_address}).")
                continue

            # Adjust start address if the segment partially overlaps the start_address
            segment_start = max(start_addr, start_address)
            length = end_addr - segment_start
            segment_data = ih.tobinarray(start=segment_start, end=end_addr)
            
            # Write the valid segment to the BIN file
            f.seek(segment_start - start_address)  # Align to new starting address in the BIN file
            f.write(segment_data)

            metadata.append({
                "start_address": segment_start,
                "length": length
            })

    # Write metadata to a JSON file
    with open(metadata_file, 'w') as meta_f:
        json.dump(metadata, meta_f, indent=4)

    print(f"Sparse BIN file created from {hex_file}.")
    print(f"Metadata saved to {metadata_file}.")

# Usage
hex_to_sparse_bin_with_metadata(
    hex_file='file_1.hex', 
    bin_file='file_1.bin', 
    metadata_file='file_1_metadata_2.json', 
    start_address=0xC000,  # Application start address
    max_address=0x100000   # Maximum valid address
)
