#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <float.h>
#include <string.h>
#define STRLENGTH 20
#define NUMMEMBERS 3
#define ZONE1PERCENT 0.6
#define ZONE2PERCENT 0.7
#define ZONE3PERCENT 0.8
#define FILEPATH "c:/Dev/CS2060/MemData/"
#define GYMFILE "gym.txt"


typedef struct MemberStruct {

	char name[STRLENGTH];
	unsigned int age;
	float zone;
	double heartRate;

} Member;

void sortGymMembersByAge(Member gymMembers[], size_t numMembers);
void displayMembers(const Member gymMembers[], size_t numMembers);
void returnFilePath(char* rootPath, char* fileName, char* filePath);
void writeToFile(Member gymMembers[], char filePath[], size_t numMembers);
void updateHeartRate(Member gymMembers[], size_t numMembers);
void printFile(char filePath[], size_t numMembers);

int main(void) {

	Member staticMem = { "Static", 45, 3, 0 };
	Member dynamicMem = { "Dynamic", 23, 1, 0 };
	Member queueMem = { "Queue", 19, 2, 0 };

	Member gymMembers[] = { staticMem, dynamicMem, queueMem };

	sortGymMembersByAge(gymMembers, NUMMEMBERS);
	displayMembers(gymMembers, NUMMEMBERS);
	updateHeartRate(gymMembers, NUMMEMBERS);

	char rootFilePath[] = FILEPATH;
	char gymFile[] = GYMFILE;
	
	char filePath[80] = "";

	returnFilePath(rootFilePath, gymFile, filePath);

	printf("Final File Path: %s\n", filePath);

	writeToFile(gymMembers, filePath, NUMMEMBERS);

	printFile(filePath, NUMMEMBERS);
}

void sortGymMembersByAge(Member gymMembers[], size_t numMembers) {
	
	for (int i = 0; i < numMembers; i++) {

		for (int z = 0; z < numMembers; z++) {

			if (gymMembers[z].age > gymMembers[z + 1].age) {

				Member tempMem = gymMembers[z];
				gymMembers[z] = gymMembers[z + 1];
				gymMembers[z + 1] = tempMem;
			}
		}
	}
}

void displayMembers(const Member gymMembers[], size_t numMembers) {

	for (int i = 0; i < numMembers; i++) {

		printf("%-10s%-10d%-10.1f", gymMembers[i].name, gymMembers[i].age, gymMembers[i].heartRate);
	}
}

void updateHeartRate(Member gymMembers[], size_t numMembers) {

	for (int i = 0; i < numMembers; i++) {

		double maxHeartRate = 220 - gymMembers[i].age;
		int currentZone = gymMembers[i].zone;
		double workoutHeartRate = 0.0;

		switch (currentZone) {

			case 1:
				workoutHeartRate = maxHeartRate * ZONE1PERCENT;
				gymMembers[i].heartRate = workoutHeartRate;
				break;

			case 2:
				workoutHeartRate = maxHeartRate * ZONE2PERCENT;
				gymMembers[i].heartRate = workoutHeartRate;
				break;

			case 3:
				workoutHeartRate = maxHeartRate * ZONE3PERCENT;
				gymMembers[i].heartRate = workoutHeartRate;
				break;

			default:
				puts("Invalid workout zone\n");
				break;
					
		}
	}
}

void returnFilePath(char *rootPath, char *fileName, char *filePath) {

	strcpy(filePath, rootPath);
	strcat(filePath, fileName);

}

void writeToFile(Member gymMembers[], char filePath[], size_t numMembers) {

	FILE* filePtr;

	filePtr = fopen(filePath, "w");

	for (int i = 0; i < numMembers; i++) {

		fprintf(filePtr, "%-10s%-10d%-10.2f\n", gymMembers[i].name, gymMembers[i].age, gymMembers[i].heartRate);
	}

	fclose(filePtr);
}

void printFile(char filePath[], size_t numMembers) {

	FILE* filePtr;

	filePtr = fopen(filePath, "r");
	char currentLine[80] = "";

	for (int i = 0; i < numMembers; i++) {

		fgets(currentLine, 80, filePtr);
		printf("%s\n", currentLine);
	}
}