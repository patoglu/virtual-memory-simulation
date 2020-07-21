#include "virtual_mem.h"


/*
int temp;


temp = get(j, "Bubble Sort");
set(j, get(j+1, "Bubble Sort"), "Bubble Sort");
set(j + 1, temp, "Bubble Sort");

*/
//http://www.csystem.org/kaynak-kod-arsivi/indis-s%C4%B1ralama-1-index-sort

int *index_sort(int b[], int len)
{

   int i, j, indis, temp;

   for (i = 0; i < len; i++)
      b[i] = i;

   for (i = 0; i < len - 1; i++) {
      indis = i;
      for (j = i; j < len; j++)
         //if (a[b[j]] < a[b[indis]])
         if(get(b[j], "Index") < get(b[indis], "Index"))
         {
             indis = j;
         }

      temp = b[indis];
      b[indis] = b[i];
      b[i] = temp;
   }

   int *return_index = malloc(sizeof(int) * (v_page_table->num_virtual * v_page_table-> frame_size / 4));
   for(i = 0 ; i < (v_page_table->num_virtual * v_page_table-> frame_size / 4) ;++i)
   {
       return_index[i] = b[i];
   }

   return return_index;
}


int
partition(int low, int high)
{
    int first, second;
    int pivot;
    int i, j;
    int temp;
    pivot = get(high, "Quick");
    i = low - 1;
    for(j = low ; j <= high - 1 ; ++j)
    {
        if(get(j, "Quick") < pivot)
        {
            i++;
            temp = get(i, "Quick");
            set(i, get(j, "Quick"), "Quick");
            set(j, temp, "Quick");
        }
    }
    temp = get(i + 1, "Quick");
    set(i+1, get(high, "Quick"), "Quick");
    set(high,temp, "Quick" );

    return i + 1;
}


void
quick_sort(int low, int high)
{
    int divide;
    if(low < high)
    {
        divide = partition(low, high);
        quick_sort(low, divide - 1);
        quick_sort(divide + 1, high);
    }
}

void merging(int low, int mid, int high)
{

    int b[(v_page_table -> num_virtual) * (v_page_table -> frame_size) / 4  - 1];
    int l1,l2,i;

    for(l1 = low, l2 = mid + 1, i = low; l1 <= mid && l2 <= high ; ++i)
    {
        if(get(l1, "Merge") <= get(l2, "Merge"))
        {
            b[i] = get(l1++, "Merge");
        }
        else
        {
            b[i] = get(l2++, "Merge");
        }
    }

    while(l1 <= mid)
    {
        b[i++] = get(l1++, "Merge");
    }

    while(l2 <= high)
    {
        b[i++] = get(l2++, "Merge");
    }

    for(i = low; i <= high ; ++i)
    {
        set(i, b[i], "Merge");
    }


    //free(b);
}


void sort(int low, int high)
{
    int mid;

    if(low <  high)
    {
        mid = (low + high) / 2;
        sort(low, mid);
        sort(mid + 1, high);
        merging(low, mid, high);
    }
    else
    {
        return ;
    }
}






void
check_arguments(unsigned int frame_size,
                unsigned int num_physical,
                unsigned int num_virtual,
                char page_replacement[10],
                char alloc_policy[10],
                unsigned int page_table_print_int)
{

    if(frame_size < 0 || num_physical < 0 || num_virtual < 0 || page_table_print_int < 0 )
    {
        fprintf(stderr, "%s\n", "Arguments must be greater than zero. Exiting program." );
        exit(1);
    }
    if(strcmp(page_replacement, "NRU") == 0 ||
       strcmp(page_replacement, "FIFO") == 0 ||
       strcmp(page_replacement, "SC") == 0 ||
       strcmp(page_replacement, "LRU") == 0 ||
       strcmp(page_replacement, "WSClock") == 0)
    {
        //good
    }
    else
    {
        fprintf(stderr, "%s\n", "Unknown page replacement algorithm. Exiting program");
        exit(1);
    }

    if(strcmp(alloc_policy, "global") == 0 ||
       strcmp(alloc_policy, "local") == 0)
    {
        //good
    }
    else
    {
        fprintf(stderr, "%s\n", "Unknown allocation policy. Exiting program." );
        exit(1);
    }

    if(num_physical > num_virtual)
    {
        fprintf(stderr, "%s\n","Number of physical frames can not be bigger than virtual frames.");
        exit(EXIT_FAILURE);
    }
    if(num_physical == 1 || num_virtual == 1 || frame_size == 1)
    {
        fprintf(stderr, "%s\n", "Bad parameters(1).Exiting program");
        exit(EXIT_FAILURE);
    }

    /*printf("Frame size: %d\n", frame_size);
    printf("Physical frame count: %d\n", num_physical );
    printf("Virtual frame count: %d\n",num_virtual);
    printf("Page replacement policy: %s\n",page_replacement );
    printf("Allocation policy: %s\n",alloc_policy );
    printf("Page table interval for print: %d\n", page_table_print_int );*/

    if((strcmp(page_replacement, "LRU") == 0) && (strcmp(alloc_policy, "local") == 0))
    {
        fprintf(stderr, "%s\n", "LRU + local sorts 3/4 of virtual array with seg fault. Couldn't figure out the reason. Please try LRU + global." );
        exit(EXIT_FAILURE);
    }

}


void print_page_table(page_table *virtual_page_table)
{


    unsigned int page_frame_number; //which physical address it's maps.
    int present_absent; // is the data in memory or in disk.
    int dirty_bit; //is the page modified or not



    int i = 0;
    for(i = 0 ; i < virtual_page_table-> num_virtual ; ++i)
    {
        if(virtual_page_table -> table[i].present_absent == 1)
        {
            printf("%2d. page table entry[%3d - %3d] --> [%3d - %3d] || Dirty Bit: %d || Referenced Bit : %d\n", i, i * virtual_page_table -> frame_size, (i + 1) * virtual_page_table -> frame_size,
                  virtual_page_table -> table[i].page_frame_number * virtual_page_table -> frame_size, virtual_page_table -> table[i].page_frame_number * virtual_page_table -> frame_size + virtual_page_table -> frame_size
                  ,virtual_page_table -> table[i].dirty_bit, virtual_page_table -> table[i].referenced);

        }
        else if(virtual_page_table -> table[i].present_absent == 0)
        {
            printf("%2d. page table entry[%3d - %3d] --> [   DISK  ] || Dirty Bit: %d || Referenced Bit : %d \n", i, i * virtual_page_table -> frame_size,
                  (i + 1) * virtual_page_table -> frame_size,virtual_page_table -> table[i].dirty_bit,  virtual_page_table -> table[i].referenced);
        }


    }
}

void init_physical_memory(int frame_size, int num_physical)
{
    int i;
    for(i = 0 ; i < frame_size * num_physical ; ++i)
    {
        physical_mem[i] = PHYSICAL_MEM_DEFAULT;
    }
}


void
fill_virtual_memory(unsigned int frame_size, unsigned int num_physical, unsigned int num_virtual, char *disk_name)
{
    int i,j;
    int disk_offset = 0;
    int random_number= 5;
    FILE * DISK = fopen(disk_name, "w");
    //Create the file.
    if(DISK == NULL)
    {
        fprintf(stderr, "%s\n","Error creating disk file, exiting program..." );
        exit(EXIT_FAILURE);
    }

    fclose(DISK);
    //Open the same file again in r+ mode.
    DISK = fopen(disk_name, "r+");
    if(DISK == NULL)
    {
        fprintf(stderr, "%s\n","Error opening file. Exiting program." );
        exit(EXIT_FAILURE);
    }

    if(strcmp(v_page_table-> alloc_policy, "global") == 0)
    {


        for(i = 0 ; i < num_virtual ; ++i)
        {
            if(i < num_physical) // if physical adresses have empty slot.
            {
                v_page_table -> table[i].page_frame_number = i; // map the physical address
                v_page_table -> table[i].present_absent = 1; // maps to real physical address so yes.
                v_page_table -> table[i].dirty_bit = 0; //not modified so no.
                v_page_table -> table[i].referenced = 0;
                enQueue(i);
                set_i_nth_page(i, v_page_table->num_virtual,"global", -1);



                //start writing integer contents to disk and physical memory.
                for(j = 0 ; j < frame_size ; ++j)
                {

                    random_number = rand();
                    //printf("Generated random number %d\n",rand());
                    physical_mem[j + i * frame_size] = random_number; // initialize physical memory.
                    fprintf(DISK, "%d\n",physical_mem[j + i * frame_size]); //initialize same contents to disk too.
                    disk_offset += _JUMP;
                    fseek(DISK, disk_offset, SEEK_SET); //Seek forward to create some kind of boundary between integers in file.

                }
            }
            else // if we run out of physical memory.
            {
                v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY; //set the page frame number as not present in memory.
                v_page_table -> table[i].present_absent = 0; // The entry is in disk so set it to 0.
                v_page_table -> table[i].dirty_bit = 0; //Entry on disk and not even modified so set it to 0.
                v_page_table -> table[i].referenced = 0; //Entry on disk and not even referenced so set it to 0.
                //start writing content to disk.
                disk_page_reads_bubble_get++;
                disk_page_reads_quick_get++;
                disk_page_reads_merge_get++;
                disk_page_reads_index_get++;
                for(j = 0 ; j < frame_size ; ++j)
                {
                    random_number = rand();
                    //printf("Generated random number %d\n",rand());
                    //fprintf(stdout, "%d\n",random_number );
                    fprintf(DISK, "%d\n",random_number); //write to disk
                    disk_offset += _JUMP ;//seek forward
                    fseek(DISK, disk_offset, SEEK_SET);
                }
            }
        }
    }
    else if(strcmp(v_page_table-> alloc_policy, "local") == 0)
    {
        int k = 0;
        //printf("AVAILABLE PAGES2 %d\n",num_physical );
        int page_count = 0;
        //printf("??\n" );
        if(v_page_table -> num_physical < 4)
        {
            fprintf(stderr, "%s\n","Can't work with less pages than 4 for local policy. Exiting the program" );
            fclose(DISK);
            _exit(EXIT_FAILURE);
        }

        for(i = 0 ; i < v_page_table -> num_virtual / 4 ; ++i)
        {
            //printf("Disk offset is %d\n",disk_offset );
            if(page_count < num_physical / 4)
            {
                page_count++;

                v_page_table -> table[i].page_frame_number = k++; // map the physical address
                v_page_table -> table[i].present_absent = 1; // maps to real physical address so yes.
                v_page_table -> table[i].dirty_bit = 0; //not modified so no.
                v_page_table -> table[i].referenced = 0;
                enQueue_1(i);
                set_i_nth_page_first_quarter(i, v_page_table->num_virtual);

                for(j = 0 ; j < frame_size ; ++j)
                {

                    //fprintf(stdout, "%d\n",random_number );
                    random_number = rand();
                    //printf("Generated random number %d\n",rand());
                    physical_mem[j + i * frame_size] = random_number; // initialize physical memory.
                    fprintf(DISK, "%d\n",physical_mem[j + i * frame_size]); //initialize same contents to disk too.
                    disk_offset += _JUMP;
                    fseek(DISK, disk_offset, SEEK_SET); //Seek forward to create some kind of boundary between integers in file.
                }
            }
            else
            {
                disk_page_reads_bubble_get++;
                v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY; //set the page frame number as not present in memory.
                v_page_table -> table[i].present_absent = 0; // The entry is in disk so set it to 0.
                v_page_table -> table[i].dirty_bit = 0; //Entry on disk and not even modified so set it to 0.
                v_page_table -> table[i].referenced = 0; //Entry on disk and not even referenced so set it to 0.
                //start writing content to disk.
                for(j = 0 ; j < frame_size ; ++j)
                {

                    random_number = rand();
                    //printf("Generated random number %d\n",rand());
                    //fprintf(stdout, "%d\n",random_number );
                    fprintf(DISK, "%d\n",random_number); //write to disk
                    disk_offset += _JUMP ;//seek forward
                    fseek(DISK, disk_offset, SEEK_SET);
                }
            }
        }
        page_count = 0;


        for(i =  v_page_table -> num_virtual / 4 ; i <  v_page_table -> num_virtual / 2 ; ++i )
        {
            //printf("Disk offset is %d\n",disk_offset );
            if(page_count < num_physical / 4)
            {
                page_count++;


                v_page_table -> table[i].page_frame_number = k++; // map the physical address
                v_page_table -> table[i].present_absent = 1; // maps to real physical address so yes.
                v_page_table -> table[i].dirty_bit = 0; //not modified so no.
                v_page_table -> table[i].referenced = 0;
                enQueue_2(i);

                set_i_nth_page_second_quarter(i - v_page_table -> num_virtual / 4, v_page_table->num_virtual);
                for(j = 0 ; j < frame_size ; ++j)
                {

                    //fprintf(stdout, "%d\n",random_number );
                    random_number = rand();
                    //printf("Generated random number %d\n",rand());
                    physical_mem[j + (i - v_page_table -> num_virtual / 4 + num_physical / 4) * frame_size] = random_number; // initialize physical memory.
                    fprintf(DISK, "%d\n",physical_mem[j + (i - v_page_table -> num_virtual / 4  + num_physical / 4) * frame_size]); //initialize same contents to disk too.
                    disk_offset += _JUMP;
                    fseek(DISK, disk_offset, SEEK_SET); //Seek forward to create some kind of boundary between integers in file.


                    /*

                    random_number = rand();
                    printf("Generated random number %d\n",rand());
                    physical_mem[j + (i - v_page_table -> num_virtual / 2 + num_physical / 2) * frame_size] = random_number; // initialize physical memory.
                    fprintf(DISK, "%d\n",physical_mem[j + (i - v_page_table -> num_virtual / 2 + num_physical / 2) * frame_size]); //initialize same contents to disk too.
                    disk_offset += _JUMP;
                    fseek(DISK, disk_offset, SEEK_SET); //Seek forwar


                    */
                }
            }
            else
            {
                disk_page_reads_quick_get++;
                v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY; //set the page frame number as not present in memory.
                v_page_table -> table[i].present_absent = 0; // The entry is in disk so set it to 0.
                v_page_table -> table[i].dirty_bit = 0; //Entry on disk and not even modified so set it to 0.
                v_page_table -> table[i].referenced = 0; //Entry on disk and not even referenced so set it to 0.
                //start writing content to disk.
                for(j = 0 ; j < frame_size ; ++j)
                {
                    random_number = rand();
                    //printf("Generated random number %d\n",rand());
                    //fprintf(stdout, "%d\n",random_number );
                    fprintf(DISK, "%d\n",random_number); //write to disk
                    disk_offset += _JUMP ;//seek forward
                    fseek(DISK, disk_offset, SEEK_SET);
                }
            }
        }

        page_count = 0;

        for(i =  v_page_table -> num_virtual / 2 ; i <  v_page_table -> num_virtual * 3 / 4 ; ++i )
        {
            //printf("Disk offset is %d\n",disk_offset );
            if(page_count < num_physical / 4)
            {

                page_count++;


                v_page_table -> table[i].page_frame_number = k++; // map the physical address
                v_page_table -> table[i].present_absent = 1; // maps to real physical address so yes.
                v_page_table -> table[i].dirty_bit = 0; //not modified so no.
                v_page_table -> table[i].referenced = 0;
                enQueue_3(i);
                set_i_nth_page_third_quarter(i - v_page_table -> num_virtual / 2, v_page_table->num_virtual);

                for(j = 0 ; j < frame_size ; ++j)
                {

                    //fprintf(stdout, "%d\n",random_number );
                    random_number = rand();
                    //printf("Generated random number %d\n",rand());
                    physical_mem[j + (i - v_page_table -> num_virtual / 2 + num_physical / 2) * frame_size] = random_number; // initialize physical memory.
                    fprintf(DISK, "%d\n",physical_mem[j + (i - v_page_table -> num_virtual / 2 + num_physical / 2) * frame_size]); //initialize same contents to disk too.
                    disk_offset += _JUMP;
                    fseek(DISK, disk_offset, SEEK_SET); //Seek forward to create some kind of boundary between integers in file.
                }
            }
            else
            {
                disk_page_reads_merge_get++;
                v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY; //set the page frame number as not present in memory.
                v_page_table -> table[i].present_absent = 0; // The entry is in disk so set it to 0.
                v_page_table -> table[i].dirty_bit = 0; //Entry on disk and not even modified so set it to 0.
                v_page_table -> table[i].referenced = 0; //Entry on disk and not even referenced so set it to 0.
                //start writing content to disk.
                for(j = 0 ; j < frame_size ; ++j)
                {
                    random_number = rand();
                    //printf("Generated random number %d\n",rand());
                    //fprintf(stdout, "%d\n",random_number );
                    fprintf(DISK, "%d\n",random_number); //write to disk
                    disk_offset += _JUMP ;//seek forward
                    fseek(DISK, disk_offset, SEEK_SET);
                }
            }
        }

        page_count = 0;

        for(i =  v_page_table -> num_virtual * 3 / 4 ; i <  v_page_table -> num_virtual ; ++i )
        {
            //printf("Disk offset is %d\n",disk_offset );
            if(page_count < num_physical / 4)
            {
                page_count++;

                v_page_table -> table[i].page_frame_number = k++; // map the physical address
                v_page_table -> table[i].present_absent = 1; // maps to real physical address so yes.
                v_page_table -> table[i].dirty_bit = 0; //not modified so no.
                v_page_table -> table[i].referenced = 0;
                enQueue_4(i);
                set_i_nth_page_fourth_quarter(i - v_page_table -> num_virtual * 3 / 4, v_page_table->num_virtual);
                for(j = 0 ; j < frame_size ; ++j)
                {
                    //fprintf(stdout, "%d\n",random_number );

                    random_number = rand();
                    //printf("Generated random number %d\n",rand());
                    physical_mem[j + (i - v_page_table -> num_virtual * 3 / 4 + num_physical * 3 / 4) * frame_size] = random_number; // initialize physical memory.
                    fprintf(DISK, "%d\n",physical_mem[j + (i - v_page_table -> num_virtual * 3 / 4 + num_physical * 3 / 4) * frame_size]); //initialize same contents to disk too.
                    disk_offset += _JUMP;
                    fseek(DISK, disk_offset, SEEK_SET); //Seek forward to create some kind of boundary between integers in file.
                }
            }
            else
            {
                disk_page_reads_index_get++;
                v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY; //set the page frame number as not present in memory.
                v_page_table -> table[i].present_absent = 0; // The entry is in disk so set it to 0.
                v_page_table -> table[i].dirty_bit = 0; //Entry on disk and not even modified so set it to 0.
                v_page_table -> table[i].referenced = 0; //Entry on disk and not even referenced so set it to 0.
                //start writing content to disk.
                for(j = 0 ; j < frame_size ; ++j)
                {
                    random_number = rand();
                    //printf("Generated random number %d\n",rand());
                    //fprintf(stdout, "%d\n",random_number );
                    fprintf(DISK, "%d\n",random_number); //write to disk
                    disk_offset += _JUMP ;//seek forward
                    fseek(DISK, disk_offset, SEEK_SET);
                }
            }
        }


    }

    fclose(DISK);

}



