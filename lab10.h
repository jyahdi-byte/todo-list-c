#ifndef LAB10_H
#define LAB10_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define LEN 20
#define MAX_UNDO 10

typedef union {
    int  time[2];
    char note[LEN];
} REMINDER;

typedef struct ENTRY {
    char     task[LEN];
    int      date[2];
    int      flag;
    REMINDER reminder;
    struct ENTRY *next;
    struct ENTRY *prev;
} ENTRY;

typedef struct{
    char action; // 'i' = insert, 'd' = delete
    struct ENTRY data; // copy of the affected node
} undoRecord;

extern int undo_top; // new global variable
extern undoRecord undo_stack[MAX_UNDO]; // new global variable


extern ENTRY *head;
extern ENTRY *last_added;
extern pthread_mutex_t list_mutex;

int  main(int argc, char **argv);
void insert(void);
void delete(void);
void showall(void);
void change(void);
void show_last(void);
int  load_list(void);
int  save_list(void);
ENTRY *raw_insert(char *ltask, int *ldate, int lflag, REMINDER *lreminder);
void raw_delete(ENTRY *target);
void real_insert(char *ltask, int *ldate, int lflag, REMINDER *lreminder);
void real_delete(ENTRY *target);
bool compare(int *newDate, int *existingDate);
void clear_buffer(void);
void show_backwards(ENTRY *cur);
void delete_all(ENTRY *cur);
void *save_binary(void *name);
void read_binary(char *name);
void push_undo(char action, ENTRY *target);
void undo_last();

#endif
