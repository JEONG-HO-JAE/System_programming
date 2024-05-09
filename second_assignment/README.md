# Second Assignment
## Goal
• Write a program that modifies an ELF file’s “rodata” section.  
• First, you need to implement a function to check whether the file is
really an ELF file or not.  
• Second, the program must modify the .rodata section as follows.  
- Find a string “software”
- Modify the string to “hackers!”
so that when the elf file execute, it uses the modified string
## Requirements
• You MUST provide simple and proper comments on your code  
• Your program name MUST be your student ID#.
2021201  
• The program MUST take a file path to an ELF file as an argument of the main function so that it can execute like
   ./20212021 /home/my_elf_file