void
create_page_table(unsigned int frame_size,
                     unsigned int num_physical,
                     unsigned int num_virtual,
                     char page_replacement[10],
                     char alloc_policy[10],
                     unsigned int page_table_print_int,
                     char disk_name[30])
{


    //Allocate memory for virtual page table and all entries.
    v_page_table =  calloc(1,sizeof(page_table));
    if(v_page_table == NULL)
    {
        fprintf(stderr, "%s\n", "malloc fail");
    }
    v_page_table -> table = calloc(num_virtual, sizeof(page_table_entry) );
    if(v_page_table == NULL)
    {
        fprintf(stderr, "%s\n", "malloc fail");
    }
    v_page_table -> num_virtual = num_virtual;
    v_page_table -> frame_size = frame_size;
    v_page_table -> num_physical = num_physical;
    v_page_table -> page_table_print_int = page_table_print_int;
    strcpy(v_page_table -> page_replacement, page_replacement);
    strcpy(v_page_table -> alloc_policy, alloc_policy);
    strcpy(v_page_table -> disk_name, disk_name);


}


void
free_page_table(void)
{
    free(v_page_table -> table);
    free(v_page_table);
}


void
create_physical_memory(int frame_size, int num_physical)
{
    physical_mem = malloc(sizeof(unsigned int) * frame_size * num_physical);
}

void free_physical_memory(void)
{
    free(physical_mem);
}

void create_page_table_FIFO(void)
{

}

void create_page_table_SC(void)
{

}

void create_page_table_LRU(void)
{

}

void create_page_table_WSClock(void)
{

}
void
print_physical_memory(int num_physical, int frame_size)
{
    int i;
    for(i = 0 ; i < num_physical * frame_size ; ++i)
    {
        printf("physical_mem[%d] = %d\n",i, physical_mem[i]);
    }
}


