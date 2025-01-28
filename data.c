#include "data.h"

char* mystrtok(char* str, const char* delim)
{
	static char* nextToken = NULL; //store the next token position
	if (str != NULL) nextToken = str; //new string - reset static variable
	if (nextToken == NULL) return NULL; // If there are no more tokens, return NULL

	//if the current character is a delimiter (prevents later fields from being returned if target is null)
	if (strchr(delim, *nextToken) != NULL) {
		char* emptyToken = nextToken;
		*nextToken = '\0';
		nextToken++;
		return emptyToken;
	}

	// Find the start of the next token
	char* tokenStart = nextToken;
	while (*tokenStart != '\0' && strchr(delim, *tokenStart) != NULL) tokenStart++; //skip delimiter characters

	if (*tokenStart == '\0') {
		nextToken = NULL; // no more tokens
		return NULL;
	}

	char* tokenEnd = tokenStart;
	while (*tokenEnd != '\0' && strchr(delim, *tokenEnd) == NULL) tokenEnd++; //find the end of the current token

	if (*tokenEnd == '\0') nextToken = NULL; //if reached the end of the string, update nextToken
	else {
		*tokenEnd = '\0'; //terminate the curr token
		nextToken = tokenEnd + 1; //update nextToken to point to the next character
	}

	return tokenStart; //return start of substring
}

String getPatientCode(FILE* infile)
{
	String patientCode = { "\0" };
	
	if (infile == NULL) {
		fprintf(stderr, "Error: Invalid file pointer.\n");
		return patientCode;
	}
	
	char* token, tp;
	char line[100];

	fgets(line, 100, infile);
	
	tp = strtok(line, ","); //skip the first 
	token = strtok(NULL, ",");
	
	strcpy(patientCode.str, token);

	fgets(line, 100, infile); //consume empty header line

	return patientCode;
}

FitbitData readLine(char lineSrc[100], char patientID[10])
{
    char line[100];
    strcpy(line, lineSrc);

    char* token;
    char* delim = ",";
    char code[25] = {'\0'}, mins[25] = { '\0' }, cals[35] = { '\0' }, dist[35] = { '\0' }, floors[25] = { '\0' }, bpms[25] = { '\0' }, steps[25] = { '\0' }, sleepLvl[25] = { '\0' };

    FitbitData record = {
        .patient = "", .minute = "", .calories = 0.0, .distance = 0.0, .floors = 0, .heartRate = 0, .steps = 0, .sleepLevel = 0};

    /*=========CODE=========*/
    token = mystrtok(line, delim);
    if (token != NULL && *token != '\0')
        strcpy(record.patient, token);
    else
    {
        printf("no patient ID detected.\nline:\"%s\"", line);
        return record;
    }

    /*=========MINS=========*/
    token = mystrtok(NULL, delim);
    if (token != NULL && *token != '\0')
        strcpy(record.minute, token);

    // if patient is different return empty record w/ only patientID and time
    if (strcmp(record.patient, patientID) != 0)
        return record;

    /*=========CALS=========*/
    token = mystrtok(NULL, delim);
    if (token != NULL && *token != '\0')
    {
        strcpy(cals, token);
        record.calories = atof(cals);
    }
    else
        record.calories = -1;

    /*=========DIST=========*/
    token = mystrtok(NULL, delim);
    if (token != NULL && *token != '\0')
    {
        strcpy(dist, token);
        record.distance = atof(dist);
    }
    else
        record.distance = -1;

    /*========FLOORS=========*/
    token = mystrtok(NULL, delim);
    if (token != NULL && *token != '\0')
    {
        strcpy(floors, token);
        record.floors = (unsigned int)atoi(floors);
    } /*not bothering with else since i cant do -1 since they are uint*/

    /*=========BPMS=========*/
    token = mystrtok(NULL, delim);
    if (token != NULL && *token != '\0')
    {
        strcpy(bpms, token);
        record.heartRate = (unsigned int)atoi(bpms);
    }
    /*=========STEPS=========*/
    token = mystrtok(NULL, delim);
    if (token != NULL && *token != '\0')
    {
        strcpy(steps, token);
        record.steps = (unsigned int)atoi(steps);
    }
    /*=========SLEEP=========*/
    token = mystrtok(NULL, delim);
    if (token != NULL && *token != '\0')
        strcpy(sleepLvl, token);

    record.sleepLevel = NONE;
    if (sleepLvl[0] == '1') record.sleepLevel = ASLEEP;
    if (sleepLvl[0] == '2') record.sleepLevel = AWAKE;
    if (sleepLvl[0] == '3') record.sleepLevel = REALLYAWAKE;

    //for (int i = 0; i < 25 || sleepLvl[i] != '\0'; ++i) if (sleepLvl[i] == '\n') sleepLvl[i] = ' ';
    //printf("After: \"%s\"  \n", sleepLvl);    
    //printf("\"%s\" = %d\n", sleepLvl, record.sleepLevel);

    return record;
}

int timeToIndex(char time[10]) {
    char temp[10];
    strcpy(temp, time);
    
    char* token = strtok(temp, ":");
    int hour = atoi(token); 
    token = strtok(NULL, ":"); 
    int minutes = atoi(token);
    
    return (hour * 60) + minutes; 
}

void readAndSortCSV(FILE *infile, FitbitData data[1440], char patientID[10])
{
    char line[100];
    int init[1440] = {0};
    //int i = 0;

    while (fgets(line, 100, infile) != NULL)
    {

        FitbitData record = readLine(line, patientID);
        //printf("line %d: %s", i++, line);
        //printf("      {%s,%s,%lf,%.2f,%u,%u,%u,%d}\n\n", record.patient, record.minute, record.calories, record.distance, record.floors, record.heartRate, record.steps, record.sleepLevel);
        if (record.patient[0] == '\0') continue;
 
        int index = timeToIndex(record.minute);

        if (!init[index]) //if not initialized at index
		{
			data[index] = record;
            printf("{%s,%s,%lf,%.2f,%u,%u,%u,%d}\n\n", data[index].patient, data[index].minute, data[index].calories, data[index].distance, data[index].floors, data[index].heartRate, data[index].steps, data[index].sleepLevel);
			init[index] = 1;
        } else if (strcmp(data[index].patient, patientID) != 0) data[index] = record;  //overwrite with target patient
        else continue; //else skip the line cause its a duplicate
        
    }
}
