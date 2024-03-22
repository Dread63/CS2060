#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Defining function prototypes so main can access functions created after it
double getValidDouble(int min, int max, int sentinel);
int estimatedTimeOfArrival(double miles);
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes);
void printFare(int count, double miles, int minutes, double fare);

int main(void) {

	// Initializing variables to avoid garbage data and create flexible code
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
	
	double riderMiles = getValidDouble(minMiles, maxMiles, sentinel); // Collecting inputted miles from rider

	// Counters to be shown to the business owner
	int riderCount = 0;
	double totalMiles = 0.0;
	int totalMinutes = 0;
	double totalRevenue = 0.0;

	while (riderMiles != sentinel) {

		riderCount = riderCount + 1; // If the loop was enetered, then one rider has traveled successfully
		int eta = estimatedTimeOfArrival(riderMiles);
		double calculatedFare = calculateFare(baseFare, costPerMinute, costPerMile, minFlatRate, riderMiles, eta);
		
		printFare(riderCount, riderMiles, eta, calculatedFare);

		// Updating total information for business owner
		totalMiles = totalMiles + riderMiles;
		totalMinutes = totalMinutes + eta;
		totalRevenue = totalRevenue + calculatedFare;

		riderMiles = getValidDouble(minMiles, maxMiles, sentinel);
	}

	// When the sentinel value is entered, display totals for the day
	printf("%s\n", "UCCS Ride Share Business Summary");
	printFare(riderCount, totalMiles, totalMinutes, totalRevenue);
}

// Collecting valid inputted miles given a range
double getValidDouble(int min, int max, int sentinel) {

	double inputtedMiles = 0.0;
	double validDouble = 0.0;

	printf("%s", "Welcome to UCCS rideshare, we provide service for rides from 1 to 100 miles. Enter the number of miles to your destination: ");
	// ScanfReturn is used to ensure the value was successfully read and re-promt if not
	int scanfReturn = scanf("%lf", &inputtedMiles);

	// Ensure the miles are valid and not equal to the sentinel value
	if (inputtedMiles != sentinel) {

		while (inputtedMiles < min || inputtedMiles > max || scanfReturn != 1) {

			if (inputtedMiles == 0) {
				printf("%s\n%s", "You did not enter a number", "Enter the number of miles to your destination: ");
			}
			else {
				printf("%s\n%s", "Not within 1 and 100 miles", "Enter the number of miles to your destination: ");
			}

			// This line of code clears the input buffer by looping until the character in the input buffer is the new line character (empty)
			while ((getchar()) != '\n');

			// Miles is set back to zero to ensure the proper error message is displayed after running the code multiple times
			inputtedMiles = 0.0;

			scanfReturn = scanf("%lf", &inputtedMiles);
		}
	}
	
	// If the inputted miles is the sentinel value, return the sentinel value
	if (inputtedMiles == sentinel) {
		validDouble = sentinel;
	}

	// Else, send back the inputted miles
	else {
		validDouble = inputtedMiles;
	}

	return validDouble;
} 
 
// Functiont to calculate the riders estimated travel time
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

// Function to calculate the cost of the ride given variables defined in main
double calculateFare(double base, double minuteCost, double mileCost, double minRate, double miles, int minutes) {

	double totalFare = 0.0;

	double calcuatedFare = base + (minuteCost * minutes) + (mileCost * miles);

	// Ensuring the base rate is always charged regardless of length of trip
	if (calcuatedFare < minRate) {
		totalFare = minRate;
	}

	else {
		totalFare = calcuatedFare;
	}

	return totalFare;
}

// Function to print ride information
void printFare(int count, double miles, int minutes, double fare) {

	// Ensuring something is displayed if the sentinel value is entered before any rides take place
	if (count == 0) {
		printf("\n%s\n", "There were no rides");
	}

	else {
		printf("\n%s\n", "Current Ride Information");
		printf("%-10s%-20s%-20s%-20s\n", "Rider", "Number of Miles", "Number of Minutes", "Ride Fare Amount");
		printf("%-10d%-20f%-20d%-20f\n", count, miles, minutes, fare);
	}
}