#!/usr/bin/env python3
"""
Convert make build output (make -nw) to compilation database (compile_commands.json)

Usage:
    python make_to_compiledb.py < make_output.txt > compile_commands.json
    or
    make -nw | python make_to_compiledb.py > compile_commands.json
"""

import sys
import re
import json
import os
from pathlib import Path


def extract_directory_from_make_output(line):
    """Extract directory from make 'Entering directory' messages"""
    match = re.search(r"make: Entering directory [`']([^'`]+)[`']", line)
    if match:
        return match.group(1)
    return None


def parse_compiler_command(command_line, current_directory):
    """Parse a compiler command line and extract relevant information"""
    # Split the command line into parts
    parts = command_line.strip().split()
    
    if not parts:
        return None
    
    compiler = parts[0]
    
    # Only process compiler commands (gcc, clang, etc.)
    if not any(comp in compiler.lower() for comp in ['gcc', 'clang', 'cc']):
        return None
    
    # Find input file (source file being compiled)
    source_file = None
    output_file = None
    has_compile_flag = False
    has_preprocess_flag = False
    
    i = 1
    while i < len(parts):
        arg = parts[i]
        
        # Check for compilation/preprocessing flags
        if arg == '-c':
            has_compile_flag = True
            i += 1
        elif arg == '-E':
            has_preprocess_flag = True
            i += 1
        # Look for -c flag followed by source file
        elif arg == '-c' and i + 1 < len(parts):
            has_compile_flag = True
            source_file = parts[i + 1]
            i += 2
        # Look for -o flag followed by output file
        elif arg == '-o' and i + 1 < len(parts):
            output_file = parts[i + 1]
            i += 2
        else:
            # Check if this argument looks like a source file
            if (arg.endswith(('.c', '.cpp', '.cc', '.cxx', '.C', '.s', '.S', '.asm')) and 
                not arg.startswith('-') and source_file is None):
                source_file = arg
            i += 1
    
    # We want commands that either compile (-c flag) or preprocess (-E flag) source files
    if (has_compile_flag or has_preprocess_flag) and source_file:
        return {
            "directory": current_directory,
            "file": source_file,  # Keep the original path as shown in expected output
            "command": command_line.strip()
        }
    
    return None


def parse_make_output(make_output):
    """Parse make output and extract compilation commands"""
    lines = make_output.strip().split('\n')
    compilation_entries = []
    current_directory = os.getcwd()  # Default directory
    
    for line in lines:
        line = line.strip()
        
        # Skip empty lines and comment lines
        if not line or line.startswith('#'):
            continue
        
        # Check for directory changes
        new_dir = extract_directory_from_make_output(line)
        if new_dir:
            current_directory = new_dir
            continue
        
        # Skip printf statements and other non-compiler commands
        if line.startswith('printf ') or 'printf ' in line:
            continue
        
        # Skip make leaving directory messages
        if 'make: Leaving directory' in line:
            continue
        
        # Try to parse as a compiler command
        entry = parse_compiler_command(line, current_directory)
        if entry:
            compilation_entries.append(entry)
    
    return compilation_entries


def main():
    """Main function to convert make output to compilation database"""
    if len(sys.argv) > 1:
        # Read from file if provided as argument
        with open(sys.argv[1], 'r') as f:
            make_output = f.read()
    else:
        # Read from stdin
        make_output = sys.stdin.read()
    
    # Parse the make output
    compilation_entries = parse_make_output(make_output)
    
    # Output as JSON
    json.dump(compilation_entries, sys.stdout, indent=2)
    print()  # Add final newline


if __name__ == "__main__":
    main()


# Example usage and test function
def test_with_example():
    """Test function with the provided example"""
    example_make_output = """make: Entering directory /Users/hariprasadkv/Documents/Projects/laser/kyber-test/stm-projects/test_h7/h7_miniblink'
#printf " CC miniblink.c\\n"
arm-none-eabi-gcc -Os -std=c99 -ggdb3 -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -Wextra -Wshadow -Wimplicit-function-declaration -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes -fno-common -ffunction-sections -fdata-sections -MD -Wall -Wundef -DSTM32H7 -DSTM32H7XXXI -DSTM32H753ZI -I../libopencm3/include -o miniblink.o -c miniblink.c
printf " GENLNK STM32H753ZI\\n"
arm-none-eabi-gcc -E -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DSTM32H7 -DSTM32H7XXXI -DSTM32H753ZI -D_RAM=512K -D_RAM2=128K -D_RAM3=128K -D_RAM4=32K -D_RAM5=64K -D_CCM=128K -D_ROM=1M -D_ROM2=1M -D_ROM_OFF=0x08000000 -D_ROM2_OFF=0x08100000 -D_RAM_OFF=0x24000000 -D_RAM2_OFF=0x30000000 -D_RAM3_OFF=0x30020000 -D_RAM4_OFF=0x30040000 -D_RAM5_OFF=0x38000000 -D_CCM_OFF=0x20000000 -P -E ../libopencm3/ld/linker.ld.S -o generated.STM32H753ZI.ld
#printf " LD miniblink.elf\\n"
arm-none-eabi-gcc --static -nostartfiles -Tgenerated.STM32H753ZI.ld -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -ggdb3 -Wl,-Map=miniblink.map -Wl,--cref -Wl,--gc-sections -L../libopencm3/lib miniblink.o -lopencm3_stm32h7 -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group -o miniblink.elf
make: Leaving directory /Users/hariprasadkv/Documents/Projects/laser/kyber-test/stm-projects/test_h7/h7_miniblink'"""
    
    entries = parse_make_output(example_make_output)
    print("Test output:")
    print(json.dumps(entries, indent=2))


# Uncomment the line below to run the test
# test_with_example()
