#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//the day structure consists of the expenses and income
struct day{
	char da[10];
	float food;
	float transport;
	float vehicle;
	float fuel;
	float other;
	float income;
};

//to make things easier
typedef struct day day;


void enter_data(FILE* efile, char * d);
float get_diff(day * dy);

int main (int argc, char * argv[]){
	//initialising the variables 
	char d[10];
	int t = 0;
	char* fpath = (char*)malloc(sizeof(char)*100);
	FILE * efile;

	//if no arguments are given the file path is the default
	if(argc == 1){
		
		efile = fopen("./efile.txt", "a+");
		
		if(efile == NULL) return -2;

	}
	else if(argc == 3){
		//else if the arguments meet the criteria a new file path is created
		if (strcmp(argv[1],"fp") == 0){

		strcpy(fpath, argv[2]);
		efile = fopen(fpath, "a+");
		free(fpath);
		if(efile == NULL) return -2;

		}
		else{
			//if the second argument is not "fp" this message is shown
			printf("Wrong file path Usage: ./expenses fp $PATH\n");
			exit(-3);		
		}

	}
	else{
		//if the arguments don't match the criteria an error message is given to the output
		printf("Wrong arguments for file path Usage: ./expenses fp $PATH\n");
		exit(-3);
	}

//gets the day
	printf("what is the day today?\n");
	scanf("%s",&d);

//the file is given to the function to add data		
	enter_data(efile, d);

}

//the function that adds data
void enter_data(FILE* efile, char* d){

	float cost = 0;
	float diff;
	
	//goes to the end of the file
	fseek(efile, 0, SEEK_END);	
	
	//initialising a structure to hold the values in the heap
	day* dy = (day*) malloc(sizeof(day));
	
	//copying the day to the output stream then to the structure and then to the file
	printf("day is %s", d);
	strcpy((dy->da), d);
	fprintf(efile, "\n\n%s\n",dy->da);	

	//the data are entered to the structure then to the file
	printf("\nwhat is the cost for transport?\n");
	scanf("%f", &cost);
	dy->transport = cost;
	cost = 0;
	fprintf(efile, "transport : %f\n",dy->transport);

	printf("what is the cost for food?\n");
	scanf("%f", &cost);
	dy->food = cost;
	cost = 0;
	fprintf(efile, "food : %f\n",dy->food);

	printf("what is the cost for fuel?\n");
	scanf("%f", &cost);
	dy->fuel = cost;
	cost = 0;
	fprintf(efile, "transport : %f\n",dy->fuel);
	
	printf("what is the cost for vehicle?\n");
	scanf("%f", &cost);
	dy->vehicle = cost;
	cost = 0;
	fprintf(efile, "vehicle : %f\n",dy->vehicle);

	printf("what is the cost for other?\n");
	scanf("%f", &cost);
	dy->other = cost;
	cost = 0;
	fprintf(efile, "other : %f\n\n\n",dy->other);
	
	printf("what is the income for today?\n");
	scanf("%f", &cost);
	dy->income = cost;
	cost = 0;
	fprintf(efile, "income : %f\n\n\n",dy->income);

	//this gives the difference between the income and the expenses and shows it in the output and then entered to the file
	diff = get_diff(dy);
	printf("Total for today is : %f\n",diff);
	fprintf(efile, "difference : %f\n\n\n",diff);
	
	free(dy);
	fclose(efile);
	
}

float get_diff(day * dy){

	//this functions returns the difference after calculating it
	float total = (dy->income) - (dy->fuel + dy->transport + dy->vehicle + dy->other + dy->food);

	return total;
}
