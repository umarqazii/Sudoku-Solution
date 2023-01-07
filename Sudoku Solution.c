#include <stdio.h>

#include <stdlib.h>

#include <pthread.h>

#include<semaphore.h>

#include <unistd.h>

#include <sys/types.h>

 

 



//valid sudoku

/*int matrix[9][9]={

        {4,3,5,  2,6,9  ,7,8,1},
        {6,8,2,  5,7,1  ,4,9,3},
        {1,9,7,  8,3,4  ,5,6,2},

        {8,2,6,  1,9,5  ,3,4,7},
        {3,7,4,  6,8,2  ,9,1,5},
        {9,5,1,  7,4,3  ,6,2,8},

        {5,1,9,  3,2,6  ,8,7,4},
        {2,4,8,  9,5,7  ,1,3,6},
        {7,6,3,  4,1,8  ,2,5,9}

    };

*/

 

//   

//invalid

int matrix[9][9]={

        {4,3,1,  2,6,2  ,7,8,1},
        {2,8,2,  1,7,1  ,3,9,3},
        {1,9,7,  8,1,4  ,5,6,2},

        {8,2,6,  1,9,5  ,3,4,7},
        {3,7,4,  6,8,2  ,9,1,5},
        {9,5,1,  7,4,3  ,6,2,8},

        {5,1,9,  3,2,6  ,8,7,4},
        {2,4,8,  9,5,7  ,1,3,6},
        {7,6,3,  4,1,8  ,2,5,9}
    };
 

int validrows = 0;
int validcolumns = 0;
int validgrids = 0;
int invalidentries = 0;

 

//semaphores declaration

sem_t s1;
sem_t s2;
sem_t s3;

// function to check rows

void *checkrow(void *arg){

    int t;
    int *r = (int *)arg;
    int num = 0;
    int count[9] = {0};
    for (int i = 0; i < 9; i++){
        count[matrix[*r][i] - 1]++;
    } 

    for (int i = 0; i < 9, r < 9; i++){
        do{
        if (count[i] != 1){
            invalidentries++;
            if (count[i] > 1){

                printf("(Row %d) is invalid", *r + 1);

                // display the repeated number
                printf(" because the number (%d) is repeated", i + 1);
                // display the position of the repeated number
                printf(" at position(s): ");
            
                int z = 0;
		for (int i = 0; i < 9; i++){
			if (count[i] == 0){

                        num = i + 1;
                        //printf("\n\n%d\n\n", num);

                        //break;
                    }
                }

                for (int j = 0; j < 9; j++){

                    if (matrix[*r][j] == i + 1){
			z++;
			if (z >= 0){

                            printf("(%d,%d) ", *r + 1, j + 1);
			    sem_wait(&s1);
			    matrix[*r][j] = num;
			    sem_post(&s1);

                            break;

                        }
                    }
                }
                printf("\nCalculated by thread with threadID '%d' on the %dth index.\n", gettid(), *r);
                printf("\n\t\t\t\t       ------------\n\n");
                pthread_exit(NULL);
            }

            if (count[i] == 0){

                printf("(Row %d) is invalid", *r + 1);
		// display the missing number
		printf(" because the number (%d) is missing", i + 1);
		printf("\nCalculated by thread with threadID '%d' on the %dth index.\n", gettid(), *r);
		printf("\n\t\t\t\t       ------------\n\n");
                pthread_exit(NULL);

            }

        }
        }while(count[i] != 1);

    }
    validrows++;

}

 

// function to check columns

void *checkcolumn(void *arg){

    int *c = (int *)arg;
    int num = 0;
    int count[9] = {0};

    for (int i = 0; i < 9; i++){
        count[matrix[i][*c] - 1]++;
    }

    for (int i = 0; i < 9, *c < 9; i++){
        do{
        if (count[i] != 1){
		invalidentries++;
		if (count[i] > 1){

                printf("(Column %d) is invalid", *c + 1);
		// display the repeated number
                printf(" because the number (%d) is repeated", i + 1);
		// display the position of the repeated number
                printf(" at position(s): ");

                int z = 0;

                for (int i = 0; i < 9; i++){
                    if (count[i] == 0){

                        num = i + 1;
			//printf("\n\n%d\n\n", num);
                        // break;

                    }
                }

                for (int j = 0; j < 9; j++){

                    if (matrix[j][*c] == i + 1){

                        z++;
                        if (z > 0){

                            printf("(%d,%d) ", j + 1, *c + 1);

                            sem_wait(&s2);
                            matrix[j][*c] = num;
                            sem_post(&s2);
                           
                            break;

                        }
                    
                    }

                }

                printf("\nCalculated by thread with threadID '%d' on the %dth index.\n", gettid(), *c);
                
                printf("\n\t\t\t\t       ------------\n\n");
                pthread_exit(NULL);

            }

            if (count[i] == 0){

                printf("(Column %d) is invalid", *c + 1);

                // display the missing number

                printf(" because the number (%d) is missing", i + 1);

                printf("\nCalculated by thread with threadID '%d' on the %dth index.\n", gettid(), *c);
                
                printf("\n\t\t\t\t       ------------\n\n");

                pthread_exit(NULL);

            }
        }
        
    }while(count[i] != 1);
    }

    validcolumns++;

}

 

// function to check grids

