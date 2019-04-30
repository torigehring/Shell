# My Shell (msh.c)

Victoria Gehring CS240 03/25/2019
Assignment 3

## Description

This shell is bash compatible. User types a bash command or "exit" to exit. It also features semicolon separated commands, reading commands from files, aliasing, history, PATH extensions, piping, and changing font color. 

## Usage

This shell will run bash commands.
Upon running, program will prompt for color choice, automatically run commands in mshrc (if file exists in the current directory), and will prompt the user with ">:" where they can enter bash commands. 

1. SEMICOLONS: Commands entered with semicolons (ex. ls ; pwd) will be executed sequentially by the shell ("ls" then "pwd").

2. MSHRC: If a mshrc file exists, the shell will run the commands in this file immediately after prompting for a color choice and then proceed to prompt the user. If it doesn't exist it will prompt the user for commands.

3. ALIAS/UNALIAS: Typing "alias" will output a list of aliases. To create an alias use regular Unix syntax. Typing alias xyz="x -x" will define an alias xyz. Typing unalias xyz will remove that alias if it exists. You can redefine an alias by adding it again, and the previous alias will be removed.

4. PATH/ENVIRONMENT VARIABLES: You can set environment variables by typing "export XYZ="xxx" ". Environment variables MUST BE IN ALL CAPS. PATH will be automatically stored in this list at the start of the program. Typing "export PATH=$PATH:/foo/bin" etc. will expend the PATH variable. You can define any environment variable with this syntax. To unset a variable type "unset XYZ" and it will be removed from the list. Typing just export or unset with no args will print the list of current environment variables.

5. HISTORY: Typing "history" will print a list of the last 20 commands. Typing !! will execute the previous command. Typing !xx where x is an integer less than 20, will execute the command indexed by that number in the 20 item history. Duplicate commands and unsuccessful commands are included in the history. 

6. PIPING: Only supports 2 pipe commands with 1 argument for each (ex. "ls -l | wc -l"). Only supports piping with the '|' symbol.

7. FONT COLOR: At the start of the program user is provided with a list of colors, enter the number of the corresponding color to change color. Invalid entries result in default color. The color is reset to default upon exit. 

## How to Build

```
There is a Makefile included in order to build msh. 
(the test files included were used for my personal testing purposes)
To make msh: 
make msh
```

## How to Run

```
./msh
```
