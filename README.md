# ASH

A simple custom shell built in C++.

This project was made to understand how real shells work by implementing basic shell features from scratch.

## Features

* Custom shell prompt (`$`)
* `echo` builtin command
* `exit` builtin command
* `type` builtin command
* Searches commands in `PATH`
* Runs external commands using `fork()` and `execvp()`
* Supports command arguments

## Example Usage

```bash
$ echo hello world
hello world

$ type echo
echo is a shell builtin

$ type python
python is /usr/bin/python

$ ls
main.cpp shell

$ pwd
/home/user/project

$ exit
```

## How It Works

1. Shows a shell prompt
2. Takes user input
3. Parses command and arguments
4. Checks if command is builtin
5. If not builtin, searches executable in `PATH`
6. Runs external commands


## Compile and Run

```bash
g++ main.cpp -o shell
./shell
```

## Future Improvements

* Add `cd` command
* Support pipes (`|`)
* Support redirection (`>`)
* Handle quotes properly
* Add command history

## Purpose

This project was built as a learning project to understand operating systems, process creation, and how command-line shells work internally.