void *checkGrid(void *arg){

    int g = *(int *)arg;

    int i, j, k = 0;

    int store[9];

    for (i = 0; i < 3; i++){

        for (j = 0; j < 3; j++){

            store[k] = matrix[i + 3 * (g / 3)][j + 3 * (g % 3)];

            k++;
        }

    }

    // check that in store array, all numbers are unique from 1 to 9

    int count[9] = {0};

    for (i = 0; i < 9; i++){
        count[store[i] - 1]++;
    }
    
    for (i = 0; i < 9; i++){

        if (count[i] != 1){

            invalidentries++;

            if (count[i] > 1){

                printf("(Grid %d) is invalid", g + 1);

                // display the repeated number

                printf(" because the number (%d) is repeated", i + 1); 

                // display the position of the repeated number

                printf(" at position(s): ");
 
                for (int j = 0; j < 9; j++){

                    if (store[j] == i + 1){

                        printf("(%d,%d) ", j / 3 + 1 + 3 * (g / 3), j % 3 + 1 + 3 * (g % 3));
                    }
                }
                
                printf("\nCalculated by thread with threadID '%d' on the %dth index.\n", gettid(), g);
                
                printf("\n\t\t\t\t       ------------\n\n");

                pthread_exit(NULL);
            }
            
            if (count[i] == 0){

                printf("(Grid %d) is invalid", g + 1);

                // display the missing number

                printf(" because the number (%d) is missing", i + 1);
                
                printf("\nCalculated by thread with threadID '%d' on the %dth index.\n", gettid(), g);
                
                printf("\n\t\t\t\t       ------------\n\n");

                pthread_exit(NULL);
            }
        }
    }

    validgrids++;

}
 

void DisplaySudokuGrid(){

    printf(" -------------------------");

    printf("\n");

    printf("|       SUDOKU BOARD           |");
 
    printf("\n");

    printf(" -------------------------");

    printf("\n");

    for (int k = 0; k < 27; k++)
        printf("-");
    printf("\n");
 
    for (int r = 0; r < 9; r++){
        printf("| ");

        for (int c = 0; c < 9; c++)

        {

            if (c == 3 || c == 6)

                printf(" | ");

            printf("%d ", matrix[r][c]);

        }

 

        printf("| ");

        if (r == 2 || r == 5)

        {

            printf("\n");

            for (int k = 0; k < 27; k++)

                printf("-");

        }

        printf("\n");

    }

 

    for (int k = 0; k < 27; k++)

        printf("-");

 

    printf("\n");

}

 

void DisplayResSudokuGrid()

{

    printf(" -------------------------");

    printf("\n");

    printf("| RESULTANT SUDOKU BOARD |");

    printf("\n");

    printf(" -------------------------");

    printf("\n");

 

    for (int k = 0; k < 27; k++)

        printf("-");

    printf("\n");

    for (int r = 0; r < 9; r++)

    {

        printf("| ");

        for (int c = 0; c < 9; c++)

        {

            if (c == 3 || c == 6)

                printf(" | ");

            printf("%d ", matrix[r][c]);

        }

        printf("| ");

        if (r == 2 || r == 5)

        {

            printf("\n");

            for (int k = 0; k < 27; k++)

                printf("-");

        }

        printf("\n");

    }

 

    for (int k = 0; k < 27; k++)

        printf("-");

    printf("\n");

}

 

int main()

{

 

    // declare threads

    pthread_t r[9];

    pthread_t c[9];

    pthread_t g[9];

 

    //semaphore initialization

    sem_init(&s1, 0, 1);

    sem_init(&s2, 0, 1);

    sem_init(&s3, 0, 1);

 

    // display the sudoku board

    DisplaySudokuGrid();

 

    printf("\n");

 

printf("------------------------------------ROW INFORMATION-----------------------------------\n\n");

              //threads for rows' validation

              for(int i=0;i<9;i++){

        pthread_create(&r[i], NULL, &checkrow, &i);

        pthread_join(r[i], NULL);

              }

printf("--------------------------------------------------------------------------------------\n\n");

 

 

printf("-----------------------------------COLUMN INFORMATION---------------------------------\n\n");

              //threads for columns' validation

              for(int i=0;i<9;i++){

        pthread_create(&c[i], NULL, &checkcolumn, &i);

        pthread_join(c[i], NULL);

              }

printf("--------------------------------------------------------------------------------------\n\n");

 

 

printf("------------------------------------GRID INFORMATION----------------------------------\n\n");

              //threads for grids' validation

              for(int i=0;i<9;i++){

        pthread_create(&g[i], NULL, &checkGrid, &i);

        pthread_join(g[i], NULL);

              }

printf("--------------------------------------------------------------------------------------\n\n");

 

              printf("\n");

              //display no of valid rows

              printf("\nNumber of Valid rows: %d",validrows);

 

              //display no of valid columns

              printf("\nNumber of Valid columns: %d",validcolumns);

 

              //display no of valid grids

              printf("\nNumber of Valid Grids: %d",validgrids);

              printf("\n");

 

    //display no of invalid entries
	      if(invalidentries > 1){
             	 printf("Number of Invalid Entries: %d\n",invalidentries - 2);
              }else{
		 printf("Number of Invalid Enteries: %d\n", 0);
	      }

 

printf("\n\n\n-----------------------------------RESULTANT SUDOKU MATRIX---------------------------------\n\n");

 

                DisplayResSudokuGrid();

 

printf("--------------------------------------------------------------------------------------\n\n");

}
