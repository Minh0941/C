/*Author: Minh Le
Date Created: November 8th, 2022
Inputs: a number between 1-4, movie amount followd by one of these: 

a test word, year, rating or duration (in minutes)
Ouputs: List of movies based on a word, year, rating or duration

Algorihim:

First enter the amount of movies you'd like to check in the list:
    If 1 is entered next:
        searches movies with a certain keyword in them in the amount listed up to 10, sorted by rating
        choice to quit (q or Q)
        
    If 2 is entered next
        searches movies with a certain rating given to them in the amount listed up to 10, sorted by rating
        choice to quit (q or Q)
        
    If 3 is entered next
        searches movies made in  a certain year listed up to 10, sorted by rating
        choice to quit (q or Q)
        
    If 4 is entered next
        searches movies with a certain duraton listed up to 10, sorted by rating
        choice to quit (q or Q)
        
    If anyhting else is entered
        asked to type a number 1-4
        
*/


 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct movie_s 
{
	char title[100];
	double rating; //rating from 1 - 10
	int duration; //in minutes
	int year;
}movies;

void swap(movies* x, movies* y) 
{
	movies temp;
	temp = *x;
	*x = *y;
	*y = temp;
}

void selection_sort(movies x[], int size) 
{
	int i, j, max;
	for (i = 0; i < size; i++) {
		max = i;// start searching from currently unsorted
		for (j = i; j < size; j++) {
			if (x[j].rating > x[max].rating) { //change it to use “rating” member of movies
				max = j;
			}//if found a larger element, move it to the front
		}
		swap(&x[i], &x[max]);
	}
}

void fileReader(movies* mov,FILE* inp) 
{
	char temp;
	fgets(mov -> title, 100, inp);

	mov -> title[strlen(mov -> title) - 1] = '\0';
	
	fscanf(inp,"%lf", &mov -> rating);
	fscanf(inp,"%d", &mov -> duration);
	fscanf(inp,"%d", &mov -> year);
	fscanf(inp,"%c", &temp);
}

void printmovie(movies* P) 
{
	printf("%.2lf %s(%d, %d mins)\n", P -> rating, P -> title,  P -> year, P -> duration);
}


int main(void) 
{
    FILE* inp;
	inp = fopen("movies_database.txt", "r");
	movies* movieslist;
	int num, i, option;
	char choice, keyword[50];
	
	printf("Enter number of movies you want to use from the database: ");
	scanf("%d", &num);
	
	movieslist = (movies*)malloc(sizeof(movies) * num);
	
	for (i = 0; i < num; i++) 
	{
		fileReader(&movieslist[i], inp);
	}

	selection_sort(movieslist, num);

	while(1) 
	{
		printf("1: search movie(s) with a keyword\n");
	    printf("2: search movie(s) by above rating\n");
	    printf("3: search movie(s) by year\n");
	    printf("4: search movie(s) by above duration\n\n");
		printf("Choose 1-4 from the above options: ");
		scanf("%d", &option);
		
		while (option != 1 && option != 2 && option != 3 && option != 4) 
		{
			printf("Choose 1-4 from the above options: ");
			scanf("%d", &option);
		}
		
		if (option == 1) 
		{
			int count = 0, count1 = 0;
			printf("\nEnter the keyword: ");
			scanf("%s", keyword);
			
			for (int y = 0; y < num; y++) 
			{
				if (strstr(movieslist[y].title, keyword) != NULL) 
				{
					count++;
				}
			}
			
			if (count == 0) 
			{
				printf("No movies with %s\n", keyword);
			}
			
			else 
			{
				printf("%d Movies with %s\n", count, keyword);
			}
			
			if (count > 10) 
			{
				printf("First 10 movies sorted by rating are\n");
			}

			for (int y = 0; y < num; y++) 
			{
				if (strstr(movieslist[y].title, keyword) != NULL) 
				{
					printmovie(&movieslist[y]);
					count1++;
				}
				if (count1 == 10) 
				{
					break;
				}
			}
		}
		
		else if (option == 2) 
		{
		    double irate;
			int count, count2 = 0;
			
			printf("\nEnter rating: ");
			scanf("%lf", &irate);
			
			for (int y = 0; y < num; y++) 
			{
				if (movieslist[y].rating >= irate) 
				{
					count++;
				}
			}
			
			if (count == 0) 
			{
				printf("No movies with rating higher than %.2lf\n", irate);
			}
			
			else 
			{
				printf("%d Movies with ratings higher than %.2lf\n", count, irate);
			}
			
			if (count > 10) 
			{
				printf("First 10 movies sorted by rating are\n");
			}

			for (i = 0; i < num; i++) 
			{
				if (movieslist[i].rating >= irate) 
				{
					printmovie(&movieslist[i]);
					count2++;
					
					if (count2 == 10) 
					{
						break;
				    }
				}
			}
		}
		
		else if (option == 3) 
		{
			int year_int;
			int count = 0;
			int count3 = 0;
			printf("\nEnter year: ");
			scanf("%d", &year_int);
			
			for (i= 0; i < num; i++) 
			{
				if (movieslist[i].year == year_int) 
				{
					count++;
				}
			}
			
			if (count == 0) 
			{
				printf("No movies with year %d\n", year_int);
			}
			
			else 
			{
				printf("%d Movies with year %d\n", count, year_int);
			}
			
			if (count > 10) 
			{
				printf("First 10 movies sorted by rating are\n");
			}
			
			for (int y = 0; y < num; y++) 
			{
				if (movieslist[y].year == year_int) 
				{
					printmovie(&movieslist[y]);
					count3++;
				}
				if (count3 == 10) 
				{
					break;
				}
			}

		}
		
		else if (option == 4) 
		{
			int timecheck;
			int count = 0;
			int count4 = 0;
			printf("\nEnter duration (in mins): ");
			scanf("%d", &timecheck);
			for (int y = 0; y < num; y++) 
			{
				if (movieslist[y].duration >= timecheck) 
				{
					count++;
				}
			}
			
			if (count == 0) 
			{
				printf("No movies with duration higher than %d\n", timecheck);
			}
			
			else 
			{
				printf("%d Movies with duration higher than %d\n", count, timecheck);
			}
			
			if (count > 10) 
			{
				printf("First 10 movies sorted by rating are\n");
			}
			
			for (i = 0; i < num; i++) 
			{
				if (movieslist[i].duration >= timecheck) 
				{
					printmovie(&movieslist[i]);
					count4++;
				}
				if (count4 == 10) 
				{
					break;
				}
			}
		}
		
		printf("Continue (q/Q to quit)? ");
		scanf(" %c", &choice);
		
		if(choice == 'q' || choice == 'Q')
		{
		    break;
		}
		
		printf("\n");
		
	}
  
   return 0;
}