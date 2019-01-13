#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define ZERO 0

//the day structure consists of the expenses and income
struct day {
	char da[10];
	float food;
	float transport;
	float vehicle;
	float fuel;
	float other;
	float income;
};

struct week {
	float day1;
	float day2;
	float day3;
	float day4;
	float day5;
	float day6;
	float day7;
};

//to make things easier
typedef struct day day;
typedef struct week week;

int enter_data(FILE* efile, char * d);
float get_diff(day * dy);
void data_week(week * cur_week, FILE * wf);
float add_expenses(week* cur_week);
void enter_week_d(FILE *wf, int daynum, float diff);
void init_week_d(FILE *wf, float diff);

int main(int argc, char * argv[]) {
	//initialising the variables 
	char d[10];
	int t = 0;
	char* fpath = (char*)malloc(sizeof(char) * 100);
	FILE * efile;

	//if no arguments are given the file path is the default
	if (argc == 1) {

		efile = fopen("./efile.txt", "a+");

		if (efile == NULL) return -2;

	}
	else if (argc == 3) {
		//else if the arguments meet the criteria a new file path is created
		if (strcmp(argv[1], "fp") == 0) {

			strcpy(fpath, argv[2]);
			efile = fopen(fpath, "a+");
			free(fpath);
			if (efile == NULL) return -2;

		}
		else {
			//if the second argument is not "fp" this message is shown
			printf("Wrong file path Usage: ./expenses fp $PATH\n");
			exit(-3);
		}

	}
	else {
		//if the arguments don't match the criteria an error message is given to the output
		printf("Wrong arguments for file path Usage: ./expenses fp $PATH\n");
		exit(-3);
	}

	//gets the day
	printf("what is the day today?\n");
	scanf("%s", &d);

	//the file is given to the function to add data		
	enter_data(efile, d);
	return 0;

}

