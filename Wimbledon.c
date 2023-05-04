/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct Wimbledon_s 
{
    int year;
    char Won_Name[50];
    char Won_Country[50];
    char Lost_Name[50];
    char Lost_Country[50];
}Wimbledon;

void Load_Struct(Wimbledon* P, FILE* fid)
{
    char wfirst[50], wlast[50], lfirst[50], llast[50];
    fscanf(fid, "%d", &P -> year);
    fscanf(fid, "%s", wfirst);
    fscanf(fid, "%s", wlast);
    strcpy(P -> Won_Name, wfirst);
    strcat(P -> Won_Name, " ");
    strcat(P -> Won_Name, wlast);
    fscanf(fid, "%s", P -> Won_Country);
    fscanf(fid, "%s", lfirst);
    fscanf(fid, "%s", llast);
    strcpy(P -> Lost_Name, lfirst);
    strcat(P -> Lost_Name, " ");
    strcat(P -> Lost_Name, llast);
    fscanf(fid, "%s", P -> Lost_Country);
}

void print_winner(Wimbledon* P)
{
    printf("%d\t%s\t%s\n", P -> year, P -> Won_Country, P -> Lost_Country);
}

int main()
{
    FILE *inp;
    inp = fopen("Wimbledon_Tennis_Data.txt", "r");
    int players, check = 0, count = 0; 
    char option;
    char country[69];
    char name[69];
    printf("Enter number of players: ");
    scanf("%d", &players);
    Wimbledon *allwinners = (Wimbledon *)malloc(players * sizeof(Wimbledon) );
    
    if (inp == NULL)
    {
        printf("you did something wrong");
    }
    
    for (int i = 0; i < players; i++)
    {
        Load_Struct(&allwinners[i], inp);
    }
    fclose(inp);
    
    while(1)
    {
        char cname[50];
        printf("Which country would you like to see? ");
        scanf("%s", cname);
        
        for (int j = 0; j < players; j++)
        {
            if (strcmp(allwinners[j].Won_Country, cname) == 0 || strcmp(allwinners[j].Lost_Country, cname) == 0 )
            {
                
                    if(strcmp(allwinners[j].Won_Country, allwinners[j].Lost_Country) == 0)
                    {
                        count = count + 1;
                    }
                    
                count = count + 1;
            }
        }
        
        printf("%s has played %d times\n", cname, count);
        printf("Year\tWon\tLost\n");
        
        for (int j = 0; j < players; j++)
        {
            if (strcmp(allwinners[j].Won_Country, cname) == 0 || strcmp(allwinners[j].Lost_Country, cname) == 0 )
            {
                print_winner(&allwinners[j]);
                
                    if(strcmp(allwinners[j].Won_Country, allwinners[j].Lost_Country) == 0)
                    {
                        print_winner(&allwinners[j]);
                    }
                check = 420;
            }
        }
        
        if (check == 0)
        {
            printf("%s is not in the list\n", cname);
        }
        
        printf("Continue (Q to quit)? ");
        scanf(" %c", &option);
        printf("\n");
        if(option == 'Q')
        {
            break;
        }
        
        if(option != 'Q')
        {
            count = 0;
        }
        
    }

    return 0;
}