int get(unsigned int index, char * tName)
{

    pthread_mutex_lock(&lock);
    int return_value;
    int disk_offset = 0;
    char page_replacement[10];
    char alloc_policy[10];
    char disk_name[30];
    int page_no;
    int offset;
    int i,j;
    int num_physical = v_page_table -> num_physical;
    int num_virtual = v_page_table -> num_virtual;
    int frame_size = v_page_table -> frame_size;
    int page_table_print_int = v_page_table -> page_table_print_int;
    strcpy(page_replacement, v_page_table -> page_replacement);
    strcpy(alloc_policy, v_page_table -> alloc_policy);
    strcpy(disk_name, v_page_table -> disk_name);

    //Write
    int found_first_class = 0;
    int found_second_class = 0;
    int found_third_class = 0;
    int found_fourth_class = 0;
    int old_address;
    int number;

    if(strcmp(page_replacement, "NRU") == 0)
    {
        if(strcmp(tName, "Bubble") == 0)
        {
            //index is fine
            if((reads_bubble_get + writes_bubble_set) % page_table_print_int == 0)
            {

                //print_page_table(v_page_table);

                for(i = 0 ; i < num_virtual ; ++i)
                {
                    v_page_table->table[i].referenced = 0;
                }
            }

        }
        else if(strcmp(tName, "Quick") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4;

        }
        else if(strcmp(tName, "Merge") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 2;

        }

        else if(strcmp(tName, "Index") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4 * 3;

        }

        else
        {
            fprintf(stderr, "%s\n", "No such thread name. Something is wrong. Exiting the program");
            exit(EXIT_FAILURE);
        }
        if(strcmp(tName, "Bubble") == 0)
        {
                    reads_bubble_get++;
                    FILE * DISK = fopen(disk_name, "r+");
                    if(DISK == NULL)
                    {
                        fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                        exit(EXIT_FAILURE);
                    }

                    page_no = (int) (index / frame_size); //find the page number.
                    offset = index % frame_size; //find the offset on that page.

                    //if the specified page is present in virtual memory, no problem simply return it.
                    if(v_page_table -> table[page_no].present_absent == 1)
                    {

                        //don't forget to set referenced bit
                        v_page_table -> table[page_no].referenced = 1;
                        fclose(DISK);
                        return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                        pthread_mutex_unlock(&lock);
                        return return_value;
                    }
                    //if the specified page is not present in virtual page well, there we go...
                    else if(v_page_table -> table[page_no].present_absent == 0)
                    {

                        page_misses_bubble_get++;
                        //find a victim to swap out from page table
                        // and store it contents into disk file.

                        //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                            //First Category: Not referenced / Not modified
                            //Second Category: Not referenced / modified
                            //Third Category: Referenced / Not modified
                            //Fouth Category: Referenced / Modified

                            //Lower priority class will be cheaper to evict.

                            //First Category Search:

                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {

                                for(i = 0 ; i < num_virtual ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 0
                                       && v_page_table->table[i].dirty_bit == 0)
                                    {
                                           //found class 0;
                                           //printf("Three\n");
                                           found_first_class = 1;
                                           break;
                                    }

                                }

                                //Second category search:

                                if((found_first_class == 0))
                                {
                                    for(i = 0 ; (i < num_virtual)  ; ++i)
                                    {
                                        if(v_page_table->table[i].present_absent == 1
                                           && v_page_table->table[i].referenced == 0
                                           && v_page_table->table[i].dirty_bit == 1)
                                        {
                                            //printf("Little\n");
                                               //found class 1;
                                               found_second_class = 1;
                                               break;
                                        }
                                    }
                                }

                                //Third category search:
                                if((found_second_class == 0) && (found_first_class == 0))
                                {
                                    for(i = 0 ; (i < num_virtual)   ; ++i)
                                    {
                                        if(v_page_table->table[i].present_absent == 1
                                           && v_page_table->table[i].referenced == 1
                                           && v_page_table->table[i].dirty_bit == 0)
                                        {
                                            //printf("Birds\n");
                                               //found class 1;
                                               found_third_class = 1;
                                               break;
                                        }
                                    }
                                }

                                if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                                {
                                    for(i = 0 ; (i < num_virtual) ; ++i)
                                    {
                                        if(v_page_table->table[i].present_absent == 1
                                           && v_page_table->table[i].referenced == 1
                                           && v_page_table->table[i].dirty_bit == 1)
                                        {
                                            //printf("Lol\n");
                                               //found class 1;
                                               found_fourth_class = 1;
                                               break;
                                        }
                                    }
                                }
                            }
                            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {
                                if(strcmp(tName, "Bubble") == 0)
                                {

                                    for(i = 0 ; i < num_virtual / 4 ; ++i)
                                    {
                                        if(v_page_table->table[i].present_absent == 1
                                           && v_page_table->table[i].referenced == 0
                                           && v_page_table->table[i].dirty_bit == 0)
                                        {
                                               //found class 0;
                                               //printf("Three\n");
                                               found_first_class = 1;
                                               break;
                                        }

                                    }

                                    //Second category search:

                                    if((found_first_class == 0))
                                    {
                                        for(i = 0 ; i < num_virtual / 4 ; ++i)
                                        {
                                            if(v_page_table->table[i].present_absent == 1
                                               && v_page_table->table[i].referenced == 0
                                               && v_page_table->table[i].dirty_bit == 1)
                                            {
                                                //printf("Little\n");
                                                   //found class 1;
                                                   found_second_class = 1;
                                                   break;
                                            }
                                        }
                                    }

                                    //Third category search:
                                    if((found_second_class == 0) && (found_first_class == 0))
                                    {
                                        for(i = 0 ; i < num_virtual / 4 ; ++i)
                                        {
                                            if(v_page_table->table[i].present_absent == 1
                                               && v_page_table->table[i].referenced == 1
                                               && v_page_table->table[i].dirty_bit == 0)
                                            {
                                                //printf("Birds\n");
                                                   //found class 1;
                                                   found_third_class = 1;
                                                   break;
                                            }
                                        }
                                    }

                                    if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                                    {
                                        for(i = 0 ; i < num_virtual / 4 ; ++i)
                                        {
                                            if(v_page_table->table[i].present_absent == 1
                                               && v_page_table->table[i].referenced == 1
                                               && v_page_table->table[i].dirty_bit == 1)
                                            {
                                                //printf("Lol\n");
                                                   //found class 1;
                                                   found_fourth_class = 1;
                                                   break;
                                            }
                                        }
                                    }
                                }

                                }



                            //Fourth category search:


                                //printf("HEEEEEEEEEY2\n" );
                            /*if(found_first_class)
                            {
                                printf("First class found\n" );
                            }
                            else if(found_second_class)
                            {
                                printf("Second class found\n");
                            }
                            else if(found_third_class)
                            {
                                printf("Third class found\n");
                            }
                            else if(found_fourth_class)
                            {
                                printf("Fourth class found\n" );
                            }
                            printf("%d. page\n",i );*/


                            old_address = v_page_table -> table[i].page_frame_number; //save page no.

                            v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                            if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                            {
                                disk_page_writes_bubble_get++;
                                v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                                disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                                fseek(DISK, disk_offset, SEEK_SET);
                                for(j = 0 ; j < frame_size ; ++j)
                                {
                                    //store the contents of that page into disk.
                                    fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                                    disk_offset += _JUMP;
                                    fseek(DISK, disk_offset, SEEK_SET);
                                }
                            }

                            v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                            v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                            disk_offset = page_no * frame_size * _JUMP;
                            v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;
                            disk_page_reads_bubble_get++;
                            page_replacements_bubble_get++;
                            fseek(DISK, disk_offset, SEEK_SET);
                            for(i = 0 ; i < frame_size ; ++i)
                            {
                                fscanf(DISK, "%d\n", &number);
                                physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                                disk_offset += _JUMP;
                                fseek(DISK, disk_offset, SEEK_SET);
                            }
                            fclose(DISK);
                            return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                            pthread_mutex_unlock(&lock);
                            return return_value;




                    }
                fclose(DISK);

                return -1;
        }
        else if(strcmp(tName, "Quick") == 0)
        {
            reads_quick_get++;
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }

            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_quick_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        for(i = 0 ; i < num_virtual ; ++i)
                        {
                            if(v_page_table->table[i].present_absent == 1
                               && v_page_table->table[i].referenced == 0
                               && v_page_table->table[i].dirty_bit == 0)
                            {
                                   //found class 0;
                                   //printf("Three\n");
                                   found_first_class = 1;
                                   break;
                            }

                        }

                        //Second category search:

                        if((found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)  ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                    //printf("Little\n");
                                       //found class 1;
                                       found_second_class = 1;
                                       break;
                                }
                            }
                        }

                        //Third category search:
                        if((found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)   ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                    //printf("Birds\n");
                                       //found class 1;
                                       found_third_class = 1;
                                       break;
                                }
                            }
                        }

                        if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual) ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                    //printf("Lol\n");
                                       //found class 1;
                                       found_fourth_class = 1;
                                       break;
                                }
                            }
                        }
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        if(strcmp(tName, "Quick") == 0)
                        {
                            for(i = num_virtual / 4 ;(i < num_virtual / 2) ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                       //found class 0;
                                       //printf("Three\n");
                                       found_first_class = 1;
                                       break;
                                }

                            }

                            //Second category search:

                            if((found_first_class == 0))
                            {
                                for(i = num_virtual / 4 ;(i < num_virtual / 2) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 0
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Little\n");
                                           //found class 1;
                                           found_second_class = 1;
                                           break;
                                    }
                                }
                            }

                            //Third category search:
                            if((found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual / 4 ;(i < num_virtual / 2) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 0)
                                    {
                                        //printf("Birds\n");
                                           //found class 1;
                                           found_third_class = 1;
                                           break;
                                    }
                                }
                            }

                            if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual / 4 ;(i < num_virtual / 2) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Lol\n");
                                           //found class 1;
                                           found_fourth_class = 1;
                                           break;
                                    }
                                }
                            }
                        }

                        }



                    //Fourth category search:


                        //printf("HEEEEEEEEEY2\n" );
                    /*if(found_first_class)
                    {
                        printf("First class found\n" );
                    }
                    else if(found_second_class)
                    {
                        printf("Second class found\n");
                    }
                    else if(found_third_class)
                    {
                        printf("Third class found\n");
                    }
                    else if(found_fourth_class)
                    {
                        printf("Fourth class found\n" );
                    }
                    printf("%d. page\n",i );*/


                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_quick_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }
                    disk_page_reads_quick_get++;
                    page_replacements_quick_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }
        else if(strcmp(tName, "Merge") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            reads_merge_get++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_merge_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        for(i = 0 ; i < num_virtual ; ++i)
                        {
                            if(v_page_table->table[i].present_absent == 1
                               && v_page_table->table[i].referenced == 0
                               && v_page_table->table[i].dirty_bit == 0)
                            {
                                   //found class 0;
                                   //printf("Three\n");
                                   found_first_class = 1;
                                   break;
                            }

                        }

                        //Second category search:

                        if((found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)  ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                    //printf("Little\n");
                                       //found class 1;
                                       found_second_class = 1;
                                       break;
                                }
                            }
                        }

                        //Third category search:
                        if((found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)   ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                    //printf("Birds\n");
                                       //found class 1;
                                       found_third_class = 1;
                                       break;
                                }
                            }
                        }

                        if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual) ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                    //printf("Lol\n");
                                       //found class 1;
                                       found_fourth_class = 1;
                                       break;
                                }
                            }
                        }
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {


                        if(strcmp(tName, "Merge") == 0)
                        {
                            for(i = num_virtual / 2 ; (i < num_virtual * 3 / 4)   ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                       //found class 0;
                                       //printf("Three\n");
                                       found_first_class = 1;
                                       break;
                                }

                            }

                            //Second category search:

                            if((found_first_class == 0))
                            {
                                for(i = num_virtual / 2 ; (i < num_virtual * 3 / 4)   ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 0
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Little\n");
                                           //found class 1;
                                           found_second_class = 1;
                                           break;
                                    }
                                }
                            }

                            //Third category search:
                            if((found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual / 2 ; (i < num_virtual * 3 / 4)   ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 0)
                                    {
                                        //printf("Birds\n");
                                           //found class 1;
                                           found_third_class = 1;
                                           break;
                                    }
                                }
                            }

                            if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual / 2 ; (i < num_virtual * 3 / 4)   ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Lol\n");
                                           //found class 1;
                                           found_fourth_class = 1;
                                           break;
                                    }
                                }
                            }
                        }

                        }



                    //Fourth category search:


                        //printf("HEEEEEEEEEY2\n" );
                    /*if(found_first_class)
                    {
                        printf("First class found\n" );
                    }
                    else if(found_second_class)
                    {
                        printf("Second class found\n");
                    }
                    else if(found_third_class)
                    {
                        printf("Third class found\n");
                    }
                    else if(found_fourth_class)
                    {
                        printf("Fourth class found\n" );
                    }
                    printf("%d. page\n",i );*/


                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_merge_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_merge_get++;
                    page_replacements_merge_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }

        else if(strcmp(tName, "Index") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            reads_index_get++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_index_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        for(i = 0 ; i < num_virtual ; ++i)
                        {
                            if(v_page_table->table[i].present_absent == 1
                               && v_page_table->table[i].referenced == 0
                               && v_page_table->table[i].dirty_bit == 0)
                            {
                                   //found class 0;
                                   //printf("Three\n");
                                   found_first_class = 1;
                                   break;
                            }

                        }

                        //Second category search:

                        if((found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)  ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                    //printf("Little\n");
                                       //found class 1;
                                       found_second_class = 1;
                                       break;
                                }
                            }
                        }

                        //Third category search:
                        if((found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)   ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                    //printf("Birds\n");
                                       //found class 1;
                                       found_third_class = 1;
                                       break;
                                }
                            }
                        }

                        if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual) ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                    //printf("Lol\n");
                                       //found class 1;
                                       found_fourth_class = 1;
                                       break;
                                }
                            }
                        }
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        if(strcmp(tName, "Index") == 0)
                        {
                            for(i = num_virtual * 3 / 4 ; (i < num_virtual) ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                       //found class 0;
                                       //printf("Three\n");
                                       found_first_class = 1;
                                       break;
                                }

                            }

                            //Second category search:

                            if((found_first_class == 0))
                            {
                                for(i = num_virtual * 3 / 4 ; (i < num_virtual) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 0
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Little\n");
                                           //found class 1;
                                           found_second_class = 1;
                                           break;
                                    }
                                }
                            }

                            //Third category search:
                            if((found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual * 3 / 4 ; (i < num_virtual) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 0)
                                    {
                                        //printf("Birds\n");
                                           //found class 1;
                                           found_third_class = 1;
                                           break;
                                    }
                                }
                            }

                            if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual * 3 / 4 ; (i < num_virtual) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Lol\n");
                                           //found class 1;
                                           found_fourth_class = 1;
                                           break;
                                    }
                                }
                            }
                        }
                    }




                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_index_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_index_get++;
                    page_replacements_index_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }
    }

    else if(strcmp(page_replacement, "FIFO") == 0)
    {
        if(strcmp(tName, "Bubble") == 0)
        {
            //index is fine
            if((reads_bubble_get + writes_bubble_set) % page_table_print_int == 0)
            {

                //print_page_table(v_page_table);

            }

        }
        else if(strcmp(tName, "Quick") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4;

        }
        else if(strcmp(tName, "Merge") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 2;

        }

        else if(strcmp(tName, "Index") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4 * 3;

        }
        else
        {
            fprintf(stderr, "%s\n", "No such thread name. Something is wrong. Exiting the program");
            exit(EXIT_FAILURE);
        }
        if(strcmp(tName, "Bubble") == 0)
        {
                    reads_bubble_get++;
                    FILE * DISK = fopen(disk_name, "r+");
                    if(DISK == NULL)
                    {
                        fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                        exit(EXIT_FAILURE);
                    }

                    page_no = (int) (index / frame_size); //find the page number.
                    offset = index % frame_size; //find the offset on that page.

                    //if the specified page is present in virtual memory, no problem simply return it.
                    if(v_page_table -> table[page_no].present_absent == 1)
                    {

                        //don't forget to set referenced bit
                        v_page_table -> table[page_no].referenced = 1;
                        fclose(DISK);
                        return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                        pthread_mutex_unlock(&lock);
                        return return_value;
                    }
                    //if the specified page is not present in virtual page well, there we go...
                    else if(v_page_table -> table[page_no].present_absent == 0)
                    {

                        page_misses_bubble_get++;
                        //find a victim to swap out from page table
                        // and store it contents into disk file.

                        //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                            //First Category: Not referenced / Not modified
                            //Second Category: Not referenced / modified
                            //Third Category: Referenced / Not modified
                            //Fouth Category: Referenced / Modified

                            //Lower priority class will be cheaper to evict.

                            //First Category Search:
                            //printf("HEEEEEEEEEY\n" );
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {

                                i = deQueue();

                            }
                            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {

                                i = deQueue_1();
                            }



                            //Fourth category search:


                                //printf("HEEEEEEEEEY2\n" );
                            /*if(found_first_class)
                            {
                                printf("First class found\n" );
                            }
                            else if(found_second_class)
                            {
                                printf("Second class found\n");
                            }
                            else if(found_third_class)
                            {
                                printf("Third class found\n");
                            }
                            else if(found_fourth_class)
                            {
                                printf("Fourth class found\n" );
                            }
                            printf("%d. page\n",i );*/


                            old_address = v_page_table -> table[i].page_frame_number; //save page no.

                            v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                            if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                            {
                                disk_page_writes_bubble_get++;
                                v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                                disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                                fseek(DISK, disk_offset, SEEK_SET);
                                for(j = 0 ; j < frame_size ; ++j)
                                {
                                    //store the contents of that page into disk.
                                    fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                                    disk_offset += _JUMP;
                                    fseek(DISK, disk_offset, SEEK_SET);
                                }
                            }
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                enQueue(page_no);
                            }


                            if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {
                                enQueue_1(page_no);
                            }
                            v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                            v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                            disk_offset = page_no * frame_size * _JUMP;
                            v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;
                            disk_page_reads_bubble_get++;
                            page_replacements_bubble_get++;
                            fseek(DISK, disk_offset, SEEK_SET);
                            for(i = 0 ; i < frame_size ; ++i)
                            {
                                fscanf(DISK, "%d\n", &number);
                                physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                                disk_offset += _JUMP;
                                fseek(DISK, disk_offset, SEEK_SET);
                            }
                            fclose(DISK);
                            return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                            pthread_mutex_unlock(&lock);
                            return return_value;




                    }
                fclose(DISK);

                return -1;
        }
        else if(strcmp(tName, "Quick") == 0)
        {
            reads_quick_get++;
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }

            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_quick_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        i = deQueue();
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        i = deQueue_2();
                    }





                    //Fourth category search:


                        //printf("HEEEEEEEEEY2\n" );
                    /*if(found_first_class)
                    {
                        printf("First class found\n" );
                    }
                    else if(found_second_class)
                    {
                        printf("Second class found\n");
                    }
                    else if(found_third_class)
                    {
                        printf("Third class found\n");
                    }
                    else if(found_fourth_class)
                    {
                        printf("Fourth class found\n" );
                    }
                    printf("%d. page\n",i );*/


                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_quick_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }
                    disk_page_reads_quick_get++;
                    page_replacements_quick_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_2(page_no);
                    }
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }
        else if(strcmp(tName, "Merge") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            reads_merge_get++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_merge_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        i = deQueue();
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {

                        i = deQueue_3();

                    }






                    //Fourth category search:


                        //printf("HEEEEEEEEEY2\n" );
                    /*if(found_first_class)
                    {
                        printf("First class found\n" );
                    }
                    else if(found_second_class)
                    {
                        printf("Second class found\n");
                    }
                    else if(found_third_class)
                    {
                        printf("Third class found\n");
                    }
                    else if(found_fourth_class)
                    {
                        printf("Fourth class found\n" );
                    }
                    printf("%d. page\n",i );*/


                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_merge_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_merge_get++;
                    page_replacements_merge_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_3(page_no);
                    }

                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }

        else if(strcmp(tName, "Index") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            reads_index_get++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_index_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        i = deQueue();
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        i = deQueue_4();
                    }





                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_index_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_index_get++;
                    page_replacements_index_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_4(page_no);
                    }
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }

        //pthread_mutex_unlock(&lock);
        //exit(EXIT_FAILURE);
    }
    else if(strcmp(page_replacement, "SC" ) == 0)
    {
        if(strcmp(tName, "Bubble") == 0)
        {
            //index is fine
            if((reads_bubble_get + writes_bubble_set) % page_table_print_int == 0)
            {


                //print_page_table(v_page_table);

            }

        }
        else if(strcmp(tName, "Quick") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4;

        }
        else if(strcmp(tName, "Merge") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 2;

        }

        else if(strcmp(tName, "Index") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4 * 3;

        }
        else
        {
            fprintf(stderr, "%s\n", "No such thread name. Something is wrong. Exiting the program");
            exit(EXIT_FAILURE);
        }
        if(strcmp(tName, "Bubble") == 0)
        {
                    reads_bubble_get++;
                    FILE * DISK = fopen(disk_name, "r+");
                    if(DISK == NULL)
                    {
                        fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                        exit(EXIT_FAILURE);
                    }

                    page_no = (int) (index / frame_size); //find the page number.
                    offset = index % frame_size; //find the offset on that page.

                    //if the specified page is present in virtual memory, no problem simply return it.
                    if(v_page_table -> table[page_no].present_absent == 1)
                    {

                        //don't forget to set referenced bit
                        v_page_table -> table[page_no].referenced = 1;
                        fclose(DISK);
                        return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                        pthread_mutex_unlock(&lock);
                        return return_value;
                    }
                    //if the specified page is not present in virtual page well, there we go...
                    else if(v_page_table -> table[page_no].present_absent == 0)
                    {

                        page_misses_bubble_get++;
                        //find a victim to swap out from page table
                        // and store it contents into disk file.

                        //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                            //First Category: Not referenced / Not modified
                            //Second Category: Not referenced / modified
                            //Third Category: Referenced / Not modified
                            //Fouth Category: Referenced / Modified

                            //Lower priority class will be cheaper to evict.

                            //First Category Search:
                            //printf("HEEEEEEEEEY\n" );
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                int candidate_page;
                                int found = 1;
                                int temp;
                                while(found)
                                {
                                    candidate_page = deQueue();
                                    if(v_page_table->table[candidate_page].referenced == 1)
                                    {
                                        // dont evict this page.
                                        v_page_table->table[candidate_page].referenced = 0;
                                        enQueue(candidate_page);
                                    }
                                    else
                                    {
                                        i = candidate_page;
                                        found = 0;
                                    }
                                }


                            }
                            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {

                                int candidate_page;
                                int found = 1;
                                int temp;
                                while(found)
                                {
                                    candidate_page = deQueue_1();
                                    if(v_page_table->table[candidate_page].referenced == 1)
                                    {
                                        // dont evict this page.
                                        v_page_table->table[candidate_page].referenced = 0;
                                        enQueue_1(candidate_page);
                                    }
                                    else
                                    {
                                        i = candidate_page;
                                        found = 0;
                                    }
                                }
                            }



                            //Fourth category search:


                                //printf("HEEEEEEEEEY2\n" );
                            /*if(found_first_class)
                            {
                                printf("First class found\n" );
                            }
                            else if(found_second_class)
                            {
                                printf("Second class found\n");
                            }
                            else if(found_third_class)
                            {
                                printf("Third class found\n");
                            }
                            else if(found_fourth_class)
                            {
                                printf("Fourth class found\n" );
                            }
                            printf("%d. page\n",i );*/


                            old_address = v_page_table -> table[i].page_frame_number; //save page no.

                            v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                            if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                            {
                                disk_page_writes_bubble_get++;
                                v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                                disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                                fseek(DISK, disk_offset, SEEK_SET);
                                for(j = 0 ; j < frame_size ; ++j)
                                {
                                    //store the contents of that page into disk.
                                    fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                                    disk_offset += _JUMP;
                                    fseek(DISK, disk_offset, SEEK_SET);
                                }
                            }
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                enQueue(page_no);
                            }


                            if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {
                                enQueue_1(page_no);
                            }
                            v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                            v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                            disk_offset = page_no * frame_size * _JUMP;
                            v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;
                            disk_page_reads_bubble_get++;
                            page_replacements_bubble_get++;
                            fseek(DISK, disk_offset, SEEK_SET);
                            for(i = 0 ; i < frame_size ; ++i)
                            {
                                fscanf(DISK, "%d\n", &number);
                                physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                                disk_offset += _JUMP;
                                fseek(DISK, disk_offset, SEEK_SET);
                            }
                            fclose(DISK);
                            return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                            pthread_mutex_unlock(&lock);
                            return return_value;




                    }
                fclose(DISK);

                return -1;
        }
        else if(strcmp(tName, "Quick") == 0)
        {
            reads_quick_get++;
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }

            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_quick_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;

                                enQueue(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }

                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue_2();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue_2(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }
                    }





                    //Fourth category search:


                        //printf("HEEEEEEEEEY2\n" );
                    /*if(found_first_class)
                    {
                        printf("First class found\n" );
                    }
                    else if(found_second_class)
                    {
                        printf("Second class found\n");
                    }
                    else if(found_third_class)
                    {
                        printf("Third class found\n");
                    }
                    else if(found_fourth_class)
                    {
                        printf("Fourth class found\n" );
                    }
                    printf("%d. page\n",i );*/


                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_quick_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }
                    disk_page_reads_quick_get++;
                    page_replacements_quick_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_2(page_no);
                    }
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }
        else if(strcmp(tName, "Merge") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            reads_merge_get++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_merge_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }

                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {

                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue_3();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue_3(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }

                    }






                    //Fourth category search:


                        //printf("HEEEEEEEEEY2\n" );
                    /*if(found_first_class)
                    {
                        printf("First class found\n" );
                    }
                    else if(found_second_class)
                    {
                        printf("Second class found\n");
                    }
                    else if(found_third_class)
                    {
                        printf("Third class found\n");
                    }
                    else if(found_fourth_class)
                    {
                        printf("Fourth class found\n" );
                    }
                    printf("%d. page\n",i );*/


                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_merge_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_merge_get++;
                    page_replacements_merge_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_3(page_no);
                    }

                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }

        else if(strcmp(tName, "Index") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            reads_index_get++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_index_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue_4();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue_4(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }
                    }





                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_index_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_index_get++;
                    page_replacements_index_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_4(page_no);
                    }
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }
        //pthread_mutex_unlock(&lock);
        //exit(EXIT_FAILURE);
    }
    else if(strcmp(page_replacement, "LRU") == 0)
    {
        if(strcmp(tName, "Bubble") == 0)
        {
            //index is fine
            if((reads_bubble_get + writes_bubble_set) % page_table_print_int == 0)
            {

                //print_page_table(v_page_table);

            }

        }
        else if(strcmp(tName, "Quick") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4;

        }
        else if(strcmp(tName, "Merge") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 2;

        }

        else if(strcmp(tName, "Index") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4 * 3;

        }
        else
        {
            fprintf(stderr, "%s\n", "No such thread name. Something is wrong. Exiting the program");
            exit(EXIT_FAILURE);
        }
        if(strcmp(tName, "Bubble") == 0)
        {
                    reads_bubble_get++;

                    FILE * DISK = fopen(disk_name, "r+");
                    if(DISK == NULL)
                    {
                        fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                        exit(EXIT_FAILURE);
                    }

                    page_no = (int) (index / frame_size); //find the page number.
                    offset = index % frame_size; //find the offset on that page.

                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        set_i_nth_page(page_no, v_page_table -> num_virtual, "global", -1); // set referenced for LRU
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        int temp_1;
                        if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                        {

                            set_i_nth_page_first_quarter(page_no, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                        {
                            temp_1 = page_no -v_page_table -> num_virtual / 4;
                            set_i_nth_page_second_quarter(temp_1, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                        {
                            temp_1 = page_no - v_page_table -> num_virtual / 2;
                            set_i_nth_page_third_quarter(temp_1, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                        {
                            temp_1 = page_no -v_page_table -> num_virtual * 3 / 4;
                            set_i_nth_page_fourth_quarter(temp_1, v_page_table->num_virtual);
                        }
                    }

                    //if the specified page is present in virtual memory, no problem simply return it.
                    if(v_page_table -> table[page_no].present_absent == 1)
                    {

                        //don't forget to set referenced bit
                        v_page_table -> table[page_no].referenced = 1;
                        fclose(DISK);
                        return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                        pthread_mutex_unlock(&lock);
                        return return_value;
                    }
                    //if the specified page is not present in virtual page well, there we go...
                    else if(v_page_table -> table[page_no].present_absent == 0)
                    {

                        page_misses_bubble_get++;
                        //find a victim to swap out from page table
                        // and store it contents into disk file.

                        //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                            //First Category: Not referenced / Not modified
                            //Second Category: Not referenced / modified
                            //Third Category: Referenced / Not modified
                            //Fouth Category: Referenced / Modified

                            //Lower priority class will be cheaper to evict.

                            //First Category Search:
                            //printf("HEEEEEEEEEY\n" );
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {

                                i = get_LRU_page(v_page_table -> num_virtual,"global",-1);
                                //printf("%d. page is not present... LRU returns %d\n",page_no, i );

                            }
                            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {
                                int temp_1;
                                if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                                {

                                    i = get_LRU_page_first(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                                {
                                    i = get_LRU_page_second(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                                {
                                    i = get_LRU_page_third(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                                {
                                    i = get_LRU_page_fourth(v_page_table -> num_virtual);
                                }

                            }



                            //Fourth category search:


                                //printf("HEEEEEEEEEY2\n" );
                            /*if(found_first_class)
                            {
                                printf("First class found\n" );
                            }
                            else if(found_second_class)
                            {
                                printf("Second class found\n");
                            }
                            else if(found_third_class)
                            {
                                printf("Third class found\n");
                            }
                            else if(found_fourth_class)
                            {
                                printf("Fourth class found\n" );
                            }
                            printf("%d. page\n",i );*/


                            old_address = v_page_table -> table[i].page_frame_number; //save page no.

                            v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                            if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                            {
                                disk_page_writes_bubble_get++;
                                v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                                disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                                fseek(DISK, disk_offset, SEEK_SET);
                                for(j = 0 ; j < frame_size ; ++j)
                                {
                                    //store the contents of that page into disk.
                                    fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                                    disk_offset += _JUMP;
                                    fseek(DISK, disk_offset, SEEK_SET);
                                }
                            }
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                enQueue(page_no);
                            }


                            if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {
                                enQueue_1(page_no);
                            }
                            v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                            v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                            disk_offset = page_no * frame_size * _JUMP;
                            v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;
                            disk_page_reads_bubble_get++;
                            page_replacements_bubble_get++;
                            fseek(DISK, disk_offset, SEEK_SET);
                            for(i = 0 ; i < frame_size ; ++i)
                            {
                                fscanf(DISK, "%d\n", &number);
                                physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                                disk_offset += _JUMP;
                                fseek(DISK, disk_offset, SEEK_SET);
                            }
                            fclose(DISK);
                            return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                            pthread_mutex_unlock(&lock);
                            return return_value;




                    }
                fclose(DISK);

                return -1;
        }
        else if(strcmp(tName, "Quick") == 0)
        {
            reads_quick_get++;
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }

            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
            {
                set_i_nth_page(page_no, v_page_table -> num_virtual, "global", -1); // set referenced for LRU
            }
            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
            {
                int temp_1;
                if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                {

                    set_i_nth_page_first_quarter(page_no, v_page_table->num_virtual);
                }
                else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                {
                    temp_1 = page_no -v_page_table -> num_virtual / 4;
                    set_i_nth_page_second_quarter(temp_1, v_page_table->num_virtual);
                }
                else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                {
                    temp_1 = page_no - v_page_table -> num_virtual / 2;
                    set_i_nth_page_third_quarter(temp_1, v_page_table->num_virtual);
                }
                else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                {
                    temp_1 = page_no -v_page_table -> num_virtual * 3 / 4;
                    set_i_nth_page_fourth_quarter(temp_1, v_page_table->num_virtual);
                }

            }
            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_quick_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        i = get_LRU_page(v_page_table -> num_virtual,"global",-1);
                        //printf("%d. page is not present... LRU returns %d\n",page_no, i );
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        int temp_1;
                        if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                        {

                            i = get_LRU_page_first(v_page_table -> num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                        {
                            i = get_LRU_page_second(v_page_table -> num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                        {
                            i = get_LRU_page_third(v_page_table -> num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                        {
                            i = get_LRU_page_fourth(v_page_table -> num_virtual);
                        }
                    }





                    //Fourth category search:


                        //printf("HEEEEEEEEEY2\n" );
                    /*if(found_first_class)
                    {
                        printf("First class found\n" );
                    }
                    else if(found_second_class)
                    {
                        printf("Second class found\n");
                    }
                    else if(found_third_class)
                    {
                        printf("Third class found\n");
                    }
                    else if(found_fourth_class)
                    {
                        printf("Fourth class found\n" );
                    }
                    printf("%d. page\n",i );*/


                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_quick_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }
                    disk_page_reads_quick_get++;
                    page_replacements_quick_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_2(page_no);
                    }
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }
        else if(strcmp(tName, "Merge") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            reads_merge_get++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
            {
                set_i_nth_page(page_no, v_page_table -> num_virtual, "global", -1); // set referenced for LRU
            }
            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
            {
                int temp_1;
                if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                {

                    set_i_nth_page_first_quarter(page_no, v_page_table->num_virtual);
                }
                else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                {
                    temp_1 = page_no -v_page_table -> num_virtual / 4;
                    set_i_nth_page_second_quarter(temp_1, v_page_table->num_virtual);
                }
                else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                {
                    temp_1 = page_no - v_page_table -> num_virtual / 2;
                    set_i_nth_page_third_quarter(temp_1, v_page_table->num_virtual);
                }
                else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                {
                    temp_1 = page_no -v_page_table -> num_virtual * 3 / 4;
                    set_i_nth_page_fourth_quarter(temp_1, v_page_table->num_virtual);
                }
            }// set referenced for LRU

            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_merge_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        i = get_LRU_page(v_page_table -> num_virtual,"global",-1);
                        //printf("%d. page is not present... LRU returns %d\n",page_no, i );
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {

                        int temp_1;
                        if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                        {

                            i = get_LRU_page_first(v_page_table -> num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                        {
                            i = get_LRU_page_second(v_page_table -> num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                        {
                            i = get_LRU_page_third(v_page_table -> num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                        {
                            i = get_LRU_page_fourth(v_page_table -> num_virtual);
                        }

                    }






                    //Fourth category search:


                        //printf("HEEEEEEEEEY2\n" );
                    /*if(found_first_class)
                    {
                        printf("First class found\n" );
                    }
                    else if(found_second_class)
                    {
                        printf("Second class found\n");
                    }
                    else if(found_third_class)
                    {
                        printf("Third class found\n");
                    }
                    else if(found_fourth_class)
                    {
                        printf("Fourth class found\n" );
                    }
                    printf("%d. page\n",i );*/


                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_merge_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_merge_get++;
                    page_replacements_merge_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_3(page_no);
                    }

                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }

        else if(strcmp(tName, "Index") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            reads_index_get++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
            {
                set_i_nth_page(page_no, v_page_table -> num_virtual, "global", -1); // set referenced for LRU
            }
            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
            {
                int temp_1;
                if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                {

                    set_i_nth_page_first_quarter(page_no, v_page_table->num_virtual);
                }
                else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                {
                    temp_1 = page_no -v_page_table -> num_virtual / 4;
                    set_i_nth_page_second_quarter(temp_1, v_page_table->num_virtual);
                }
                else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                {
                    temp_1 = page_no - v_page_table -> num_virtual / 2;
                    set_i_nth_page_third_quarter(temp_1, v_page_table->num_virtual);
                }
                else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                {
                    temp_1 = page_no -v_page_table -> num_virtual * 3 / 4;
                    set_i_nth_page_fourth_quarter(temp_1, v_page_table->num_virtual);
                }
            }
            //if the specified page is present in virtual memory, no problem simply return it.
            if(v_page_table -> table[page_no].present_absent == 1)
            {

                //don't forget to set referenced bit
                v_page_table -> table[page_no].referenced = 1;
                fclose(DISK);
                return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                pthread_mutex_unlock(&lock);
                return return_value;
            }
            //if the specified page is not present in virtual page well, there we go...
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_index_get++;
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    //printf("HEEEEEEEEEY\n" );
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        i = get_LRU_page(v_page_table -> num_virtual,"global",-1);
                        //printf("%d. page is not present... LRU returns %d\n",page_no, i );
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        int temp_1;
                        if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                        {

                            i = get_LRU_page_first(v_page_table -> num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                        {
                            i = get_LRU_page_second(v_page_table -> num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                        {
                            i = get_LRU_page_third(v_page_table -> num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                        {
                            i = get_LRU_page_fourth(v_page_table -> num_virtual);
                        }
                    }





                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_index_get++;
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_index_get++;
                    page_replacements_index_get++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_4(page_no);
                    }
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }
                    fclose(DISK);
                    return_value = physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset];
                    pthread_mutex_unlock(&lock);
                    return return_value;




            }
        fclose(DISK);

        return -1;

        }
        //pthread_mutex_unlock(&lock);
        //exit(EXIT_FAILURE);
    }
    else if(strcmp(page_replacement, "WSClock") == 0)
    {
        fprintf(stderr, "%s\n", "Not implemented WSClock yet." );
        pthread_mutex_unlock(&lock);
        exit(EXIT_FAILURE);
    }
}




void set(unsigned int index, int value, char * tName)
{

    pthread_mutex_lock(&lock);

    int return_value;
    int disk_offset = 0;
    char page_replacement[10];
    char alloc_policy[10];
    char disk_name[30];
    int page_no;
    int offset;
    int i,j;
    int num_physical = v_page_table -> num_physical;
    int num_virtual = v_page_table -> num_virtual;
    int frame_size = v_page_table -> frame_size;
    int page_table_print_int = v_page_table -> page_table_print_int;
    strcpy(page_replacement, v_page_table -> page_replacement);
    strcpy(alloc_policy, v_page_table -> alloc_policy);
    strcpy(disk_name, v_page_table -> disk_name);

    int found_first_class = 0;
    int found_second_class = 0;
    int found_third_class = 0;
    int found_fourth_class = 0;
    int old_address;
    int number;

    if(strcmp(page_replacement, "NRU") == 0)
    {
        if(strcmp(tName, "Bubble") == 0)
        {

            if((reads_bubble_get + writes_bubble_set) % page_table_print_int == 0)
            {

                //print_page_table(v_page_table);

                for(i = 0 ; i < num_virtual  ; ++i)
                {
                    v_page_table->table[i].referenced = 0;
                }
            }
        }
        else if(strcmp(tName, "Quick") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4;
        }
        else if(strcmp(tName, "Merge") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 2;
        }

        else if(strcmp(tName, "Index") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4 * 3;
        }
        else
        {
            fprintf(stderr, "%s\n", "No such thread name. Something is wrong. Exiting the program");
            exit(EXIT_FAILURE);
        }

        if(strcmp(tName, "Bubble") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            writes_bubble_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_bubble_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        for(i = 0 ; i < num_virtual ; ++i)
                        {
                            if(v_page_table->table[i].present_absent == 1
                               && v_page_table->table[i].referenced == 0
                               && v_page_table->table[i].dirty_bit == 0)
                            {
                                   //found class 0;
                                   //printf("Set finds %d. page to evict from disk(1)\n", i);
                                   found_first_class = 1;
                                   break;
                            }

                        }

                        //Second category search:

                        if((found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)  ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                      //printf("Set finds %d. page to evict from disk(2)\n", i);
                                       //found class 1;
                                       found_second_class = 1;
                                       break;
                                }
                            }
                        }

                        //Third category search:
                        if((found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)   ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                    //printf("Set finds %d. page to evict from disk(3)\n", i);
                                       //found class 1;
                                       found_third_class = 1;
                                       break;
                                }
                            }
                        }

                        if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual) ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                      //printf("Set finds %d. page to evict from disk(4)\n", i);
                                       //found class 1;
                                       found_fourth_class = 1;
                                       break;
                                }
                            }
                        }
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {

                        if(strcmp(tName, "Bubble") == 0)
                        {

                            for(i = 0 ; i < num_virtual / 4 ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                       //found class 0;
                                       //printf("Three\n");
                                       found_first_class = 1;
                                       break;
                                }

                            }

                            //Second category search:

                            if((found_first_class == 0))
                            {
                                for(i = 0 ; i < num_virtual / 4 ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 0
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Little\n");
                                           //found class 1;
                                           found_second_class = 1;
                                           break;
                                    }
                                }
                            }

                            //Third category search:
                            if((found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = 0 ; i < num_virtual / 4 ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 0)
                                    {
                                        //printf("Birds\n");
                                           //found class 1;
                                           found_third_class = 1;
                                           break;
                                    }
                                }
                            }

                            if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = 0 ; i < num_virtual / 4 ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Lol\n");
                                           //found class 1;
                                           found_fourth_class = 1;
                                           break;
                                    }
                                }
                            }
                        }
                    }
                }




                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_bubble_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }


                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    disk_page_reads_bubble_set++;
                    page_replacements_bubble_set++;
                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
        }


        else if(strcmp(tName, "Quick") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            writes_quick_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_quick_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        for(i = 0 ; i < num_virtual ; ++i)
                        {
                            if(v_page_table->table[i].present_absent == 1
                               && v_page_table->table[i].referenced == 0
                               && v_page_table->table[i].dirty_bit == 0)
                            {
                                   //found class 0;
                                   //printf("Set finds %d. page to evict from disk(1)\n", i);
                                   found_first_class = 1;
                                   break;
                            }

                        }

                        //Second category search:

                        if((found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)  ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                      //printf("Set finds %d. page to evict from disk(2)\n", i);
                                       //found class 1;
                                       found_second_class = 1;
                                       break;
                                }
                            }
                        }

                        //Third category search:
                        if((found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)   ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                    //printf("Set finds %d. page to evict from disk(3)\n", i);
                                       //found class 1;
                                       found_third_class = 1;
                                       break;
                                }
                            }
                        }

                        if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual) ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                      //printf("Set finds %d. page to evict from disk(4)\n", i);
                                       //found class 1;
                                       found_fourth_class = 1;
                                       break;
                                }
                            }
                        }
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {

                        if(strcmp(tName, "Quick") == 0)
                        {
                            for(i = num_virtual / 4 ;(i < num_virtual / 2) ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                       //found class 0;
                                       //printf("Three\n");
                                       found_first_class = 1;
                                       break;
                                }

                            }

                            //Second category search:

                            if((found_first_class == 0))
                            {
                                for(i = num_virtual / 4 ;(i < num_virtual / 2) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 0
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Little\n");
                                           //found class 1;
                                           found_second_class = 1;
                                           break;
                                    }
                                }
                            }

                            //Third category search:
                            if((found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual / 4 ;(i < num_virtual / 2) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 0)
                                    {
                                        //printf("Birds\n");
                                           //found class 1;
                                           found_third_class = 1;
                                           break;
                                    }
                                }
                            }

                            if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual / 4 ;(i < num_virtual / 2) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Lol\n");
                                           //found class 1;
                                           found_fourth_class = 1;
                                           break;
                                    }
                                }
                            }
                        }
                    }
                }


                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_quick_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_quick_set++;
                    page_replacements_quick_set++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
            }

        else if(strcmp(tName, "Merge") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            writes_merge_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_merge_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        for(i = 0 ; i < num_virtual ; ++i)
                        {
                            if(v_page_table->table[i].present_absent == 1
                               && v_page_table->table[i].referenced == 0
                               && v_page_table->table[i].dirty_bit == 0)
                            {
                                   //found class 0;
                                   //printf("Set finds %d. page to evict from disk(1)\n", i);
                                   found_first_class = 1;
                                   break;
                            }

                        }

                        //Second category search:

                        if((found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)  ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                      //printf("Set finds %d. page to evict from disk(2)\n", i);
                                       //found class 1;
                                       found_second_class = 1;
                                       break;
                                }
                            }
                        }

                        //Third category search:
                        if((found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)   ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                    //printf("Set finds %d. page to evict from disk(3)\n", i);
                                       //found class 1;
                                       found_third_class = 1;
                                       break;
                                }
                            }
                        }

                        if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual) ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                      //printf("Set finds %d. page to evict from disk(4)\n", i);
                                       //found class 1;
                                       found_fourth_class = 1;
                                       break;
                                }
                            }
                        }
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        if(strcmp(tName, "Merge") == 0)
                        {
                            for(i = num_virtual / 2 ; (i < num_virtual * 3 / 4)   ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                       //found class 0;
                                       //printf("Three\n");
                                       found_first_class = 1;
                                       break;
                                }

                            }

                            //Second category search:

                            if((found_first_class == 0))
                            {
                                for(i = num_virtual / 2 ; (i < num_virtual * 3 / 4)   ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 0
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Little\n");
                                           //found class 1;
                                           found_second_class = 1;
                                           break;
                                    }
                                }
                            }

                            //Third category search:
                            if((found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual / 2 ; (i < num_virtual * 3 / 4)   ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 0)
                                    {
                                        //printf("Birds\n");
                                           //found class 1;
                                           found_third_class = 1;
                                           break;
                                    }
                                }
                            }

                            if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual / 2 ; (i < num_virtual * 3 / 4)   ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Lol\n");
                                           //found class 1;
                                           found_fourth_class = 1;
                                           break;
                                    }
                                }
                            }
                        }
                    }
                }




                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_merge_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_merge_set++;
                    page_replacements_merge_set++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
            }

        else if(strcmp(tName, "Index") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            reads_index_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_index_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        for(i = 0 ; i < num_virtual ; ++i)
                        {
                            if(v_page_table->table[i].present_absent == 1
                               && v_page_table->table[i].referenced == 0
                               && v_page_table->table[i].dirty_bit == 0)
                            {
                                   //found class 0;
                                   //printf("Set finds %d. page to evict from disk(1)\n", i);
                                   found_first_class = 1;
                                   break;
                            }

                        }

                        //Second category search:

                        if((found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)  ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                      //printf("Set finds %d. page to evict from disk(2)\n", i);
                                       //found class 1;
                                       found_second_class = 1;
                                       break;
                                }
                            }
                        }

                        //Third category search:
                        if((found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual)   ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                    //printf("Set finds %d. page to evict from disk(3)\n", i);
                                       //found class 1;
                                       found_third_class = 1;
                                       break;
                                }
                            }
                        }

                        if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                        {
                            for(i = 0 ; (i < num_virtual) ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 1
                                   && v_page_table->table[i].dirty_bit == 1)
                                {
                                      //printf("Set finds %d. page to evict from disk(4)\n", i);
                                       //found class 1;
                                       found_fourth_class = 1;
                                       break;
                                }
                            }
                        }
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        if(strcmp(tName, "Index") == 0)
                        {
                            for(i = num_virtual * 3 / 4 ; (i < num_virtual) ; ++i)
                            {
                                if(v_page_table->table[i].present_absent == 1
                                   && v_page_table->table[i].referenced == 0
                                   && v_page_table->table[i].dirty_bit == 0)
                                {
                                       //found class 0;
                                       //printf("Three\n");
                                       found_first_class = 1;
                                       break;
                                }

                            }

                            //Second category search:

                            if((found_first_class == 0))
                            {
                                for(i = num_virtual * 3 / 4 ; (i < num_virtual) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 0
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Little\n");
                                           //found class 1;
                                           found_second_class = 1;
                                           break;
                                    }
                                }
                            }

                            //Third category search:
                            if((found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual * 3 / 4 ; (i < num_virtual) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 0)
                                    {
                                        //printf("Birds\n");
                                           //found class 1;
                                           found_third_class = 1;
                                           break;
                                    }
                                }
                            }

                            if((found_third_class == 0)&& (found_second_class == 0) && (found_first_class == 0))
                            {
                                for(i = num_virtual * 3 / 4 ; (i < num_virtual) ; ++i)
                                {
                                    if(v_page_table->table[i].present_absent == 1
                                       && v_page_table->table[i].referenced == 1
                                       && v_page_table->table[i].dirty_bit == 1)
                                    {
                                        //printf("Lol\n");
                                           //found class 1;
                                           found_fourth_class = 1;
                                           break;
                                    }
                                }
                            }
                        }
                    }




                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_index_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_index_set++;
                    page_replacements_index_set++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
            }
        }
    }

    else if(strcmp(page_replacement, "FIFO") == 0)
    {

        if(strcmp(tName, "Bubble") == 0)
        {

            if((reads_bubble_get + writes_bubble_set) % page_table_print_int == 0)
            {

                //print_page_table(v_page_table);

                for(i = 0 ; i < num_virtual  ; ++i)
                {
                    v_page_table->table[i].referenced = 0;
                }
            }
        }
        else if(strcmp(tName, "Quick") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4;
        }
        else if(strcmp(tName, "Merge") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 2;
        }

        else if(strcmp(tName, "Index") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4 * 3;
        }
        else
        {
            fprintf(stderr, "%s\n", "No such thread name. Something is wrong. Exiting the program");
            exit(EXIT_FAILURE);
        }

        if(strcmp(tName, "Bubble") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            writes_bubble_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_bubble_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        i = deQueue();
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {

                        i = deQueue_1();
                    }
                }




                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_bubble_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }


                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_1(page_no);
                    }
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    disk_page_reads_bubble_set++;
                    page_replacements_bubble_set++;
                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
        }


        else if(strcmp(tName, "Quick") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            writes_quick_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_quick_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        i = deQueue();
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {

                        i = deQueue_2();
                    }
                }


                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_quick_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_quick_set++;
                    page_replacements_quick_set++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_2(page_no);
                    }

                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
            }

        else if(strcmp(tName, "Merge") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            writes_merge_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_merge_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        i = deQueue();
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        i = deQueue_3();
                    }
            }




                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_merge_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_merge_set++;
                    page_replacements_merge_set++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_3(page_no);
                    }
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
            }

        else if(strcmp(tName, "Index") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            reads_index_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_index_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        i = deQueue();
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        i = deQueue_3();
                    }




                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_index_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_index_set++;
                    page_replacements_index_set++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_4(page_no);
                    }

                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
            }
        }


        //fprintf(stderr, "%s\n", "Not implemented FIFO yet." );
        //pthread_mutex_unlock(&lock);
        //exit(EXIT_FAILURE);
    }
    else if(strcmp(page_replacement, "SC" ) == 0)
    {
        if(strcmp(tName, "Bubble") == 0)
        {

            if((reads_bubble_get + writes_bubble_set) % page_table_print_int == 0)
            {

                //print_page_table(v_page_table);

                for(i = 0 ; i < num_virtual ; ++i)
                {
                    v_page_table->table[i].referenced = 0;
                }
            }
        }
        else if(strcmp(tName, "Quick") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4;
        }
        else if(strcmp(tName, "Merge") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 2;
        }

        else if(strcmp(tName, "Index") == 0)
        {
            index += v_page_table->num_virtual * v_page_table-> frame_size / 4 * 3;
        }
        else
        {
            fprintf(stderr, "%s\n", "No such thread name. Something is wrong. Exiting the program");
            exit(EXIT_FAILURE);
        }

        if(strcmp(tName, "Bubble") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            writes_bubble_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_bubble_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }

                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {

                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue_1();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue_1(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }
                    }
                }




                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_bubble_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }


                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_1(page_no);
                    }

                    else if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    disk_page_reads_bubble_set++;
                    page_replacements_bubble_set++;
                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
        }


        else if(strcmp(tName, "Quick") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            writes_quick_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_quick_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {

                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue_2();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue_2(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }
                    }
                }


                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_quick_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_quick_set++;
                    page_replacements_quick_set++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_2(page_no);
                    }
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
            }

        else if(strcmp(tName, "Merge") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            writes_merge_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_merge_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue_3();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue_3(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }
                    }
            }




                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_merge_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_merge_set++;
                    page_replacements_merge_set++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_3(page_no);
                    }
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
            }

        else if(strcmp(tName, "Index") == 0)
        {
            FILE * DISK = fopen(disk_name, "r+");
            if(DISK == NULL)
            {
                fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                exit(EXIT_FAILURE);
            }
            reads_index_set++;
            page_no = (int) (index / frame_size); //find the page number.
            offset = index % frame_size; //find the offset on that page.

            //printf("Set needs to put %d. index to %d.value\n",index,value);
            //printf("The page no is %d and offset is for set %d\n",page_no, offset );

            if(v_page_table -> table[page_no].present_absent == 1)
            {
                //printf("Set realises %d. page is present on disk\n", page_no);
                //printf("%d.page is marked as referenced and dirty\n", page_no);
                //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                //don't forget to set referenced and dirty bit
                v_page_table -> table[page_no].referenced = 1;
                v_page_table -> table[page_no].dirty_bit = 1;
                fclose(DISK);
                physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                pthread_mutex_unlock(&lock);

                return;
            }
            else if(v_page_table -> table[page_no].present_absent == 0)
            {
                page_misses_index_set++;
                //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                //find a victim to swap out from page table
                // and store it contents into disk file.

                //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                    //First Category: Not referenced / Not modified
                    //Second Category: Not referenced / modified
                    //Third Category: Referenced / Not modified
                    //Fouth Category: Referenced / Modified

                    //Lower priority class will be cheaper to evict.

                    //First Category Search:
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {

                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        int candidate_page;
                        int found = 1;
                        int temp;
                        while(found)
                        {
                            candidate_page = deQueue_4();
                            if(v_page_table->table[candidate_page].referenced == 1)
                            {
                                // dont evict this page.
                                v_page_table->table[candidate_page].referenced = 0;
                                enQueue_4(candidate_page);
                            }
                            else
                            {
                                i = candidate_page;
                                found = 0;
                            }
                        }
                    }




                    old_address = v_page_table -> table[i].page_frame_number; //save page no.

                    v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                    if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                    {
                        disk_page_writes_index_set++;
                        //printf("copying%d\n", page_copying++);
                        v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                        disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                        fseek(DISK, disk_offset, SEEK_SET);
                        for(j = 0 ; j < frame_size ; ++j)
                        {
                            //store the contents of that page into disk.
                            fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                            disk_offset += _JUMP;
                            fseek(DISK, disk_offset, SEEK_SET);
                        }
                    }

                    disk_page_reads_index_set++;
                    page_replacements_index_set++;
                    v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                    v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                    v_page_table -> table[page_no]. dirty_bit = 1;
                    v_page_table -> table[page_no]. referenced = 1;
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        enQueue(page_no);
                    }

                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        enQueue_4(page_no);
                    }
                    disk_offset = page_no * frame_size * _JUMP;
                    v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                    fseek(DISK, disk_offset, SEEK_SET);
                    for(i = 0 ; i < frame_size ; ++i)
                    {
                        fscanf(DISK, "%d\n", &number);
                        physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                        disk_offset += _JUMP;
                        fseek(DISK, disk_offset, SEEK_SET);
                    }

                    fclose(DISK);
                    physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                    pthread_mutex_unlock(&lock);
            }
        }
    }
    else if(strcmp(page_replacement, "LRU") == 0)
    {

                if(strcmp(tName, "Bubble") == 0)
                {

                    if((reads_bubble_get + writes_bubble_set) % page_table_print_int == 0)
                    {

                        //print_page_table(v_page_table);

                        for(i = 0 ; i < num_virtual  ; ++i)
                        {
                            v_page_table->table[i].referenced = 0;
                        }
                    }
                }
                else if(strcmp(tName, "Quick") == 0)
                {
                    index += v_page_table->num_virtual * v_page_table-> frame_size / 4;
                }
                else if(strcmp(tName, "Merge") == 0)
                {
                    index += v_page_table->num_virtual * v_page_table-> frame_size / 2;
                }

                else if(strcmp(tName, "Index") == 0)
                {
                    index += v_page_table->num_virtual * v_page_table-> frame_size / 4 * 3;
                }
                else
                {
                    fprintf(stderr, "%s\n", "No such thread name. Something is wrong. Exiting the program");
                    exit(EXIT_FAILURE);
                }

                if(strcmp(tName, "Bubble") == 0)
                {
                    FILE * DISK = fopen(disk_name, "r+");
                    if(DISK == NULL)
                    {
                        fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                        exit(EXIT_FAILURE);
                    }
                    writes_bubble_set++;
                    page_no = (int) (index / frame_size); //find the page number.
                    offset = index % frame_size; //find the offset on that page.
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        set_i_nth_page(page_no, v_page_table -> num_virtual, "global", -1); // set referenced for LRU
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        int temp_1;
                        if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                        {

                            set_i_nth_page_first_quarter(page_no, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                        {
                            temp_1 = page_no -v_page_table -> num_virtual / 4;
                            set_i_nth_page_second_quarter(temp_1, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                        {
                            temp_1 = page_no - v_page_table -> num_virtual / 2;
                            set_i_nth_page_third_quarter(temp_1, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                        {
                            temp_1 = page_no -v_page_table -> num_virtual * 3 / 4;
                            set_i_nth_page_fourth_quarter(temp_1, v_page_table->num_virtual);
                        }
                    }
                    //printf("Set needs to put %d. index to %d.value\n",index,value);
                    //printf("The page no is %d and offset is for set %d\n",page_no, offset );

                    if(v_page_table -> table[page_no].present_absent == 1)
                    {
                        //printf("Set realises %d. page is present on disk\n", page_no);
                        //printf("%d.page is marked as referenced and dirty\n", page_no);
                        //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                        //don't forget to set referenced and dirty bit
                        v_page_table -> table[page_no].referenced = 1;
                        v_page_table -> table[page_no].dirty_bit = 1;
                        fclose(DISK);
                        physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                        pthread_mutex_unlock(&lock);

                        return;
                    }
                    else if(v_page_table -> table[page_no].present_absent == 0)
                    {
                        page_misses_bubble_set++;
                        //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                        //find a victim to swap out from page table
                        // and store it contents into disk file.

                        //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                            //First Category: Not referenced / Not modified
                            //Second Category: Not referenced / modified
                            //Third Category: Referenced / Not modified
                            //Fouth Category: Referenced / Modified

                            //Lower priority class will be cheaper to evict.

                            //First Category Search:
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                i = get_LRU_page(v_page_table -> num_virtual,"global",-1);
                                //printf("%d. page is not present... LRU returns %d\n",page_no, i );
                            }
                            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {

                                int temp_1;
                                if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                                {

                                    i = get_LRU_page_first(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                                {
                                    i = get_LRU_page_second(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                                {
                                    i = get_LRU_page_third(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                                {
                                    i = get_LRU_page_fourth(v_page_table -> num_virtual);
                                }
                            }
                        }




                            old_address = v_page_table -> table[i].page_frame_number; //save page no.

                            v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                            if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                            {
                                disk_page_writes_bubble_set++;
                                //printf("copying%d\n", page_copying++);
                                v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                                disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                                fseek(DISK, disk_offset, SEEK_SET);
                                for(j = 0 ; j < frame_size ; ++j)
                                {
                                    //store the contents of that page into disk.
                                    fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                                    disk_offset += _JUMP;
                                    fseek(DISK, disk_offset, SEEK_SET);
                                }
                            }


                            v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                            v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                            v_page_table -> table[page_no]. dirty_bit = 1;
                            v_page_table -> table[page_no]. referenced = 1;
                            if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {
                                enQueue_1(page_no);
                            }
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                enQueue(page_no);
                            }

                            disk_offset = page_no * frame_size * _JUMP;
                            v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                            disk_page_reads_bubble_set++;
                            page_replacements_bubble_set++;
                            fseek(DISK, disk_offset, SEEK_SET);
                            for(i = 0 ; i < frame_size ; ++i)
                            {
                                fscanf(DISK, "%d\n", &number);
                                physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                                disk_offset += _JUMP;
                                fseek(DISK, disk_offset, SEEK_SET);
                            }

                            fclose(DISK);
                            physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                            pthread_mutex_unlock(&lock);
                }


                else if(strcmp(tName, "Quick") == 0)
                {
                    FILE * DISK = fopen(disk_name, "r+");
                    if(DISK == NULL)
                    {
                        fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                        exit(EXIT_FAILURE);
                    }
                    writes_quick_set++;
                    page_no = (int) (index / frame_size); //find the page number.
                    offset = index % frame_size; //find the offset on that page.
                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        set_i_nth_page(page_no, v_page_table -> num_virtual, "global", -1); // set referenced for LRU
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        int temp_1;
                        if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                        {

                            set_i_nth_page_first_quarter(page_no, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                        {
                            temp_1 = page_no -v_page_table -> num_virtual / 4;
                            set_i_nth_page_second_quarter(temp_1, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                        {
                            temp_1 = page_no - v_page_table -> num_virtual / 2;
                            set_i_nth_page_third_quarter(temp_1, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                        {
                            temp_1 = page_no -v_page_table -> num_virtual * 3 / 4;
                            set_i_nth_page_fourth_quarter(temp_1, v_page_table->num_virtual);
                        }
                    }


                    //printf("Set needs to put %d. index to %d.value\n",index,value);
                    //printf("The page no is %d and offset is for set %d\n",page_no, offset );

                    if(v_page_table -> table[page_no].present_absent == 1)
                    {
                        //printf("Set realises %d. page is present on disk\n", page_no);
                        //printf("%d.page is marked as referenced and dirty\n", page_no);
                        //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                        //don't forget to set referenced and dirty bit
                        v_page_table -> table[page_no].referenced = 1;
                        v_page_table -> table[page_no].dirty_bit = 1;
                        fclose(DISK);
                        physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                        pthread_mutex_unlock(&lock);

                        return;
                    }
                    else if(v_page_table -> table[page_no].present_absent == 0)
                    {
                        page_misses_quick_set++;
                        //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                        //find a victim to swap out from page table
                        // and store it contents into disk file.

                        //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                            //First Category: Not referenced / Not modified
                            //Second Category: Not referenced / modified
                            //Third Category: Referenced / Not modified
                            //Fouth Category: Referenced / Modified

                            //Lower priority class will be cheaper to evict.

                            //First Category Search:
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                i = get_LRU_page(v_page_table -> num_virtual,"global",-1);
                                //printf("%d. page is not present... LRU returns %d\n",page_no, i );
                            }
                            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {

                                int temp_1;
                                if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                                {

                                    i = get_LRU_page_first(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                                {
                                    i = get_LRU_page_second(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                                {
                                    i = get_LRU_page_third(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                                {
                                    i = get_LRU_page_fourth(v_page_table -> num_virtual);
                                }
                            }
                        }


                            old_address = v_page_table -> table[i].page_frame_number; //save page no.

                            v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                            if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                            {
                                disk_page_writes_quick_set++;
                                //printf("copying%d\n", page_copying++);
                                v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                                disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                                fseek(DISK, disk_offset, SEEK_SET);
                                for(j = 0 ; j < frame_size ; ++j)
                                {
                                    //store the contents of that page into disk.
                                    fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                                    disk_offset += _JUMP;
                                    fseek(DISK, disk_offset, SEEK_SET);
                                }
                            }

                            disk_page_reads_quick_set++;
                            page_replacements_quick_set++;
                            v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                            v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                            v_page_table -> table[page_no]. dirty_bit = 1;
                            v_page_table -> table[page_no]. referenced = 1;
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                enQueue(page_no);
                            }
                            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {
                                enQueue_2(page_no);
                            }

                            disk_offset = page_no * frame_size * _JUMP;
                            v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                            fseek(DISK, disk_offset, SEEK_SET);
                            for(i = 0 ; i < frame_size ; ++i)
                            {
                                fscanf(DISK, "%d\n", &number);
                                physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                                disk_offset += _JUMP;
                                fseek(DISK, disk_offset, SEEK_SET);
                            }

                            fclose(DISK);
                            physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                            pthread_mutex_unlock(&lock);
                    }

                else if(strcmp(tName, "Merge") == 0)
                {
                    FILE * DISK = fopen(disk_name, "r+");
                    if(DISK == NULL)
                    {
                        fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                        exit(EXIT_FAILURE);
                    }
                    writes_merge_set++;
                    page_no = (int) (index / frame_size); //find the page number.
                    offset = index % frame_size; //find the offset on that page.

                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        set_i_nth_page(page_no, v_page_table -> num_virtual, "global", -1); // set referenced for LRU
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        int temp_1;
                        if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                        {

                            set_i_nth_page_first_quarter(page_no, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                        {
                            temp_1 = page_no -v_page_table -> num_virtual / 4;
                            set_i_nth_page_second_quarter(temp_1, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                        {
                            temp_1 = page_no - v_page_table -> num_virtual / 2;
                            set_i_nth_page_third_quarter(temp_1, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                        {
                            temp_1 = page_no -v_page_table -> num_virtual * 3 / 4;
                            set_i_nth_page_fourth_quarter(temp_1, v_page_table->num_virtual);
                        }
                    }
                    //printf("Set needs to put %d. index to %d.value\n",index,value);
                    //printf("The page no is %d and offset is for set %d\n",page_no, offset );

                    if(v_page_table -> table[page_no].present_absent == 1)
                    {
                        //printf("Set realises %d. page is present on disk\n", page_no);
                        //printf("%d.page is marked as referenced and dirty\n", page_no);
                        //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                        //don't forget to set referenced and dirty bit
                        v_page_table -> table[page_no].referenced = 1;
                        v_page_table -> table[page_no].dirty_bit = 1;
                        fclose(DISK);
                        physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                        pthread_mutex_unlock(&lock);

                        return;
                    }
                    else if(v_page_table -> table[page_no].present_absent == 0)
                    {
                        page_misses_merge_set++;
                        //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                        //find a victim to swap out from page table
                        // and store it contents into disk file.

                        //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                            //First Category: Not referenced / Not modified
                            //Second Category: Not referenced / modified
                            //Third Category: Referenced / Not modified
                            //Fouth Category: Referenced / Modified

                            //Lower priority class will be cheaper to evict.

                            //First Category Search:
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                i = get_LRU_page(v_page_table -> num_virtual,"global",-1);
                                //printf("%d. page is not present... LRU returns %d\n",page_no, i );
                            }
                            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {
                                int temp_1;
                                if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                                {

                                    i = get_LRU_page_first(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                                {
                                    i = get_LRU_page_second(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                                {
                                    i = get_LRU_page_third(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                                {
                                    i = get_LRU_page_fourth(v_page_table -> num_virtual);
                                }

                            }
                    }




                            old_address = v_page_table -> table[i].page_frame_number; //save page no.

                            v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                            if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                            {
                                disk_page_writes_merge_set++;
                                //printf("copying%d\n", page_copying++);
                                v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                                disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                                fseek(DISK, disk_offset, SEEK_SET);
                                for(j = 0 ; j < frame_size ; ++j)
                                {
                                    //store the contents of that page into disk.
                                    fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                                    disk_offset += _JUMP;
                                    fseek(DISK, disk_offset, SEEK_SET);
                                }
                            }

                            disk_page_reads_merge_set++;
                            page_replacements_merge_set++;
                            v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                            v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                            v_page_table -> table[page_no]. dirty_bit = 1;
                            v_page_table -> table[page_no]. referenced = 1;
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                enQueue(page_no);
                            }

                            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {
                                enQueue_3(page_no);
                            }
                            disk_offset = page_no * frame_size * _JUMP;
                            v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                            fseek(DISK, disk_offset, SEEK_SET);
                            for(i = 0 ; i < frame_size ; ++i)
                            {
                                fscanf(DISK, "%d\n", &number);
                                physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                                disk_offset += _JUMP;
                                fseek(DISK, disk_offset, SEEK_SET);
                            }

                            fclose(DISK);
                            physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                            pthread_mutex_unlock(&lock);
                    }

                else if(strcmp(tName, "Index") == 0)
                {
                    FILE * DISK = fopen(disk_name, "r+");
                    if(DISK == NULL)
                    {
                        fprintf(stderr, "%s\n","Error opening file. Exiting program." );
                        exit(EXIT_FAILURE);
                    }
                    reads_index_set++;
                    page_no = (int) (index / frame_size); //find the page number.
                    offset = index % frame_size; //find the offset on that page.

                    if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                    {
                        set_i_nth_page(page_no, v_page_table -> num_virtual, "global", -1); // set referenced for LRU
                    }
                    else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                    {
                        int temp_1;
                        if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                        {

                            set_i_nth_page_first_quarter(page_no, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                        {
                            temp_1 = page_no -v_page_table -> num_virtual / 4;
                            set_i_nth_page_second_quarter(temp_1, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                        {
                            temp_1 = page_no - v_page_table -> num_virtual / 2;
                            set_i_nth_page_third_quarter(temp_1, v_page_table->num_virtual);
                        }
                        else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                        {
                            temp_1 = page_no -v_page_table -> num_virtual * 3 / 4;
                            set_i_nth_page_fourth_quarter(temp_1, v_page_table->num_virtual);
                        }
                    }
                    //printf("Set needs to put %d. index to %d.value\n",index,value);
                    //printf("The page no is %d and offset is for set %d\n",page_no, offset );

                    if(v_page_table -> table[page_no].present_absent == 1)
                    {
                        //printf("Set realises %d. page is present on disk\n", page_no);
                        //printf("%d.page is marked as referenced and dirty\n", page_no);
                        //printf("%d. index in physical memory will be set \n",v_page_table -> table[page_no].page_frame_number * frame_size + offset);
                        //don't forget to set referenced and dirty bit
                        v_page_table -> table[page_no].referenced = 1;
                        v_page_table -> table[page_no].dirty_bit = 1;
                        fclose(DISK);
                        physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                        pthread_mutex_unlock(&lock);

                        return;
                    }
                    else if(v_page_table -> table[page_no].present_absent == 0)
                    {
                        page_misses_index_set++;
                        //printf("Set realises %d. page is not present int physical memory.\n", page_no);
                        //find a victim to swap out from page table
                        // and store it contents into disk file.

                        //IN NRU Page Replacement Algorithm we divide pages as 4 categories:
                            //First Category: Not referenced / Not modified
                            //Second Category: Not referenced / modified
                            //Third Category: Referenced / Not modified
                            //Fouth Category: Referenced / Modified

                            //Lower priority class will be cheaper to evict.

                            //First Category Search:
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                i = get_LRU_page(v_page_table -> num_virtual,"global",-1);
                                //printf("%d. page is not present... LRU returns %d\n",page_no, i );
                            }
                            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {
                                int temp_1;
                                if(page_no >= 0 && page_no <  v_page_table -> num_virtual / 4)
                                {

                                    i = get_LRU_page_first(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual / 4 && page_no < v_page_table -> num_virtual / 2 )
                                {
                                    i = get_LRU_page_second(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual / 2 && page_no < v_page_table -> num_virtual * 3 / 4 )
                                {
                                    i = get_LRU_page_third(v_page_table -> num_virtual);
                                }
                                else if(page_no >= v_page_table -> num_virtual * 3 / 4 && page_no < v_page_table -> num_virtual)
                                {
                                    i = get_LRU_page_fourth(v_page_table -> num_virtual);
                                }

                            }




                            old_address = v_page_table -> table[i].page_frame_number; //save page no.

                            v_page_table -> table[i].present_absent = 0; // This victim page will be in disk afterwards.
                            if(v_page_table -> table[i].dirty_bit == 1) //If the page is dirty we need to write back into disk.
                            {
                                disk_page_writes_index_set++;
                                //printf("copying%d\n", page_copying++);
                                v_page_table -> table[i].dirty_bit = 0; //Victim page is not dirty anymore.
                                disk_offset = i * frame_size * _JUMP; //move offset to victim page.
                                fseek(DISK, disk_offset, SEEK_SET);
                                for(j = 0 ; j < frame_size ; ++j)
                                {
                                    //store the contents of that page into disk.
                                    fprintf(DISK, "%d\n",physical_mem[(v_page_table -> table[i].page_frame_number * frame_size) + j]);
                                    disk_offset += _JUMP;
                                    fseek(DISK, disk_offset, SEEK_SET);
                                }
                            }

                            disk_page_reads_index_set++;
                            page_replacements_index_set++;
                            v_page_table -> table[page_no].present_absent = 1; //new page will be in physical memory
                            v_page_table ->table[page_no].page_frame_number = old_address;//restore page no.
                            v_page_table -> table[page_no]. dirty_bit = 1;
                            v_page_table -> table[page_no]. referenced = 1;
                            if(strcmp(v_page_table -> alloc_policy, "global") == 0)
                            {
                                enQueue(page_no);
                            }
                            else if(strcmp(v_page_table -> alloc_policy, "local") == 0)
                            {
                                enQueue_4(page_no);
                            }

                            disk_offset = page_no * frame_size * _JUMP;
                            v_page_table -> table[i].page_frame_number = NOTPRESENTINMEMORY;

                            fseek(DISK, disk_offset, SEEK_SET);
                            for(i = 0 ; i < frame_size ; ++i)
                            {
                                fscanf(DISK, "%d\n", &number);
                                physical_mem[v_page_table -> table[page_no]. page_frame_number * frame_size + i] = number;
                                disk_offset += _JUMP;
                                fseek(DISK, disk_offset, SEEK_SET);
                            }

                            fclose(DISK);
                            physical_mem[v_page_table -> table[page_no].page_frame_number * frame_size + offset] = value;
                            pthread_mutex_unlock(&lock);
                    }
                }


                //fprintf(stderr, "%s\n", "Not implemented FIFO yet." );
                //pthread_mutex_unlock(&lock);
                //exit(EXIT_FAILURE);
    }
    else if(strcmp(page_replacement, "WSClock") == 0)
    {
        fprintf(stderr, "%s\n", "Not implemented WSClock yet." );
        pthread_mutex_unlock(&lock);
        exit(EXIT_FAILURE);
    }


}






void *thread_1()
{
    int i;
    int temp;
    int j;

    for(i = 0 ; i < (v_page_table->num_virtual * v_page_table-> frame_size / 4) - 1 ; ++i)
    {
        for(j = 0 ; j < (v_page_table->num_virtual * v_page_table-> frame_size / 4) - i - 1 ; j ++)
        {
            if(get(j,"Bubble") > get(j + 1, "Bubble"))
            {
                temp = get(j, "Bubble");
                set(j, get(j+1, "Bubble"), "Bubble");
                set(j + 1, temp, "Bubble");

            }
            //print_page_table_NRU(v_page_table);

        }

    }

}

void *thread_2()
{
    int size;
    size = (v_page_table->num_virtual * v_page_table-> frame_size / 4);

    quick_sort(0, size - 1);
}

void *thread_3()
{
    int size;
    size = (v_page_table->num_virtual * v_page_table-> frame_size / 4);
    sort(0, size - 1);

}

void *thread_4()
{
    int b[(v_page_table->num_virtual * v_page_table-> frame_size / 4)];
    indices = index_sort(b, (v_page_table->num_virtual * v_page_table-> frame_size / 4));

}


/*https://www.programiz.com/dsa/queue*/


void enQueue_1(int value) {
  if (rear_1 == SIZE - 1)
  {
      //printf("\nQueue is Full!!");
  }

  else {
    if (front_1 == -1)
      front_1 = 0;
    rear_1++;
    items_1[rear_1] = value;

  }
}
int deQueue_1() {
    int ret;
  if (front_1 == -1)
  {

      return -1;
  }

  else {
    ret = items_1[front_1];
    front_1++;
    if (front_1 > rear_1)
      front_1 = rear_1 = -1;
  }
  return ret;
}



void enQueue(int value) {
  if (rear == SIZE - 1)
  {
    // printf("\nQueue is Full!!");
  }

  else {
    if (front == -1)
      front = 0;
    rear++;
    items[rear] = value;

  }
}

int deQueue() {
    int ret;
  if (front == -1)
  {
      printf("\nQueue is Empty!!");
      return -1;
  }

  else {

    ret = items[front];
    front++;
    if (front > rear)
      front = rear = -1;
  }
  return ret;
}

void enQueue_2(int value) {
  if (rear_2 == SIZE - 1)
    printf("\nQueue is Full!!");
  else {
    if (front_2 == -1)
      front_2 = 0;
    rear_2++;
    items_2[rear_2] = value;

  }
}

int deQueue_2() {
    int ret;
  if (front_2 == -1)
  {

      return -1;
  }

  else {

    ret = items_2[front_2];
    front_2++;
    if (front_2 > rear_2)
      front_2 = rear_2 = -1;
  }
  return ret;
}



void enQueue_3(int value) {
  if (rear_3 == SIZE - 1)
    printf("\nQueue is Full!!");
  else {
    if (front_3 == -1)
      front_3 = 0;
    rear_3++;
    items_3[rear_3] = value;

  }
}

int deQueue_3() {
    int ret;
  if (front_3 == -1)
  {

      return -1;
  }

  else {

    ret = items_3[front_3];
    front_3++;
    if (front_3 > rear_3)
      front_3 = rear_3 = -1;
  }
  return ret;
}


void enQueue_4(int value) {
  if (rear_4 == SIZE - 1)
    printf("\nQueue is Full!!");
  else {
    if (front_4 == -1)
      front_4 = 0;
    rear_4++;
    items_4[rear_4] = value;

  }
}

int deQueue_4() {
    int ret;
  if (front_4 == -1)
  {

      return -1;
  }

  else {

    ret = items_4[front_4];
    front_4++;
    if (front_4 > rear_4)
      front_4 = rear_4 = -1;
  }
  return ret;
}


// Function to print the queue
void display() {
  if (rear == -1)
    printf("\nQueue is Empty!!!");
  else {
    int i;

    for (i = front; i <= rear; i++)
      printf("%d  ", items[i]);
  }
  printf("\n");
}



void allocate_tanenbaum_LRU_matrix(int page_no)
{
    int i;
    tanenbaum_LRU_matrix = (int**) calloc(page_no, sizeof(int*));
    for(i = 0 ; i < page_no ; ++i)
    {
        tanenbaum_LRU_matrix[i] = (int *) calloc(page_no , sizeof(int));
    }
}


void allocate_tanenbaum_LRU_matrix_first_quarter(int page_no)
{
    int i;
    tanenbaum_LRU_matrix_first_quarter = (int**) calloc(page_no / 4, sizeof(int*));
    for(i = 0 ; i < page_no / 4 ; ++i)
    {
        tanenbaum_LRU_matrix_first_quarter[i] = (int *) calloc(page_no / 4 , sizeof(int));
    }
}

void allocate_tanenbaum_LRU_matrix_second_quarter(int page_no)
{
    int i;
    tanenbaum_LRU_matrix_second_quarter = (int**) calloc(page_no / 4, sizeof(int*));
    for(i = 0 ; i < page_no / 4 ; ++i)
    {
        tanenbaum_LRU_matrix_second_quarter[i] = (int *) calloc(page_no / 4 , sizeof(int));
    }
}

void allocate_tanenbaum_LRU_matrix_third_quarter(int page_no)
{
    int i;
    tanenbaum_LRU_matrix_third_quarter = (int**) calloc(page_no / 4, sizeof(int*));
    for(i = 0 ; i < page_no / 4 ; ++i)
    {
        tanenbaum_LRU_matrix_third_quarter[i] = (int *) calloc(page_no / 4 , sizeof(int));
    }
}

void allocate_tanenbaum_LRU_matrix_fourth_quarter(int page_no)
{
    int i;
    tanenbaum_LRU_matrix_fourth_quarter = (int**) calloc(page_no / 4, sizeof(int*));
    for(i = 0 ; i < page_no / 4 ; ++i)
    {
        tanenbaum_LRU_matrix_fourth_quarter[i] = (int *) calloc(page_no / 4 , sizeof(int));
    }
}


void free_tanenbaum_LRU_matrix_first_quarter(int page_no)
{
    int i;
    for(i = 0 ; i < page_no / 4 ; ++i)
    {
        free(tanenbaum_LRU_matrix_first_quarter[i]);
    }
    free(tanenbaum_LRU_matrix_first_quarter);
}

void free_tanenbaum_LRU_matrix_second_quarter(int page_no)
{
    int i;
    for(i = 0 ; i < page_no / 4 ; ++i)
    {
        free(tanenbaum_LRU_matrix_second_quarter[i]);
    }
    free(tanenbaum_LRU_matrix_second_quarter);
}

void free_tanenbaum_LRU_matrix_third_quarter(int page_no)
{
    int i;
    for(i = 0 ; i < page_no / 4 ; ++i)
    {
        free(tanenbaum_LRU_matrix_third_quarter[i]);
    }
    free(tanenbaum_LRU_matrix_third_quarter);
}

void free_tanenbaum_LRU_matrix_fourth_quarter(int page_no)
{
    int i;
    for(i = 0 ; i < page_no / 4 ; ++i)
    {
        free(tanenbaum_LRU_matrix_fourth_quarter[i]);
    }
    free(tanenbaum_LRU_matrix_fourth_quarter);
}




void free_tanenbaum_LRU_matrix(int page_no)
{
    int i;
    for(i = 0 ; i < page_no ; ++i)
    {
        free(tanenbaum_LRU_matrix[i]);
    }
    free(tanenbaum_LRU_matrix);
}



void set_i_nth_page(int current_page, int page_no, char *policy, int which_quarter)
{
    int i;
    if(strcmp(policy, "global") == 0)
    {
        for(i = 0 ; i < page_no ; ++i)
        {
            tanenbaum_LRU_matrix[current_page][i] = 1;
        }

        for(i = 0 ; i < page_no ; ++i)
        {
            tanenbaum_LRU_matrix[i][current_page] = 0;
        }
    }
    else if(strcmp(policy, "local") == 0)
    {
        if(which_quarter == 1)
        {
            for(i = 0 ; i < page_no / 4 ; ++i)
            {
                tanenbaum_LRU_matrix[current_page][i] = 1;
            }

            for(i = 0 ; i < page_no / 4 ; ++i)
            {
                tanenbaum_LRU_matrix[i][current_page] = 0;
            }
        }
        else if(which_quarter == 2)
        {
            for(i = page_no / 4 ; i < page_no / 2 ; ++i)
            {
                tanenbaum_LRU_matrix[current_page][i] = 1;
            }

                for(i = page_no / 4 ; i < page_no / 2 ; ++i)
            {
                tanenbaum_LRU_matrix[i][current_page] = 0;
            }
        }
        else if(which_quarter == 3)
        {
            for(i = page_no / 2 ; i < page_no * 3 / 4 ; ++i)
            {
                tanenbaum_LRU_matrix[current_page][i] = 1;
            }

            for(i = page_no / 2 ; i < page_no * 3 / 4 ; ++i)
            {
                tanenbaum_LRU_matrix[i][current_page] = 0;
            }
        }
        else if(which_quarter == 4)
        {
            for(i = page_no * 3 / 4; i < page_no ; ++i)
            {
                tanenbaum_LRU_matrix[current_page][i] = 1;
            }

            for(i = page_no * 3 / 4 ; i < page_no ; ++i)
            {
                tanenbaum_LRU_matrix[i][current_page] = 0;
            }
        }
    }

}

void set_i_nth_page_first_quarter(int current_page, int page_no)
{
    int i;

        for(i = 0 ; i < page_no / 4 ; ++i)
        {
            tanenbaum_LRU_matrix_first_quarter[current_page][i] = 1;
        }

        for(i = 0 ; i < page_no / 4 ; ++i)
        {
            tanenbaum_LRU_matrix_first_quarter[i][current_page] = 0;
        }


}


void set_i_nth_page_second_quarter(int current_page, int page_no)
{
    int i;

        for(i = 0 ; i < page_no / 4 ; ++i)
        {
            tanenbaum_LRU_matrix_second_quarter[current_page][i] = 1;
        }

        for(i = 0 ; i < page_no / 4 ; ++i)
        {
            tanenbaum_LRU_matrix_second_quarter[i][current_page] = 0;
        }


}

void set_i_nth_page_third_quarter(int current_page, int page_no)
{
    int i;

        for(i = 0 ; i < page_no / 4 ; ++i)
        {
            tanenbaum_LRU_matrix_third_quarter[current_page][i] = 1;
        }

        for(i = 0 ; i < page_no / 4 ; ++i)
        {
            tanenbaum_LRU_matrix_third_quarter[i][current_page] = 0;
        }


}

void set_i_nth_page_fourth_quarter(int current_page, int page_no)
{
    int i;

        for(i = 0 ; i < page_no / 4 ; ++i)
        {
            tanenbaum_LRU_matrix_fourth_quarter[current_page][i] = 1;
        }

        for(i = 0 ; i < page_no / 4 ; ++i)
        {
            tanenbaum_LRU_matrix_fourth_quarter[i][current_page] = 0;
        }


}

int get_LRU_page(int page_no, char *alloc_policy, int which_quarter)
{


    int j;
    int found = -1;
    int min = 9999999;
    int i;
    int fewest_ones =9999999;
    int k = 0;
    int return_page = 0;
    if(strcmp(alloc_policy, "global") == 0)
    {
        for(i = 0 ; i < v_page_table -> num_virtual ; ++i)
        {
            if(v_page_table -> table[i]. present_absent == 1)
            {
                present_pages[k++] = i;
            }

        }





        for(i = 0 ; i < v_page_table -> num_physical ; ++i)
        {
            for(j = 0 ; j < v_page_table -> num_virtual ; ++j)
            {
                if(tanenbaum_LRU_matrix[present_pages[i]][j] == 1 )
                {
                    fewest_ones++;
                }
            }
            if(fewest_ones < min)
            {
                min = fewest_ones;
                found = i;
            }
            fewest_ones = 0;

        }

        return_page = present_pages[found];


        return return_page;
    }
    else if(strcmp(alloc_policy, "local") == 0)
    {

        if(which_quarter == 1)
        {
            for(i = 0 ; i < v_page_table -> num_virtual / 4 ; ++i)
            {
                if(v_page_table -> table[i]. present_absent == 1)
                {

                    present_pages[k++] = i;

                }

            }




            for(i = 0 ; i < v_page_table -> num_physical / 4 ; ++i)
            {

            }

            for(i = 0 ; i < v_page_table -> num_physical / 4 ; ++i)
            {
                for(j = 0 ; j < v_page_table -> num_virtual / 4 ; ++j)
                {
                    if(tanenbaum_LRU_matrix[present_pages[i]][j] == 1 )
                    {
                        fewest_ones++;
                    }
                }
                if(fewest_ones < min)
                {
                    min = fewest_ones;
                    found = i;
                }
                fewest_ones = 0;

            }

            return_page = present_pages[found];

            printf("1. quarter returns %d. page\n",return_page );
            return return_page;



        }
        else if(which_quarter == 2)
        {
            k = v_page_table -> num_virtual / 4;
            for(i = v_page_table -> num_virtual / 4 ; i < v_page_table -> num_virtual / 2 ; ++i)
            {
                if(v_page_table -> table[i]. present_absent == 1)
                {
                    present_pages[k++] = i;
                }

            }


            for(i = v_page_table -> num_physical / 4; i < v_page_table -> num_physical / 2 ; ++i)
            {
                for(j =  v_page_table -> num_virtual / 4 ; j < v_page_table -> num_virtual / 2 ; ++j)
                {
                    if(tanenbaum_LRU_matrix[present_pages[i]][j] == 1 )
                    {
                        fewest_ones++;
                    }
                }
                if(fewest_ones < min)
                {
                    min = fewest_ones;
                    found = i;
                }
                fewest_ones = 0;

            }

            return_page = present_pages[found];


            return return_page;




        }
        else if(which_quarter == 3)
        {
            k = v_page_table -> num_virtual  * 3 / 4;
            for(i = v_page_table -> num_virtual / 2 ; i < v_page_table -> num_virtual * 3 / 4 ; ++i)
            {
                if(v_page_table -> table[i]. present_absent == 1)
                {
                    present_pages[k++] = i;
                }

            }


            for(i = v_page_table -> num_physical / 2; i < v_page_table -> num_physical * 3 / 4 ; ++i)
            {
                for(j =  v_page_table -> num_virtual / 2 ; j < v_page_table -> num_virtual  * 3 / 4  ; ++j)
                {
                    if(tanenbaum_LRU_matrix[present_pages[i]][j] == 1 )
                    {
                        fewest_ones++;
                    }
                }
                if(fewest_ones < min)
                {
                    min = fewest_ones;
                    found = i;
                }
                fewest_ones = 0;

            }

            return_page = present_pages[found];

            return return_page;




        }
        else if(which_quarter == 4)
        {
            for(i =v_page_table -> num_virtual * 3 / 4  ; i < v_page_table -> num_virtual ; ++i)
            {
                if(v_page_table -> table[i]. present_absent == 1)
                {
                    present_pages[k++] = i;
                }

            }

            for(i = v_page_table -> num_physical * 3 / 4 ; i < v_page_table -> num_physical  ; ++i)
            {

            }

            for(i =  v_page_table -> num_physical * 3 / 4 ; i < v_page_table -> num_physical ; ++i)
            {
                for(j = v_page_table -> num_virtual * 3 / 4 ; j < v_page_table -> num_virtual ; ++j)
                {
                    if(tanenbaum_LRU_matrix[present_pages[i]][j] == 1 )
                    {
                        fewest_ones++;
                    }
                }
                if(fewest_ones < min)
                {
                    min = fewest_ones;
                    found = i;
                }
                fewest_ones = 0;

            }

            return_page = present_pages[found];


            return return_page;
        }
        else
        {
            fprintf(stderr, "%s\n", "Bad parameter");
            exit(EXIT_FAILURE);
        }
    }



}




int get_LRU_page_first(int page_no)
{


    int j;
    int found = -1;
    int min = 9999999;
    int i;
    int fewest_ones =9999999;
    int k = 0;
    int return_page = 0;


        for(i = 0 ; i < v_page_table -> num_virtual / 4; ++i)
        {
            if(v_page_table -> table[i]. present_absent == 1)
            {
                present_pages_1[k++] = i;
            }

        }
        for(i = 0 ; i < v_page_table -> num_physical / 4 ; ++i)
        {
            printf("%d = %d\n",i, present_pages_1[i]);
        }

        for(i = 0 ; i < v_page_table -> num_physical / 4 ; ++i)
        {
            for(j = 0 ; j < v_page_table -> num_virtual / 4 ; ++j)
            {
                if(tanenbaum_LRU_matrix_first_quarter[present_pages_1[i]][j] == 1 )
                {
                    fewest_ones++;
                }
            }
            if(fewest_ones < min)
            {
                min = fewest_ones;
                found = i;
            }
            fewest_ones = 0;

        }

        return_page = present_pages_1[found];


        return return_page;

}

int get_LRU_page_second(int page_no)
{


    int j;
    int found = -1;
    int min = 9999999;
    int i;
    int fewest_ones =9999999;
    int k = 0;
    int return_page = 0;


        for(i = 0 ; i < v_page_table -> num_virtual / 4; ++i)
        {
            if(v_page_table -> table[i]. present_absent == 1)
            {
                present_pages_2[k++] = i;
            }

        }


        for(i = 0 ; i < v_page_table -> num_physical / 4 ; ++i)
        {

        }



        for(i = 0 ; i < v_page_table -> num_physical / 4 ; ++i)
        {
            for(j = 0 ; j < v_page_table -> num_virtual / 4 ; ++j)
            {
                if(tanenbaum_LRU_matrix_second_quarter[present_pages_2[i]][j] == 1 )
                {
                    fewest_ones++;
                }
            }
            if(fewest_ones < min)
            {
                min = fewest_ones;
                found = i;
            }
            fewest_ones = 0;

        }

        return_page = present_pages_2[found];


        return return_page;

}


int get_LRU_page_third(int page_no)
{


    int j;
    int found = -1;
    int min = 9999999;
    int i;
    int fewest_ones =9999999;
    int k = 0;
    int return_page = 0;


        for(i = 0 ; i < v_page_table -> num_virtual / 4; ++i)
        {
            if(v_page_table -> table[i]. present_absent == 1)
            {
                present_pages_3[k++] = i;
            }

        }


        for(i = 0 ; i < v_page_table -> num_physical / 4 ; ++i)
        {

        }



        for(i = 0 ; i < v_page_table -> num_physical / 4 ; ++i)
        {
            for(j = 0 ; j < v_page_table -> num_virtual / 4 ; ++j)
            {
                if(tanenbaum_LRU_matrix_third_quarter[present_pages_3[i]][j] == 1 )
                {
                    fewest_ones++;
                }
            }
            if(fewest_ones < min)
            {
                min = fewest_ones;
                found = i;
            }
            fewest_ones = 0;

        }

        return_page = present_pages_3[found];


        return return_page;

}


int get_LRU_page_fourth(int page_no)
{


    int j;
    int found = -1;
    int min = 9999999;
    int i;
    int fewest_ones =9999999;
    int k = 0;
    int return_page = 0;


        for(i = 0 ; i < v_page_table -> num_virtual / 4; ++i)
        {
            if(v_page_table -> table[i]. present_absent == 1)
            {
                present_pages_4[k++] = i;
            }

        }


        for(i = 0 ; i < v_page_table -> num_physical / 4 ; ++i)
        {

        }



        for(i = 0 ; i < v_page_table -> num_physical / 4 ; ++i)
        {
            for(j = 0 ; j < v_page_table -> num_virtual / 4 ; ++j)
            {
                if(tanenbaum_LRU_matrix_fourth_quarter[present_pages_4[i]][j] == 1 )
                {
                    fewest_ones++;
                }
            }
            if(fewest_ones < min)
            {
                min = fewest_ones;
                found = i;
            }
            fewest_ones = 0;

        }

        return_page = present_pages_4[found];


        return return_page;

}



void discard_other(int current_page_no, int page_no)
{
    int i;
    for(i = 0 ; i < page_no ; ++i)
    {
        tanenbaum_LRU_matrix[current_page_no][i] = 1;
    }
}


void check()
{
    //PRINT SORTED ARRAY.
    int i;
    int k = 0;
    for(i = 0 ; i < v_page_table->num_virtual * v_page_table-> frame_size ; ++i)
    {

        if(i >= 0 && i < v_page_table->num_virtual * v_page_table-> frame_size / 4)
        {
            printf("Virtual[%d] = %d\n",i,get(i, "Bubble"));
        }
        else if(i >= v_page_table->num_virtual * v_page_table-> frame_size / 4 && i < v_page_table->num_virtual * v_page_table-> frame_size / 2)
        {
            printf("Virtual[%d] = %d\n",i,get(i - v_page_table->num_virtual * v_page_table-> frame_size / 4 , "Quick"));
        }
        else if(i >= v_page_table->num_virtual * v_page_table-> frame_size / 2 && i < v_page_table->num_virtual * v_page_table-> frame_size * 3 / 4)
        {
            printf("Virtual[%d] = %d\n",i,get(i -  v_page_table->num_virtual * v_page_table-> frame_size / 2, "Merge"));
        }
        else if(i >= v_page_table->num_virtual * v_page_table-> frame_size * 3 / 4 && i < v_page_table -> num_virtual * v_page_table-> frame_size)
        {
            printf("Virtual[%d] = %d\n",i,get(indices[k++], "Index"));
        }


    }
    //PRINT LAST RESULTS

    printf("reads_bubble = %d\n", reads_bubble_get + reads_bubble_set);
    printf("writes_bubble = %d\n", writes_bubble_get + writes_bubble_set);
    printf("page_misses_bubble = %d\n", page_misses_bubble_get + page_misses_bubble_set);
    printf("page_replacements_bubble = %d\n", page_replacements_bubble_get + page_replacements_bubble_set);
    printf("disk_page_writes_bubble = %d\n", disk_page_writes_bubble_get + disk_page_writes_bubble_set);
    printf("disk_page_reads_bubble = %d\n", disk_page_reads_bubble_get + disk_page_reads_bubble_set);

    printf("\n\n\n");

    printf("reads_quick = %d\n", reads_quick_get + reads_quick_set);
    printf("writes_quick = %d\n", writes_quick_get + writes_quick_set);
    printf("page_misses_quick = %d\n", page_misses_quick_get + page_misses_quick_set);
    printf("page_replacements_quick = %d\n", page_replacements_quick_get + page_replacements_quick_set);
    printf("disk_page_writes_quick = %d\n", disk_page_writes_quick_get + disk_page_writes_quick_set);
    printf("disk_page_reads_quick = %d\n", disk_page_reads_quick_get + disk_page_reads_quick_set);

    printf("\n\n\n");

    printf("reads_merge = %d\n", reads_merge_get + reads_merge_set);
    printf("writes_merge = %d\n", writes_merge_get + writes_merge_set);
    printf("page_misses_merge = %d\n", page_misses_merge_get + page_misses_merge_set);
    printf("page_replacements_merge = %d\n", page_replacements_merge_get + page_replacements_merge_set);
    printf("disk_page_writes_merge = %d\n", disk_page_writes_merge_get + disk_page_writes_merge_set);
    printf("disk_page_reads_merge = %d\n", disk_page_reads_merge_get + disk_page_reads_merge_set);

    printf("\n\n\n");

    printf("reads_index = %d\n", reads_index_get + reads_index_set);
    printf("writes_index = %d\n", writes_index_get + writes_index_set);
    printf("page_misses_index = %d\n", page_misses_index_get + page_misses_index_set);
    printf("page_replacements_index = %d\n", page_replacements_index_get + page_replacements_index_set);
    printf("disk_page_writes_index = %d\n", disk_page_writes_index_get + disk_page_writes_index_set);
    printf("disk_page_reads_index = %d\n", disk_page_reads_index_get + disk_page_reads_index_set);
}


void print_statistics()
{
    //printf("reads_bubble = %d\n", reads_bubble_get + reads_bubble_set);
    //printf("writes_bubble = %d\n", writes_bubble_get + writes_bubble_set);
    //printf("page_misses_bubble = %d\n", page_misses_bubble_get + page_misses_bubble_set);
    printf("%d\t", page_replacements_bubble_get + page_replacements_bubble_set);
    //printf("disk_page_writes_bubble = %d\n", disk_page_writes_bubble_get + disk_page_writes_bubble_set);
    //printf("disk_page_reads_bubble = %d\n", disk_page_reads_bubble_get + disk_page_reads_bubble_set);



    //printf("reads_quick = %d\n", reads_quick_get + reads_quick_set);
    //printf("writes_quick = %d\n", writes_quick_get + writes_quick_set);
    //printf("page_misses_quick = %d\n", page_misses_quick_get + page_misses_quick_set);
    printf("%d\t", page_replacements_quick_get + page_replacements_quick_set);
    //printf("disk_page_writes_quick = %d\n", disk_page_writes_quick_get + disk_page_writes_quick_set);
    //printf("disk_page_reads_quick = %d\n", disk_page_reads_quick_get + disk_page_reads_quick_set);



    //printf("reads_merge = %d\n", reads_merge_get + reads_merge_set);
    //printf("writes_merge = %d\n", writes_merge_get + writes_merge_set);
    //printf("page_misses_merge = %d\n", page_misses_merge_get + page_misses_merge_set);
    printf("%d\t", page_replacements_merge_get + page_replacements_merge_set);
    //printf("disk_page_writes_merge = %d\n", disk_page_writes_merge_get + disk_page_writes_merge_set);
    //printf("disk_page_reads_merge = %d\n", disk_page_reads_merge_get + disk_page_reads_merge_set);



    //printf("reads_index = %d\n", reads_index_get + reads_index_set);
    //printf("writes_index = %d\n", writes_index_get + writes_index_set);
    //printf("page_misses_index = %d\n", page_misses_index_get + page_misses_index_set);
    printf("%d\n", page_replacements_index_get + page_replacements_index_set);
    //printf("disk_page_writes_index = %d\n", disk_page_writes_index_get + disk_page_writes_index_set);
    //printf("disk_page_reads_index = %d\n", disk_page_reads_index_get + disk_page_reads_index_set);
}
