#include "virtual_mem.h"
page_table *v_page_table = NULL;
int *physical_mem = NULL;
pthread_mutex_t lock;
int* indices = NULL;
int page_copying = 0;
int r1,r2;

int items[SIZE], front = -1, rear = -1;
int items_1[SIZE], front_1 = -1, rear_1 = -1;
int items_2[SIZE], front_2 = -1, rear_2 = -1;
int items_3[SIZE], front_3 = -1, rear_3 = -1;
int items_4[SIZE], front_4 = -1, rear_4 = -1;


pthread_t thread1;
pthread_t thread2;
pthread_t thread3;
pthread_t thread4;
pthread_t thread5;
pthread_mutex_t lock;

int reads_bubble_get = 0;
int writes_bubble_get = 0;
int page_misses_bubble_get = 0;
int page_replacements_bubble_get = 0;
int disk_page_writes_bubble_get = 0;
int disk_page_reads_bubble_get = 0;




int reads_bubble_set = 0;
int writes_bubble_set = 0;
int page_misses_bubble_set = 0;
int page_replacements_bubble_set = 0;
int disk_page_writes_bubble_set = 0;
int disk_page_reads_bubble_set = 0;


int reads_quick_get = 0;
int writes_quick_get = 0;
int page_misses_quick_get = 0;
int page_replacements_quick_get = 0;
int disk_page_writes_quick_get = 0;
int disk_page_reads_quick_get = 0;




int reads_quick_set = 0;
int writes_quick_set = 0;
int page_misses_quick_set = 0;
int page_replacements_quick_set = 0;
int disk_page_writes_quick_set = 0;
int disk_page_reads_quick_set = 0;


int reads_merge_get = 0;
int writes_merge_get = 0;
int page_misses_merge_get = 0;
int page_replacements_merge_get = 0;
int disk_page_writes_merge_get = 0;
int disk_page_reads_merge_get = 0;




int reads_merge_set = 0;
int writes_merge_set = 0;
int page_misses_merge_set = 0;
int page_replacements_merge_set = 0;
int disk_page_writes_merge_set = 0;
int disk_page_reads_merge_set = 0;


int reads_index_get = 0;
int writes_index_get = 0;
int page_misses_index_get = 0;
int page_replacements_index_get = 0;
int disk_page_writes_index_get = 0;
int disk_page_reads_index_get = 0;




int reads_index_set = 0;
int writes_index_set = 0;
int page_misses_index_set = 0;
int page_replacements_index_set = 0;
int disk_page_writes_index_set = 0;
int disk_page_reads_index_set = 0;

int **tanenbaum_LRU_matrix = NULL;
int **tanenbaum_LRU_matrix_first_quarter = NULL;
int **tanenbaum_LRU_matrix_second_quarter = NULL;
int **tanenbaum_LRU_matrix_third_quarter = NULL;
int **tanenbaum_LRU_matrix_fourth_quarter = NULL;


int *present_pages = NULL;
int *present_pages_1 = NULL;
int *present_pages_2 = NULL;
int *present_pages_3 = NULL;
int *present_pages_4 = NULL;
int main(int argc, char const *argv[]) {

    //page_table *virtual_page_table;
    int err;
    int i;
    unsigned int frame_size;   //page size
    unsigned int num_physical; //total no pages in physical memory.
    unsigned int num_virtual;  //total no pages in virtual memory.
    char page_replacement[10]; //page replacement algorithm type.
    char alloc_policy[10];     //allocation policy(global or local)
    unsigned int page_table_print_int; //interval of memory accesses to print.
    char disk_name[30]; //simulated disk to store out of memory frames.

    if(argc != 8)
    {
        fprintf(stderr, "%s\n", "usage: sortArrays frameSize numPhysical numVirtual pageReplacement allocPolicy pageTablePrintInt filename" );
        exit(1);
    }
    //pthread_t       thread1, thread2;
    frame_size = pow(2, atoi(argv[1]));
    num_physical = pow(2, atoi(argv[2]));
    num_virtual = pow(2, atoi(argv[3]));
    strcpy(page_replacement, argv[4]);
    strcpy(alloc_policy, argv[5]);
    page_table_print_int = atoi(argv[6]);
    strcpy(disk_name, argv[7]);

    check_arguments(frame_size,
                    num_physical,
                    num_virtual,
                    page_replacement,
                    alloc_policy,
                    page_table_print_int);

    srand(1000);


    // ALLOCATE RESOURCES
    create_physical_memory(frame_size, num_physical);
    init_physical_memory(frame_size, num_physical);


    create_page_table(frame_size, num_physical, num_virtual, page_replacement, alloc_policy, page_table_print_int, disk_name);


    allocate_tanenbaum_LRU_matrix(v_page_table->num_virtual);
    allocate_tanenbaum_LRU_matrix_first_quarter(v_page_table->num_virtual);
    allocate_tanenbaum_LRU_matrix_second_quarter(v_page_table->num_virtual);
    allocate_tanenbaum_LRU_matrix_third_quarter(v_page_table->num_virtual);
    allocate_tanenbaum_LRU_matrix_fourth_quarter(v_page_table->num_virtual);

    present_pages = calloc(v_page_table -> num_physical, sizeof(int));
    present_pages_1 = calloc(v_page_table -> num_physical / 4, sizeof(int));
    present_pages_2 = calloc(v_page_table -> num_physical / 4, sizeof(int));
    present_pages_3 = calloc(v_page_table -> num_physical / 4, sizeof(int));
    present_pages_4 = calloc(v_page_table -> num_physical / 4, sizeof(int));


    fill_virtual_memory(frame_size, num_physical, num_virtual,disk_name);





    if (pthread_mutex_init(&lock, NULL) != 0)
    {
     printf("\n mutex init failed\n");
     return 1;
    }


     err = pthread_create(&thread1, NULL, &thread_1, NULL);
     if (err != 0)
         printf("\ncan't create thread :[%s]", strerror(err));

    err = pthread_create(&thread2, NULL, &thread_2, NULL);
    if (err != 0)
        printf("\ncan't create thread :[%s]", strerror(err));

    err = pthread_create(&thread3, NULL, &thread_3, NULL);
    if (err != 0)
        printf("\ncan't create thread :[%s]", strerror(err));

    err = pthread_create(&thread4, NULL, &thread_4, NULL);
    if (err != 0)
        printf("\ncan't create thread :[%s]", strerror(err));



    //ALLOCATE RESOURCES END
     pthread_join(thread1, NULL);
     pthread_join(thread2, NULL);
     pthread_join(thread3, NULL);
     pthread_join(thread4, NULL);
    // pthread_join(thread5, NULL);
     pthread_mutex_destroy(&lock);









    //FREE RESOURCES
    free_tanenbaum_LRU_matrix(v_page_table->num_virtual);


    free_tanenbaum_LRU_matrix_first_quarter(v_page_table->num_virtual);
    free_tanenbaum_LRU_matrix_second_quarter(v_page_table->num_virtual);
    free_tanenbaum_LRU_matrix_third_quarter(v_page_table->num_virtual);
    free_tanenbaum_LRU_matrix_fourth_quarter(v_page_table->num_virtual);


    print_statistics();
    printf("\n\n\n\n");

    free(present_pages);
    free(present_pages_1);
    free(present_pages_2);
    free(present_pages_3);
    free(present_pages_4);
    free(indices);
    free_page_table();
    free_physical_memory();
    return 0;
}
