# todo-list-c

A command-line todo list app written in C. Started as a CSEN 11 lab series (labs 6 through 10) and grew into a doubly linked list with date-sorted insertion, undo, a background thread that autosaves to a binary file, and import/export to plain text.

## Features

- Add, delete, and edit tasks, each with a date and either a time reminder or a note
- Tasks stay sorted by date automatically on insert
- Undo, supporting multiple undos in a row (walks back through the full history, not just the last action)
- Import from and export to a plain text file
- Background thread that writes the list to a binary file every 10 seconds so state survives a crash
- Read the list back in from that binary file

## Build

```
make lab10
```

This compiles `main.c`, `list.c`, `file.c`, and `undo.c` with `-lpthread` and runs the binary with `todo_list.txt` and `out.bin` as arguments.

To build without running:

```
gcc -o lab10 main.c list.c file.c undo.c -lpthread
./lab10 todo_list.txt out.bin
```

## Usage

The menu looks like this:

```
1: insert a new todo item
2: delete a todo item
3: show all todo list
4: change a task's date
5: show last added todo item
6: import todo list
7: export todo list
8: read binary file
9: undo last
0: quit program
```

Dates are entered as `month day` (e.g. `04 02` for April 2nd). Each task can carry a time reminder, a text note, or nothing at all.

Option 3 walks to the end of the list and prints backwards, so what shows up on screen is newest date first, even though the list itself is stored oldest to newest.

## Text file format

Import and export both use `todo_list.txt`, which is a two-line header, an entry count, then one line per task:

```
Todo Reminder APP
Spring2026 CSEN11 - Advanced Programming
1

TaskA,1/1
```

Each entry line is `task,month/day`, followed by `,HH:MM` for a time reminder or `,note text` for a note. No trailing field means no reminder at all.

## Files

| File | Purpose |
|---|---|
| `main.c` | Menu loop, starts the autosave thread |
| `list.c` | Doubly linked list: insert, delete, edit, search |
| `undo.c` | Undo stack (holds up to 10 actions) |
| `file.c` | Text import/export and binary save/load |
| `lab10.h` | Struct definitions and function declarations |

## How undo works

Every insert or delete pushes a record onto a fixed-size stack (`push_undo`). `undo_last()` pops the most recent record and reverses it, deleting what was inserted or reinserting what was deleted, by calling the raw list-manipulation functions directly so undoing an action doesn't itself get recorded as a new one.

## A few implementation notes

- Task names top out at 19 characters — `LEN` is 20, and one byte is reserved for the null terminator
- The undo stack only remembers the last 10 actions; pushing an 11th quietly drops the oldest one
- The binary save is a raw `fwrite` of the `ENTRY` struct itself, so it reflects the memory layout of whatever machine wrote it and isn't meant to be portable across systems
- The autosave thread locks `list_mutex` before writing, so it can't collide with an insert or delete triggered from the menu at the same moment
