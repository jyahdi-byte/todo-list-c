#include "lab10.h"

int undo_top = 0;
undoRecord undo_stack[MAX_UNDO];

void push_undo(char action, ENTRY *entry){
    if (undo_top == MAX_UNDO){
        for (int i = 0; i < MAX_UNDO - 1; i++){
            undo_stack[i] = undo_stack[i+1];
        }
        undo_top--; 
    }
    undo_stack[undo_top].action = action;
    undo_stack[undo_top].data = *entry;
    undo_top++;
}

void undo_last(){
    if (undo_top == 0){
        printf("Nothing to Undo");
        return;
    }
    undo_top--;
    if (undo_stack[undo_top].action == 'i'){
        ENTRY *cur = head;
        while (cur != NULL){
            if (strcmp(cur->task, undo_stack[undo_top].data.task) == 0){
                raw_delete(cur);
                return;
            }
            cur = cur->next;
        }
    }
    else{
        raw_insert(undo_stack[undo_top].data.task, undo_stack[undo_top].data.date, undo_stack[undo_top].data.flag, &undo_stack[undo_top].data.reminder);
        }
    }