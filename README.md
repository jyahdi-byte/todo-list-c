# todo-list-c

A command-line todo list app written in C. Started as a CSEN 11 lab series and grew into a doubly linked list with date-sorted insertion, undo, a background thread that autosaves to a binary file, and import/export to plain text.

## Features

- Add, delete, and edit tasks, each with a date and either a time reminder or a note
- Tasks stay sorted by date automatically on insert
- Undo, supporting multiple undos in a row (walks back through the full history, not just the last action)
- Import from and export to a plain text file
- Background thread periodically writes the list to a binary file so state survives a crash
- Read the list back in from that binary file

## Build

\```
make lab10
\```

This compiles `main.c`, `list.c`, `file.c`, and `undo.c` with `-lpthread` and runs the binary with `todo_list.txt` and `out.bin` as arguments.

To build without running:

\```
gcc -o lab10 main.c list.c file.c undo.c -lpthread
./lab10 todo_list.txt out.bin
\```

## Files

| File | Purpose |
|---|---|
| `main.c` | Menu loop, starts the autosave thread |
| `list.c` | Doubly linked list: insert, delete, edit, search |
| `undo.c` | Undo stack (holds up to 10 actions) |
| `file.c` | Text import/export and binary save/load |
| `lab10.h` | Struct definitions and function declarations |

## How undo works

Every insert or delete pushes a record onto a fixed-size stack (`push_undo`). `undo_last()` pops the most recent record and reverses it — deletes what was inserted, or reinserts what was deleted — by calling the raw list-manipulation functions directly, so undoing an action doesn't itself get recorded as a new action.
