# Shell
## About
A functioning command line shell, written in C. 
This project works like the UNIX shell, only much simpler. Only three built-in commands — exit, cd and pwd — are implemented. 
Command line is able to execute commands with multiple arguments and read execute multiple commands from one input. CD, LS, PWD, and EXIT were all manually coded in.

### Multi-Threading and fork() in C
To run commands, I created child process and waited for them to complete. 

### Defensive Programming and Error Messages 

This project helped me learn about defensive programming. Under no circumstances will the program core dump, hang indefinitely, or prematurely terminate. 
The following scenarios are handled as errors: 

- Two or more input files to your shell program.
- A command does not exist or cannot be executed.
- A very long command line (over 512 characters, excluding the carriage return).

The following are _not_ errors, but the program can handle them.  

- An empty command line.
- An empty command between two or more ';' characters.
- Multiple white spaces on a command line.
- White space before or after the ';' character or extra white space in general.



