/*******************************************************************************************
 * Programmer: Tarang Teredesai
 * Class: CptS 122, Spring 2025; Lab Section 4
 * Programming Assignment: Programming Assignment 1
 * Date: January 15, 2025
 * Description: Reading and cleaning data from CSV
 *******************************************************************************************/

#include "data.h"

int main(int argc, char *argv[])
{

	FitbitData data[1440];
	String patientCode;

	FILE *infile = fopen("FitbitData.csv", "r");
	FILE *outfile = fopen("Results.csv", "w");

	patientCode = getPatientCode(infile);
	printf("Patient Code: %s\n", patientCode.str);

	readAndSortCSV(infile, data, patientCode.str);

	double
		totalCalories = 0.0, // lf
		totalDistance = 0,	 // lf

		totalFloors = 0,	// uint
		totalSteps = 0,		// uint
		avgHeartRate = 0.0, // uint
		maxSteps = 0;		// uint

	int maxSleepSum = 0;
	int maxSleepStart = -1;
	int maxSleepEnd = -1;
	int currentSleepSum = 0;
	int currentSleepStart = -1;

	int counter = 0;
	for (int i = 0; i < 1440; ++i)
	{
		if (strcmp(data[i].patient, patientCode.str) == 0)
		{
			// lf
			if (data[i].calories != -1)
				totalCalories += (double)data[i].calories;
			if (data[i].distance != -1)
				totalDistance += (double)data[i].distance;

			// uint
			totalFloors += (double)data[i].floors;
			totalSteps += (double)data[i].steps;
			if (data[i].steps > maxSteps)
				maxSteps = (double)data[i].steps;

			// avg
			avgHeartRate += (double)data[i].heartRate;
			if (data[i].heartRate != 0)
				++counter;

			if (data[i].sleepLevel > 1 && (i >= 1260 || i < 480))
			{
				if (currentSleepStart == -1) currentSleepStart = i;

				currentSleepSum += data[i].sleepLevel;

				if (currentSleepSum > maxSleepSum)
				{
					maxSleepSum = currentSleepSum;
					maxSleepStart = currentSleepStart;
					maxSleepEnd = i;
				}
			}
			else
			{
				currentSleepSum = 0;
				currentSleepStart = -1;
			}
		}
	}

	avgHeartRate /= counter;

	printf("sum = %d, start = %d, end = %d\n", maxSleepSum, maxSleepStart, maxSleepEnd);

	fprintf(outfile, "Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max Steps,Sleep\n");
	fprintf(outfile, "%lf,%lf,%lf,%lf,%lf,%lf,Best Sleep: %d, %s, %s\n\n", totalCalories, totalDistance, totalFloors, totalSteps, avgHeartRate, maxSteps, maxSleepSum, data[maxSleepStart].minute, data[maxSleepEnd].minute);
	fprintf(outfile, "Patient,minute,calories,distance,floors,heart,steps,sleep_level\n");

	for (int i = 0; i < 1440; ++i)
	{
		fprintf(outfile, "%s, %s, %lf, %lf, %d, %d, %d, %d\n", data[i].patient, data[i].minute, data[i].calories, data[i].distance, data[i].floors, data[i].heartRate, data[i].steps, data[i].sleepLevel);
	}

	fclose(infile);
	fclose(outfile);
	return 0;
}