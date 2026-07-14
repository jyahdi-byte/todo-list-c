#include "lab10.h"

void clear_buffer(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void insert(void)
{
    char     newTask[LEN];
    int      tempDate[2];
    int      tempFlag;
    REMINDER tempReminder;

    int days_in_month[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int valid = 0;

    printf("Enter your task description:\n");
    if (scanf("%19[^\n]", newTask) != 1)
    {
        printf("task scanf error\n");
        clear_buffer();
        return;
    }
    clear_buffer();

    /* check for duplicate */
    ENTRY *cur = head;
    while (cur != NULL)
    {
        if (strcmp(cur->task, newTask) == 0)
        {
            printf("Error: this task is already in the list\n");
            return;
        }
        cur = cur->next;
    }

    while (valid == 0){
        printf("Enter the task month and date\n");
        printf(" (format: month day, e.g. 04 02 for April 2nd)\n");
        if (scanf("%d %d", &tempDate[0], &tempDate[1]) != 2)
        {
            printf("date scanf error\n");
            clear_buffer();
        }
        else if(tempDate[0] < 1){
            printf("Invalid Date");
        }
        else if(tempDate[0] > 12){
            printf("Invalid Date");
        }
        else if (tempDate[1] > days_in_month[tempDate[0] - 1]){
            printf("Invalid Date");
        }
        else if (tempDate[1] < 1){
            printf("Invalid Date");
        }
        else{
            valid = 1;
        }
        clear_buffer();
    }

    printf("0 for time reminder, 1 for note reminder, 2 for no reminder: ");
    if (scanf("%d", &tempFlag) != 1)
    {
        printf("flag scanf error\n");
        clear_buffer();
        return;
    }
    clear_buffer();

    if (tempFlag == 0)
    {
        printf("Enter reminder time (hour minute): ");
        if (scanf("%d %d", &tempReminder.time[0], &tempReminder.time[1]) != 2)
        {
            printf("time scanf error\n");
            clear_buffer();
            return;
        }
        clear_buffer();
    }
    else if (tempFlag == 1)
    {
        printf("Enter reminder note: ");
        if (scanf("%19[^\n]", tempReminder.note) != 1)
        {
            printf("note scanf error\n");
            clear_buffer();
            return;
        }
        clear_buffer();
    }
    else if (tempFlag == 2)
    {
    }
    else
    {
        printf("Invalid flag\n");
        return;
    }
    pthread_mutex_lock(&list_mutex);
    real_insert(newTask, tempDate, tempFlag, &tempReminder);
    pthread_mutex_unlock(&list_mutex);
    printf("Successfully added!\n");
}

void delete(void)
{
    if (head == NULL)
    {
        printf("List is empty\n");
        return;
    }

    char ltask[LEN];
    printf("Enter task to delete:\n");
    if (scanf("%19[^\n]", ltask) != 1)
    {
        printf("task scanf error\n");
        clear_buffer();
        return;
    }
    clear_buffer();

    ENTRY *cur = head;
    while (cur != NULL)
    {
        if (strcmp(cur->task, ltask) == 0)
            break;
        cur = cur->next;
    }

    if (cur == NULL)
    {
        printf("This todo item is not in the list\n");
        return;
    }

    pthread_mutex_lock(&list_mutex);
    real_delete(cur);
    pthread_mutex_unlock(&list_mutex);
    printf("Successfully deleted!\n");
}

void showall(void)
{
    if (head == NULL)
    {
        printf("List is empty\n");
        return;
    }

    ENTRY *cur = head;

    while (cur != NULL){
        if (cur->flag == 0){
            printf("%d/%d   %s", cur->date[0], cur->date[1], cur->task);
            printf("\t[Reminder: %02d:%02d]\n", cur->reminder.time[0], cur->reminder.time[1]);
        }
        else if (cur->flag == 1){
            printf("%d/%d   %s", cur->date[0], cur->date[1], cur->task);
            printf("\t[Note: %s]\n", cur->reminder.note);
        }
        else{
            printf("%d/%d   %s  \n", cur->date[0], cur->date[1], cur->task);
        }
        cur = cur->next;
    }
}

void change(void)
{

    int days_in_month[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int valid = 0;

    if (head == NULL)
    {
        printf("List is empty\n");
        return;
    }

    char ltask[LEN];
    int  newDate[2];

    printf("Enter task to change:\n");
    if (scanf("%19[^\n]", ltask) != 1)
    {
        printf("task scanf error\n");
        clear_buffer();
        return;
    }
    clear_buffer();

    ENTRY *cur = head;
    while (cur != NULL)
    {
        if (strcmp(cur->task, ltask) == 0)
            break;
        cur = cur->next;
    }

    if (cur == NULL)
    {
        printf("This todo item is not in the list\n");
        return;
    }

    while (valid == 0){
        printf("Enter the new month and date (e.g., 04 02):\n");
        if (scanf("%d %d", &newDate[0], &newDate[1]) != 2)
        {
            printf("date scanf error\n");
            clear_buffer();
        }
        else if(newDate[0] < 1){
            printf("Invalid Date");
        }
        else if(newDate[0] > 12){
            printf("Invalid Date");
        }
        else if (newDate[1] > days_in_month[newDate[0] - 1]){
            printf("Invalid Date");
        }
        else if (newDate[1] < 1){
            printf("Invalid Date");
        }
        else{
            valid = 1;
        }

    clear_buffer();
    }
    int      savedFlag     = cur->flag;
    REMINDER savedReminder = cur->reminder;

    pthread_mutex_lock(&list_mutex);
    real_delete(cur);
    real_insert(ltask, newDate, savedFlag, &savedReminder);
    pthread_mutex_unlock(&list_mutex);

    printf("Successfully changed date!\n");
}

void show_last(void)
{
    if (last_added == NULL)
    {
        printf("List is empty\n");
        return;
    }

    printf("Last added: %d/%d   %s", last_added->date[0], last_added->date[1], last_added->task);

    if (last_added->flag == 0)
        printf("\t[Reminder: %02d:%02d]", last_added->reminder.time[0], last_added->reminder.time[1]);
    else if (last_added->flag == 1)
        printf("\t[Note: %s]", last_added->reminder.note);

    printf("\n");
}

bool compare(int *newDate, int *existingDate)
{
    if (newDate[0] < existingDate[0])
        return true;
    if (newDate[0] > existingDate[0])
        return false;
    return newDate[1] < existingDate[1];
}

ENTRY *raw_insert(char *ltask, int *ldate, int lflag, REMINDER *lreminder)
{
    ENTRY *new_insert = (ENTRY *) malloc(sizeof(ENTRY));
    strcpy(new_insert->task, ltask);
    new_insert->date[0]  = ldate[0];
    new_insert->date[1]  = ldate[1];
    new_insert->flag     = lflag;
    new_insert->reminder = *lreminder;
    new_insert->next     = NULL;
    new_insert->prev     = NULL;

    ENTRY *cur = head;

    while (cur != NULL && !compare(ldate, cur->date)){
        cur = cur->next;
    }

    if (cur == NULL){
        if (head == NULL){
            head = new_insert;
        }
        else{
            ENTRY *tail = head;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = new_insert;
            new_insert->prev = tail;
        }
        last_added = new_insert;
    }

    else if (cur == head){
        new_insert->next = cur;
        new_insert->prev = NULL;
        cur->prev = new_insert;
        head = new_insert;
        last_added = new_insert;
    }

    else{
        new_insert->next = cur;
        new_insert->prev = cur->prev;
        (cur->prev)->next = new_insert;
        cur->prev = new_insert;
        last_added = new_insert;
    }
    return new_insert;
}

void real_insert(char *ltask, int *ldate, int lflag, REMINDER *lreminder)
{
    ENTRY *new_insert = raw_insert(ltask, ldate, lflag, lreminder);
    push_undo('i', new_insert);
}

void raw_delete(ENTRY *target)
{
    if (target == head){
        head = target->next;
        if (head != NULL){
            head->prev = NULL;
        }
        ENTRY *temp = target;
        free(temp);
    }

    else if (target->next == NULL){
        (target->prev)->next = NULL;
        ENTRY *temp = target;
        free(temp);
    }

    else{
        (target->prev)->next = target->next;
        (target->next)->prev = target->prev;
        ENTRY *temp = target;
        free(temp);
    }
}

void real_delete(ENTRY *target)
{
    push_undo('d', target);
    raw_delete(target);
}



void show_backwards(ENTRY *cur){
    if (cur == NULL){
        return;
    }
    else{
        if (cur->flag == 0){
            show_backwards(cur->next);
            printf("%d/%d   %s", cur->date[0], cur->date[1], cur->task);
            printf("\t[Reminder: %02d:%02d]\n", cur->reminder.time[0], cur->reminder.time[1]);
        }
        else if (cur->flag == 1){
            show_backwards(cur->next);
            printf("%d/%d   %s", cur->date[0], cur->date[1], cur->task);
            printf("\t[Note: %s]\n", cur->reminder.note);
        }
        else{
            show_backwards(cur->next);
            printf("%d/%d   %s  \n", cur->date[0], cur->date[1], cur->task);
        }
    }
}

void delete_all(ENTRY *cur){
    if (cur == NULL){
        return;
    }
    else{
        delete_all(cur->next);
        free(cur);
    }
}

