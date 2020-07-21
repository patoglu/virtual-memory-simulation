#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void find_efficient_frame_size(int* bubble_values, int* quick_values, int *merge_values, int *index_values,int *bubble_index, int *quick_index, int *merge_index, int *index_index);
void print_best_page(int *frequency_b, int *frequency_q, int *frequency_m, int * frequency_i);
int results[64][4];
int main(int argc, char const *argv[])
{

    int bubble_index;
    int quick_index;
    int merge_index;
    int index_index;
    int bubble_values[5];
    int quick_values[5];
    int merge_values[5];
    int index_values[5];

    int frequency_bubble[5];
    int frequency_quick[5];
    int frequency_merge[5];
    int frequency_index[5];

    double bubble_replacement_average = 0.0;
    double quick_replacement_average = 0.0;
    double merge_replacement_average = 0.0;
    double index_replacement_average = 0.0;
    int smallest_bubble;;
    int results[64][4];
    int arr[5];
    int i;
    int a = 0;
    int k = 0;
    char command[50];
    char buffer[100];
    int frame_size;
    int num_physical;
    FILE *stats;
    int num_virtual;
    if(argc != 2 )
    {
        fprintf(stderr, "%s\n","Bad parameter" );
        exit(1);
    }
    char command_list[6][100]= {
                            "./sortArrays 5 2 4 FIFO global 10000 disk.dat >> stats.txt",
                            "./sortArrays 4 3 5  FIFO global 10000 disk.dat >> stats.txt",
                            "./sortArrays 3 4 6 FIFO global 10000 disk.dat >> stats.txt",
                            "./sortArrays 2 5 7 FIFO global 10000 disk.dat >> stats.txt",
                            "./sortArrays 1 6 8  FIFO global 10000 disk.dat >> stats.txt",
                        };


    char command_list_NRU[6][100] = {"./sortArrays 5 2 4 NRU global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 4 3 5  NRU global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 3 4 6 NRU global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 2 5 7 NRU global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 1 6 8  NRU global 10000 disk.dat >> stats.txt"};

    char command_list_SC[6][100] = {"./sortArrays 5 2 4 SC global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 4 3 5  SC global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 3 4 6 SC global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 2 5 7 SC global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 1 6 8  SC global 10000 disk.dat >> stats.txt"};
    char command_list_LRU[6][100] = {"./sortArrays 5 2 4 LRU global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 4 3 5  LRU global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 3 4 6 LRU global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 2 5 7 LRU global 10000 disk.dat >> stats.txt",
                                    "./sortArrays 1 6 8  LRU global 10000 disk.dat >> stats.txt"};


   while(k++ < 10)
   {
       for(i  = 0 ; i < 5 ; ++i)
       {
            if(strcmp(argv[1], "NRU") == 0)
            {
                printf("Executing command %s\n", command_list_NRU[i]);
                system(command_list_NRU[i]);
            }
            else if(strcmp(argv[1], "FIFO") == 0)
            {
                printf("Executing command %s\n", command_list[i]);
                system(command_list[i]);
            }
            else if(strcmp(argv[1], "SC") == 0)
            {
                printf("Executing command %s\n", command_list_SC[i]);
                system(command_list_SC[i]);
            }
            else if(strcmp(argv[1], "LRU") == 0)
            {
                printf("Executing command %s\n", command_list_LRU[i]);
                system(command_list_LRU[i]);
            }


       }

       stats = fopen("stats.txt", "r");
       i = 0;
       while (EOF != fscanf(stats, "%d\t%d\t%d\t%d\n", &results[i][0], &results[i][1], &results[i][2], &results[i][3]))
        {
             printf("> %d %d %d %d\n",  results[i][0], results[i][1], results[i][2], results[i][3]);

             bubble_replacement_average += results[i][0];
             quick_replacement_average +=  results[i][1];
             merge_replacement_average +=  results[i][2];
             index_replacement_average +=  results[i][3];
             ++i;

        }
        for(a = 0 ; a < 5 ; ++a)
        {
            bubble_values[a] = results[0][a];
        }
        for(a = 0 ; a < 5 ; ++a)
        {
            quick_values[a] = results[1][a];
        }
        for(a = 0 ; a < 5 ; ++a)
        {
            merge_values[a] = results[2][a];
        }
        for(a = 0 ; a < 5 ; ++a)
        {
            index_values[a] = results[3][a];
        }
        find_efficient_frame_size(bubble_values, quick_values, merge_values, index_values, &bubble_index, &quick_index, &merge_index, &index_index);
        printf("Indexes are %d %d %d %d \n",bubble_index,quick_index, merge_index, index_index  );
        frequency_bubble[bubble_index]++;
        frequency_quick[quick_index]++;
        frequency_merge[merge_index]++;
        frequency_index[index_index]++;
        fclose(stats);
        system("rm stats.txt");
   }
   print_best_page(frequency_bubble, frequency_quick, frequency_merge, frequency_index);




   return(0);
}
void find_efficient_frame_size(int* bubble_values, int* quick_values, int *merge_values, int *index_values,int *bubble_index, int *quick_index, int *merge_index, int *index_index)
{
    *bubble_index = 0;
    *quick_index = 0;
    *merge_index = 0;
    *index_index = 0;
    int c;
    int min_bubble = bubble_values[0];
    int min_quick = quick_values[0];
    int min_merge = merge_values[0];
    int min_index = index_values[0];
    for(c = 1 ; c < 5 ; ++c)
    {
        if(bubble_values[c] < min_bubble)
        {
            min_bubble = bubble_values[c];
            *bubble_index = c + 1;
        }
    }


    for(c = 1 ; c < 5 ; ++c)
    {
        if(quick_values[c] < min_quick)
        {
            min_quick = quick_values[c];
            *quick_index = c + 1;
        }
    }


    for(c = 1 ; c < 5 ; ++c)
    {
        if(merge_values[c] < min_merge)
        {
            min_merge = merge_values[c];
            *merge_index = c + 1;
        }
    }


    for(c = 1 ; c < 5 ; ++c)
    {
        if(index_values[c] < min_index)
        {
            min_index = index_values[c];
            *index_index = c + 1;
        }
    }
}