//the function that adds data
int enter_data(FILE* efile, char* d) {

	float cost = 0;
	float diff = 0;
	int weeknum = 0;
	int daynum = 0;
	float w_expenses = 0;
	week *cur_week = (week*)malloc(sizeof(week));
	FILE * wn;	//week number file (has a constant name that contains the number)
	FILE * wf;	//week file (has a variable name)
	char *weekfile = (char*)malloc(sizeof(char) * 30);

	//goes to the end of the file
	fseek(efile, 0, SEEK_END);

	//initialising a structure to hold the values in the heap
	day* dy = (day*)malloc(sizeof(day));

	//copying the day to the output stream then to the structure and then to the file
	printf("day is %s", d);
	strcpy((dy->da), d);
	fprintf(efile, "\n\n%s\n", dy->da);

	//the data are entered to the structure then to the file
	printf("\nwhat is the cost for transport?\n");
	scanf("%f", &cost);
	dy->transport = cost;
	cost = 0;
	fprintf(efile, "transport : %f\n", dy->transport);

	printf("what is the cost for food?\n");
	scanf("%f", &cost);
	dy->food = cost;
	cost = 0;
	fprintf(efile, "food : %f\n", dy->food);

	printf("what is the cost for fuel?\n");
	scanf("%f", &cost);
	dy->fuel = cost;
	cost = 0;
	fprintf(efile, "transport : %f\n", dy->fuel);

	printf("what is the cost for vehicle?\n");
	scanf("%f", &cost);
	dy->vehicle = cost;
	cost = 0;
	fprintf(efile, "vehicle : %f\n", dy->vehicle);

	printf("what is the cost for other?\n");
	scanf("%f", &cost);
	dy->other = cost;
	cost = 0;
	fprintf(efile, "other : %f\n\n\n", dy->other);

	printf("what is the income for today?\n");
	scanf("%f", &cost);
	dy->income = cost;
	cost = 0;
	fprintf(efile, "income : %f\n\n\n", dy->income);

	//this gives the difference between the income and the expenses and shows it in the output and then entered to the file
	diff = get_diff(dy);
	printf("Total for today is : %f\n", diff);
	fprintf(efile, "difference : %f\n\n\n", diff);

	//for weekly calculations open another file and enter the difference and another file to get the week number
	//weeknum.txt has the last week number 
	wn = fopen("./weeknum.txt", "r+");
	if (wn == NULL) {

		wn = fopen("./weeknum.txt","w+");
		
		if(wn ==NULL) exit(3);

		fprintf(wn, "0 1");
		rewind(wn);

	};

	//get the current week and the current day
	//day number will be increased later on
	fscanf(wn, "%d %d", &weeknum, &daynum);

	//after getting the current week number open that file and add the difference after the day number the numbers for days are in integer form starting from 1 to 7
	 //open it in "w+" mode and go to end
	sprintf(weekfile, "./weekfiles/%d.txt", weeknum);
	
	if (daynum == 1) {
		//if the daynum is 1 initialise the format in the file
		wf = fopen(weekfile, "w+");
		if (wf == NULL) exit(-3);
		init_week_d(wf, diff);
	}
	else {
		//enter data to the weekfile if daynum is not 1
		wf = fopen(weekfile, "r+");
		if (wf == NULL) exit(-3);
		enter_week_d(wf, daynum, diff);
	}


	//add data to the week structure
	data_week(cur_week, wf);
	w_expenses = add_expenses(cur_week);

	//if its end of the week increase the weeknum by 1 and make daynum to 1 and write in the file wn
	if (daynum == 7) {
		daynum = 1;
		weeknum++;
		rewind(wn);
		fprintf(wn, "%d %d", weeknum, daynum);
	}
	else {
		daynum++;
		rewind(wn);
		fprintf(wn, "%d %d", weeknum, daynum);

	}

	//freeing the memory
	free(cur_week);
	free(weekfile);
	free(dy);
	fclose(efile);
	fclose(wn);
	fclose(wf);

	//if the daynum is 7 display the week balance orelse ask to show the current balance
	if (daynum == 7) {
		printf("\nThe difference for the week is : %f\n", w_expenses);
		return w_expenses;
	}
	else
	{
		char ans[2];
		printf("\nPress 'y' to show the difference till now\n");
		scanf("%s", &ans);
		if (strcmp(ans, "y") == 0 || strcmp(ans, "Y") == 0) {
			printf("Difference is : %f\n\n", w_expenses);
			return w_expenses;
		}
	}

	return 2;

}

//get data from the wf file and fill the cur_week structure
void data_week(week * cur_week, FILE * wf) {

	rewind(wf);
	fscanf(wf, "%f %f %f %f %f %f %f", &cur_week->day1, &cur_week->day2, &cur_week->day3, &cur_week->day4, &cur_week->day5, &cur_week->day6, &cur_week->day7);


}

//add the expenses for the current week and return the value
float add_expenses(week* cur_week) {

	float value = 0;
	value = cur_week->day1 + cur_week->day2 + cur_week->day3 + cur_week->day4 + cur_week->day5 + cur_week->day6 + cur_week->day7;

	return value;

}

//enter the numbers to the week file and initialise it if it is starting from the begining
void enter_week_d(FILE *wf, int daynum, float diff) {

	//byte offset from the end of the file
	signed int byte_offt = (-(((7 - daynum) * 2) + 1));

	//go to that offset and write the difference then while (7 - daynum) == 0 write ZERO 
	fseek(wf, byte_offt, SEEK_END);
	fprintf(wf, "%f", diff);

	while (7 - daynum) {
		fprintf(wf, " %d", ZERO);
		daynum++;
	}

}

void init_week_d(FILE *wf, float diff) {


	//initialize the file with the first day being the difference and the rest being zeros.
	fseek(wf, 0, SEEK_SET);
	fprintf(wf, "%f %d %d %d %d %d %d", diff, ZERO, ZERO, ZERO, ZERO, ZERO, ZERO);

}

float get_diff(day * dy) {

	//this functions returns the difference after calculating it
	float total = (dy->income) - (dy->fuel + dy->transport + dy->vehicle + dy->other + dy->food);

	return total;
}
