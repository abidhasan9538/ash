# ash

A minimal POSIX-style command shell written from scratch in C++.

`ash` is a REPL that reads a line, parses it into a command and arguments, and
either handles it internally (builtins) or resolves it against `PATH` and
executes it in a child process. It exists to demystify what a shell actually
does between you pressing Enter and a program running — process creation,
`PATH` resolution, and the builtin-vs-external decision — without the weight of
a production shell.

No dependencies beyond the C++ standard library and POSIX system calls.

---

## Contents

- [Quick start](#quick-start)
- [Supported commands](#supported-commands)
- [How it works](#how-it-works)
- [Design notes](#design-notes)
- [Limitations](#limitations)
- [Roadmap](#roadmap)
- [Project layout](#project-layout)

---

## Quick start

Requires a C++ compiler with C++20 support (the source uses `<ranges>`) and a
POSIX environment (Linux or macOS).

```bash
# from the repository root
g++ -std=c++20 src/main.cpp -o ash
./ash
```

You'll get a `$ ` prompt:

```
$ echo hello world
hello world
$ pwd
/home/you/ash
$ type echo
echo is a shell builtin
$ type ls
ls is /bin/ls
$ ls src
main.cpp
$ exit
```

---

## Supported commands

### Builtins

| Command | Behavior |
| --- | --- |
| `echo [args...]` | Prints its arguments separated by single spaces, followed by a newline. |
| `pwd` | Prints the current working directory. |
| `cd [dir]` | Changes directory. With no argument or `~`, changes to `$HOME`. Reports `No such file or directory` on failure. |
| `type <name>` | Reports whether `<name>` is a shell builtin, an executable found on `PATH` (printing its full path), or not found. |
| `exit` | Exits the shell (returns 0). |

### External commands

Anything that isn't a builtin is treated as an external program. `ash` resolves
it and runs it with its arguments, waiting for it to finish before showing the
next prompt. Both bare names (`ls`, resolved via `PATH`) and explicit paths
(`./ash`, `/bin/ls`) are supported.

---

## How it works

The whole shell is a single read-eval loop in `main()`:

1. **Prompt & read.** Print `$ `, then `getline` a full line of input.
2. **Tokenize.** Feed the line into a `stringstream`; the first token is the
   command, the rest are arguments, split on whitespace.
3. **Dispatch builtins.** `exit`, `echo`, `pwd`, `cd`, and `type` are matched
   directly and handled in-process.
4. **Resolve externals.** If the command contains a `/`, it's treated as a path
   and checked directly with `access(..., X_OK)`. Otherwise `ash` walks each
   entry in `$PATH`, joining it with the command name and taking the first
   result that is executable.
5. **Execute.** `fork()` a child; in the child, `execvp()` the resolved
   executable with the argument vector. The parent `wait()`s for the child to
   exit, then loops.

`PATH` resolution is done by hand — splitting `$PATH` on `:` and testing each
`dir/command` candidate with `access()` — which is precisely the lookup a real
shell performs and the reason `type <name>` can report where a program lives.

---

## Design notes

- **Output is unbuffered.** `cout` and `cerr` are set to `unitbuf` so prompts
  and program output interleave correctly rather than being held in a buffer.
- **Builtins run in the parent process.** `cd` in particular *must* be a
  builtin: a child process changing directory would not affect the shell, so
  the directory change has to happen in `ash` itself via `chdir()`.
- **`fork` + `execvp`, not `system()`.** Spawning the child by hand is the point
  of the project — it's where the shell earns its keep.

---

## Limitations

This is a learning shell, and it stops well short of a real one. Known gaps:

- **No quoting or escaping.** `echo "hello   world"` collapses to
  `hello world`; quotes are treated as ordinary characters and repeated
  whitespace is not preserved.
- **No pipes or redirection.** `|`, `>`, `<`, and `>>` are not implemented.
- **No globbing or variable expansion.** `*` and `$VAR` are passed through
  literally.
- **No command history, line editing, or job control.** No arrow keys, no
  `Ctrl-R`, no background jobs.
- **Minimal error reporting.** A failed `execvp` prints a generic message
  rather than a specific `errno` reason.

---

## Roadmap

- [ ] Preserve arguments and whitespace inside single and double quotes
- [ ] Input/output redirection (`>`, `>>`, `<`)
- [ ] Pipelines (`cmd1 | cmd2`)
- [ ] Environment variable expansion (`$VAR`)
- [ ] Command history and line editing
- [ ] Proper `errno`-based error messages

---

## Project layout

```
ash/
├── README.md
└── src/
    └── main.cpp   # the entire shell: REPL, builtins, PATH resolution, exec
```

---

## Why

Built to understand operating-system fundamentals hands-on: how a shell creates
processes, resolves commands, and draws the line between what it handles itself
and what it hands off to the kernel. The best way to learn where the boundary
sits is to draw it yourself.
