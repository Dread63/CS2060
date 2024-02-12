#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double getValidDouble(int min, int max, int sentinel);
int estimatedTimeOfArrival(double miles);
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes);
void printFare(int count, double miles, int minutes, double fare);

int main(void) {

	#define MIN_RAND_MINUTES_FACTOR 1.2
	#define MAX_RAND_MINUTES_FACTOR 1.5
	#define SENTINEL_VALUE -1

	int minMiles = 1;
	int maxMiles = 100;
	int sentinel = -1;

	double baseFare = 1.8;
	double costPerMinute = 0.25;
	double costPerMile = 1.2;
	double minFlatRate = 20.0;
	
	double userMiles = getValidDouble(minMiles, maxMiles, sentinel);
	int riderCount = 0;
	double totalMiles = 0.0;
	int totalMinutes = 0;
	double totalRevenue = 0.0;

	while (userMiles != sentinel) {

		riderCount = riderCount + 1;
		int eta = estimatedTimeOfArrival(userMiles);
		double calculatedFare = calculateFare(baseFare, costPerMinute, costPerMile, minFlatRate, userMiles, eta);
		
		printFare(riderCount, userMiles, eta, calculatedFare);

		totalMiles = totalMiles + userMiles;
		totalMinutes = totalMinutes + eta;
		totalRevenue = totalRevenue + calculatedFare;

		userMiles = getValidDouble(minMiles, maxMiles, sentinel);
	}

	printf("%s\n", "UCCS Ride Share Business Summary");
	printFare(riderCount, totalMiles, totalMinutes, totalRevenue);
	
}

double getValidDouble(int min, int max, int sentinel) {

	double inputtedMiles = 0.0;
	double validDouble = 0.0;

	printf("%s", "Welcome to UCCS rideshare, we provide service for rides from 1 to 100 miles. Enter the number of miles to your destination: ");
	int scanfReturn = scanf("%lf", &inputtedMiles);

	if (inputtedMiles != sentinel) {

		while (inputtedMiles < min || inputtedMiles > max || scanfReturn != 1) {

			if (inputtedMiles == 0) {
				printf("%s\n%s", "You did not enter a number", "Enter the number of miles to your destination: ");
			}
			else {
				printf("%s\n%s", "Not within 1 and 100 miles", "Enter the number of miles to your destination: ");
			}

			while ((getchar()) != '\n');
			inputtedMiles = 0.0;

			scanfReturn = scanf("%lf", &inputtedMiles);
		}
	}
	
	if (inputtedMiles == sentinel) {
		validDouble = sentinel;
	}

	else {
		validDouble = inputtedMiles;
	}

	return validDouble;
}

int estimatedTimeOfArrival(double miles) {

	double maxMinutes = miles * MAX_RAND_MINUTES_FACTOR;
	double minMinutes = miles * MIN_RAND_MINUTES_FACTOR;
	int randomMinutes = 0;

	if (maxMinutes > minMinutes) {

		srand(time(0));
		randomMinutes = (rand() % (int)(maxMinutes - minMinutes + 1)) + minMinutes;
	}

	else {
		printf("%s", "ERROR: MIN MINUTES CANNOT BE GREATER THAN MAX MINUTES");
	}
	
	return randomMinutes;
}

double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes) {

	double totalFare = 0.0;

	double calcuatedFare = base + (minuteCost * minutes) + (mileCost * miles);

	if (calcuatedFare < minRate) {
		totalFare = minRate;
	}

	else {
		totalFare = calcuatedFare;
	}

	return totalFare;
}

void printFare(int count, double miles, int minutes, double fare) {

	if (count == 0) {
		printf("\n%s\n", "There were no rides");
	}

	else {
		printf("\n%s\n", "Current Ride Information");
		printf("%-10s%-20s%-20s%-20s\n", "Rider", "Number of Miles", "Number of Minutes", "Ride Fare Amount");
		printf("%-10d%-20f%-20d%-20f\n", count, miles, minutes, fare);
	}
	
}