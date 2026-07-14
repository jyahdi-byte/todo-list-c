#include "lab10.h"

ENTRY *head = NULL;
ENTRY *last_added = NULL;
pthread_mutex_t list_mutex;

int main(int argc, char **argv)
{
    if (argc < 3){ //missing arguments
        printf("Missing Arguments");
        return 1;
    }
    load_list();
    pthread_mutex_init(&list_mutex, NULL);
    pthread_t saver_thread; 
    pthread_create(&saver_thread, NULL, save_binary, argv[2]);
    while (1)
    {
        int option;
        printf("\nWelcome to todo list app\n");
        printf("1: insert a new todo item\n");
        printf("2: delete a todo item\n");
        printf("3: show all todo list\n");
        printf("4: change a task's date\n");
        printf("5: show last added todo item\n");
        printf("6: import todo list\n");
        printf("7: export todo list\n");
        printf("8: read binary file\n");
        printf("9: undo last\n");
        printf("0: quit program\n");

        if (scanf("%d", &option) != 1)
        {
            printf("option error\n");
            return 0;
        }
        clear_buffer();

        switch (option)
        {
            case 1: insert();     break;
            case 2: delete();     break;
            case 3: show_backwards(head);    break;
            case 4: change();     break;
            case 5: show_last();  break;
            case 6: load_list();  break;
            case 7: save_list();  break;
            case 8: read_binary(argv[2]); break;
            case 9: pthread_mutex_lock(&list_mutex);
                    undo_last();
                    pthread_mutex_unlock(&list_mutex);
            break;
            case 0:
                pthread_cancel(saver_thread);
                save_list();
                printf("exit\n");
                delete_all(head);
                head = NULL;
                return 0;
            default:
                printf("wrong option\n");
        }
    }
}