void print_best_page(int *frequency_b, int *frequency_q, int *frequency_m, int * frequency_i)
{
    int max_b = frequency_b[0];
    int max_q = frequency_q[0];
    int max_m = frequency_m[0];
    int max_i = frequency_i[0];
    int location_b;
    int location_q;
    int location_m;
    int location_i;
    int i;
    printf("Best page size for bubble sort is: \n");
    for(i = 1 ; i < 5 ; ++i)
    {
        if(frequency_b[i] > max_b)
        {
            max_b = frequency_b[i];
            location_b = i + 1;
        }
    }

    /*

    char command_list[6][100]= {
                            "./sortArrays 5 2 4 FIFO global 10000 disk.dat >> stats.txt",
                            "./sortArrays 4 3 5  FIFO global 10000 disk.dat >> stats.txt",
                            "./sortArrays 3 4 6 FIFO global 10000 disk.dat >> stats.txt",
                            "./sortArrays 2 5 7 FIFO global 10000 disk.dat >> stats.txt",
                            "./sortArrays 1 6 8  FIFO global 10000 disk.dat >> stats.txt",
                        };


                        */
    if(location_b == 0)
    {
        printf("Page size 32 is most ideal for bubble sort.\n" );
    }
    else if(location_b == 1)
    {
        printf("Page size 16 is most ideal for bubble sort.\n" );
    }
    else if(location_b == 2)
    {
        printf("Page size 8 is most ideal for bubble sort.\n" );
    }
    else if(location_b == 3)
    {
        printf("Page size 4 is most ideal for bubble sort.\n" );
    }
    else if(location_b == 4)
    {
        printf("Page size 2 is most ideal for bubble sort.\n" );
    }
    printf("Best page size for quick sort is: \n");



    for(i = 1 ; i < 5 ; ++i)
    {
        if(frequency_q[i] > max_q)
        {
            max_q = frequency_q[i];
            location_q = i + 1;
        }
    }



    if(location_q == 0)
    {
        printf("Page size 32 is most ideal for quick sort.\n" );
    }
    else if(location_q == 1)
    {
        printf("Page size 16 is most ideal for quick sort.\n" );
    }
    else if(location_q == 2)
    {
        printf("Page size 8 is most ideal for quick sort.\n" );
    }
    else if(location_q == 3)
    {
        printf("Page size 4 is most ideal for quick sort.\n" );
    }
    else if(location_q == 4)
    {
        printf("Page size 2 is most ideal for quick sort.\n" );
    }






    printf("Best page size for merge sort is: \n");


    for(i = 1 ; i < 5 ; ++i)
    {
        if(frequency_m[i] > max_m)
        {
            max_m = frequency_m[i];
            location_m = i + 1;
        }
    }



    if(location_m == 0)
    {
        printf("Page size 32 is most ideal for merge sort.\n" );
    }
    else if(location_m == 1)
    {
        printf("Page size 16 is most ideal for merge sort.\n" );
    }
    else if(location_m == 2)
    {
        printf("Page size 8 is most ideal for merge sort.\n" );
    }
    else if(location_m == 3)
    {
        printf("Page size 4 is most ideal for merge sort.\n" );
    }
    else if(location_m == 4)
    {
        printf("Page size 2 is most ideal for merge sort.\n" );
    }


    printf("Best page size for index sort is: \n");

    for(i = 1 ; i < 5 ; ++i)
    {
        if(frequency_i[i] > max_i)
        {
            max_i = frequency_i[i];
            location_i = i + 1;
        }
    }



    if(location_i == 0)
    {
        printf("Page size 32 is most ideal for index sort.\n" );
    }
    else if(location_i == 1)
    {
        printf("Page size 16 is most ideal for index sort.\n" );
    }
    else if(location_i == 2)
    {
        printf("Page size 8 is most ideal for index sort.\n" );
    }
    else if(location_i == 3)
    {
        printf("Page size 4 is most ideal for index sort.\n" );
    }
    else if(location_i == 4)
    {
        printf("Page size 2 is most ideal for index sort.\n" );
    }

}
