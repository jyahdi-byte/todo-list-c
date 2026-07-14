#include "lab10.h"

int load_list(){
    FILE *loaded_list;
    loaded_list = fopen("todo_list.txt", "r");
    if (loaded_list == NULL){
        return -1;
    }
    else{
        while(head != NULL){
            real_delete(head);
        }
        last_added = NULL;
        char buffer[100];
        fgets(buffer, sizeof(buffer), loaded_list);
        fgets(buffer, sizeof(buffer), loaded_list);
        int entry_count;
        fscanf(loaded_list, "%d", &entry_count);
        for (int i = 0; i < entry_count; i++){
            char task[100];
            fscanf(loaded_list, " %[^,],", task);
            int month;
            int day;
            fscanf(loaded_list, "%d/%d", &month, &day);
            int next = fgetc(loaded_list);
            int flag;
            REMINDER tempreminder;
            int hour;
                int min;
            if (next == ','){
                fscanf(loaded_list, "%[^\n]", buffer); //read the stuff from text(reminder)
                if (sscanf(buffer, "%d:%d", &hour, &min) == 2){ //count amount of ints
                    flag = 0;
                }
                else{
                    flag = 1;
                }
            }
            else if (next == '\n'){
                flag = 2;
            }
            else{
                printf("Error: Invalid Format");
                return -1;
            }
            if (flag == 0){
                tempreminder.time[0] = hour;
                tempreminder.time[1] = min;
            }
            else if (flag == 1){
                strcpy(tempreminder.note, buffer);
            }
            int date[2];
            date[0] = month;
            date[1] = day; 
            real_insert(task, date, flag, &tempreminder);
        }
        fclose(loaded_list);
        printf("Succesfully loaded from todo_list.txt");
        return 0;
    }
}

int save_list(){
    FILE *saved_list;
    saved_list = fopen("todo_list.txt", "w");
    if (saved_list == NULL){
        return -1;
    }
    fprintf(saved_list, "Todo Reminder APP\n");
    fprintf(saved_list, "Spring2026 CSEN11 - Advanced Programming\n");

    ENTRY *cur = head;
    int node_count = 0;
    while (cur != NULL){
        node_count += 1;
        cur = cur->next;
    }

    fprintf(saved_list, "%d\n", node_count);

    cur = head;
    while (cur != NULL){
        fprintf(saved_list, "\n%s,%d/%d", cur->task, cur->date[0], cur->date[1]);
        if (cur->flag == 0){
            fprintf(saved_list, ",%02d:%02d", cur->reminder.time[0], cur->reminder.time[1]);
        }
        else if (cur->flag == 1){
            fprintf(saved_list, ",%s", cur->reminder.note);
        }
        cur = cur->next;
    }
    fclose(saved_list);
    printf("Successfully saved to todo_list.txt\n");
    return 0;
}

void *save_binary(void *name){
    char *filename = (char *) name; //make char so i can use as filename
    while (1){
        sleep(10);
        pthread_mutex_lock(&list_mutex);
        FILE *binary_list;
        binary_list = fopen(filename, "wb");
        ENTRY *cur = head;
        while (cur != NULL){
            fwrite(cur, sizeof(ENTRY), 1, binary_list); //write data from each node
            cur = cur->next;
        }
        fclose(binary_list);
        pthread_mutex_unlock(&list_mutex);

    }
    return NULL;
}

void read_binary(char *name){
    FILE *binary_list;
    ENTRY node;
    binary_list = fopen(name, "rb");
    while (fread(&node, sizeof(ENTRY), 1, binary_list) == 1){
        if (node.flag == 0){
            printf("%d/%d   %s", node.date[0], node.date[1], node.task);
            printf("\t[Reminder: %02d:%02d]\n", node.reminder.time[0], node.reminder.time[1]);
        }
        else if (node.flag == 1){
            printf("%d/%d   %s", node.date[0], node.date[1], node.task);
            printf("\t[Note: %s]\n", node.reminder.note);
        }
        else{
            printf("%d/%d   %s  \n", node.date[0], node.date[1], node.task);
        }
    }
    fclose(binary_list);
}