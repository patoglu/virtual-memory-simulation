#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <execinfo.h>
#include <signal.h>




typedef struct page_table_entry_t1
{
    int page_frame_number; //which physical address it's maps.
    int present_absent; // is the data in memory or in disk.
    int dirty_bit; //is the page modified or not
    int referenced;
}page_table_entry;


typedef struct page_table_t1
{
    page_table_entry *table;
    int num_virtual;
    unsigned int frame_size;
    unsigned int num_physical;
    char page_replacement[10];
    char alloc_policy[10];
    unsigned int page_table_print_int;
    char disk_name[30];

}page_table;






extern page_table *v_page_table;
extern int *physical_mem;
extern pthread_t thread;
extern int err;
extern pthread_mutex_t lock;


extern int reads_bubble_get;
extern int writes_bubble_get;
extern int page_misses_bubble_get;
extern int page_replacements_bubble_get;
extern int disk_page_writes_bubble_get;
extern int disk_page_reads_bubble_get;

extern int reads_bubble_set;
extern int writes_bubble_set;
extern int page_misses_bubble_set;
extern int page_replacements_bubble_set;
extern int disk_page_writes_bubble_set;
extern int disk_page_reads_bubble_set;


extern int reads_quick_get;
extern int writes_quick_get;
extern int page_misses_quick_get;
extern int page_replacements_quick_get;
extern int disk_page_writes_quick_get;
extern int disk_page_reads_quick_get;




extern int reads_quick_set;
extern int writes_quick_set;
extern int page_misses_quick_set;
extern int page_replacements_quick_set;
extern int disk_page_writes_quick_set;
extern int disk_page_reads_quick_set;



extern int reads_merge_get;
extern int writes_merge_get;
extern int page_misses_merge_get;
extern int page_replacements_merge_get;
extern int disk_page_writes_merge_get;
extern int disk_page_reads_merge_get;




extern int reads_merge_set;
extern int writes_merge_set;
extern int page_misses_merge_set;
extern int page_replacements_merge_set;
extern int disk_page_writes_merge_set;
extern int disk_page_reads_merge_set;


extern int reads_index_get;
extern int writes_index_get;
extern int page_misses_index_get;
extern int page_replacements_index_get;
extern int disk_page_writes_index_get;
extern int disk_page_reads_index_get;




extern int reads_index_set;
extern int writes_index_set;
extern int page_misses_index_set;
extern int page_replacements_index_set;
extern int disk_page_writes_index_set;
extern int disk_page_reads_index_set;


extern int front;
extern int rear;
extern int front_1;
extern int rear_1;
extern int front_2;
extern int rear_2;
extern int front_3;
extern int rear_3;
extern int front_4;
extern int rear_4;







#define NOTPRESENTINMEMORY 99

#define SIZE 4096
#define PHYSICAL_MEM_DEFAULT -1
extern int items[SIZE];
extern int items_1[SIZE];
extern int items_2[SIZE];
extern int items_3[SIZE];
extern int items_4[SIZE];

#define _JUMP 15

void
check_arguments(unsigned int frame_size,
                unsigned int num_physical,
                unsigned int num_virtual,
                char page_replacement[10],
                char alloc_policy[10],
                unsigned int page_table_print_int);


void print_page_table(page_table *virtual_page_table);




void
fill_virtual_memory(unsigned int frame_size, unsigned int num_physical, unsigned int num_virtual, char *disk_name);


void
create_page_table(unsigned int frame_size,
                     unsigned int num_physical,
                     unsigned int num_virtual,
                     char page_replacement[10],
                     char alloc_policy[10],
                     unsigned int page_table_print_int,
                     char disk_name[30]);


void create_page_table_FIFO(void);

void create_page_table_SC(void);

void create_page_table_LRU(void);

void create_page_table_WSClock(void);

void init_physical_memory(int frame_size, int num_physical);

void
free_page_table(void);

void enQueue(int value);

int deQueue();


void enQueue_1(int value);

int deQueue_1();


void enQueue_2(int value);

int deQueue_2();


void enQueue_3(int value);

int deQueue_3();


void enQueue_4(int value);

int deQueue_4();

// Function to print the queue
void display();

void
create_physical_memory(int frame_size, int num_physical);

void
free_physical_memory(void);

void
set(unsigned int index, int value, char * tName);
int
get(unsigned int index, char * tName);

void
print_physical_memory(int num_physical, int frame_size);





extern int page_copying;
extern int* indices;




void*
thread_1();
void*
thread_2();
void*
thread_3();
void*
thread_4();





/*Quick Sort Helper Functions */


void
swap(int *first, int *second);



void allocate_tanenbaum_LRU_matrix(int page_no);




void free_tanenbaum_LRU_matrix(int page_no);


extern int **tanenbaum_LRU_matrix;

extern int **tanenbaum_LRU_matrir;
extern int **tanenbaum_LRU_matrix_first_quarter;
extern int **tanenbaum_LRU_matrix_second_quarter;
extern int **tanenbaum_LRU_matrix_third_quarter;
extern int **tanenbaum_LRU_matrix_fourth_quarter;

void set_i_nth_page(int current_page, int page_no, char *policy, int which_quarter);

int get_LRU_page(int page_no, char *alloc_policy, int which_quarter);

extern int *present_pages;
extern int *present_pages_1;
extern int *present_pages_2;
extern int *present_pages_3;
extern int *present_pages_4;


int get_LRU_page_first(int page_no);
int get_LRU_page_second(int page_no);
int get_LRU_page_third(int page_no);
int get_LRU_page_fourth(int page_no);


void set_i_nth_page_first_quarter(int current_page, int page_no);
void set_i_nth_page_second_quarter(int current_page, int page_no);
void set_i_nth_page_third_quarter(int current_page, int page_no);
void set_i_nth_page_fourth_quarter(int current_page, int page_no);

void check();
void print_statistics();
