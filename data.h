#ifndef DATA
#define DATA

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
	tasks

	Reading the file
	- create a 1440 long struct data array
	- check if the patient is the same
	- see which fields are null
	- see which lines are dupes (check patient id and minute)

	Data processing
	- compute the total calories burned, distance walked in miles, floors walked, and steps taken
	- compute average heartrate over the 24 hour period
	- find the max steps taken in one minute
	- find the longest consecutive range of poor sleep (from 9:00 pm to 8:00 am)
	  - one or more consecutive minutes where the sleepLevel > 1
	  - sleep is not based on the length of the range, but the sum of the sleep levels in the range
	  - the max sum of the ranges is considered the poorest sleep 
	  - report the starting and ending minutes of range

	Writing results
	- Results.csv
		- Line 1: Header line: Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max Steps,Sleep
		- Line 2: valueCalories,valueDistance,valueFloors,valueSteps,valueHeartrate,valu eMax,valueSleepStart:valueSleepEnd
		- Lines 3 - end: the filtered, deduped, and cleansed data in the original FitbitData.csv format
	- fclose both the files

*/

typedef struct string
{ 
	char str[256];
} String;

typedef enum sleep
{
	NONE = 0, ASLEEP = 1, AWAKE = 2, REALLYAWAKE = 3
} Sleep;

typedef struct fitbit
{
	char patient[10];
	char minute[9];
	double calories;
	double distance;
	unsigned int floors;
	unsigned int heartRate;
	unsigned int steps;
	Sleep sleepLevel;

} FitbitData;

/*---------------------------------- mystrtok -----
|  Purpose: behaves like strtok but with added functionality to return NULL in specific cases
|  Parameters:
|       str - string to tokenize
|       delim - char to tokenize by
|  Returns: pointer to substring
|  Post-Condition: make sure before using strcpy
|					1) that the string has been initalized
|					2) you check if (return_val != NULL && *return_val != '\0')
*-----------------------------------------------*/
char* mystrtok(char* str, const char* delim);

/*---------------------------------- getPatientCode -----
|  Purpose: reads patient code and 1st line and consumes the column header line
|  Parameters:
|       logFile - pointer to the source file (csv)
|  Returns: String (struct) patient code 
*-----------------------------------------------*/
String getPatientCode(FILE* infile);

/*---------------------------------- readLine -----
|  Purpose: logic to tokenize each line of the csv
|  Parameters:
|       line[] - entire line captured from fgets
|       patientID[] - patientID to cross verify
|  Returns: struct with filled out data
*-----------------------------------------------*/
FitbitData readLine(char lineSrc[100], char patientID[10]);

/*---------------------------------- timeToIndex -----
|  Purpose: converts string  into int minutes (ignores seconds)
|  Parameters:
|       time - string that contains the time in format "HH:MM:SS"
|  Returns: time in minutes
*-----------------------------------------------*/
int timeToIndex(char time[10]);

/*---------------------------------- readAndSortCSV -----
|  Purpose: contains logic to go through entire CSV, and sort and validate the data
|  Parameters:
|       infile - pointer to the csv file
|       data - array to holds all csv data
| 		patientID - to crosscheck each line
|  Returns: N/A, updates data array
*-----------------------------------------------*/
void readAndSortCSV(FILE *infile, FitbitData data[1440], char patientID[10]);


#endif 