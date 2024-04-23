#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <float.h>
#include <string.h>
#include <ctype.h>

#define STRING_LENGTH 80
#define SURVEY_RIDER_ROWS 5
#define SURVEY_CATEGORIES 3
#define MIN_DOLLARS 0.1
#define MAX_DOLLARS 50.0
#define MIN_RAND_MINUTES_FACTOR 1.2
#define MAX_RAND_MINUTES_FACTOR 1.5
#define MIN_MILES 1.0
#define MAX_MILES 100.0
#define MAX_SURVEY_RATING 5
#define MIN_SURVEY_RATING 1

// LOGIN AND SENTINEL VALUES
#define CORRECT_ID "id1"
#define CORRECT_PASSCODE "ABCD"
#define LOGIN_MAX_ATTEMPTS 3
#define SENTINAL_NEG1 -1

const char* surveyCategories[SURVEY_CATEGORIES] = { "Safety", "Cleanliness", "Comfort" };

struct RideShare;

typedef struct rideShare {

    double baseFare;
    double costPerMinute;
    double costPerMile;
    double minFlatRate;
    double totalMiles;
    char companyName[STRING_LENGTH];
    unsigned int rideShareRatings[SURVEY_RIDER_ROWS][SURVEY_CATEGORIES];
    unsigned int surveyCount;
    unsigned int riderCount;
    unsigned int totalMinutes;
    double totalFares;
    double categoryAverages[SURVEY_CATEGORIES];

    struct RideShare* nextPtr;

} RideShare;

void setupRideShare(RideShare* rideSharePtr, const double min, const double max);
void insertRideShare(RideShare** headPtr);
bool loginAdmin(const char* correctID, const char* correctPassword, unsigned int maxAttempts);
double getValidDouble(const double min, const double max);
bool scanDouble(const char* buffer, double* output);
void fgetsRemoveNewLine(char* str, unsigned int maxSize);
char getYesNo();
void printRideShares(RideShare* head);
void riderMode(RideShare* rideShare);
void displayRideShareRatings(const RideShare* head);
void displayRideShare(const RideShare* head);
void strToLowerCase(char* str);
int estimatedTimeOfArrival(double miles, double min, double max);
double calculateFare(const RideShare* rideSharePtr, unsigned int minutes, double miles);
void getRating(unsigned int survey[][SURVEY_CATEGORIES], unsigned int* surveyCount, size_t totalCategories, int min, int max);
RideShare* matchRideShares(RideShare* head, char* name);

int main(void) {

    RideShare *head = NULL; // MOVED INTO SECOND IF CONDITION?

    // Check if accurate details enerted, otherwise exit program
    bool validLogin = loginAdmin(CORRECT_ID, CORRECT_PASSCODE, LOGIN_MAX_ATTEMPTS);

    // Setup rideshare and enter rider mode
    if (validLogin == true) {

        char wantsToContinue;

        do {
            
            insertRideShare(&head);
            puts("\nWould you like to input another rideShare?\n");
            wantsToContinue = getYesNo();

        } while (wantsToContinue == 'y' || wantsToContinue == 'Y');
        

        displayRideShare(head);

        printRideShares(head);

        puts("Exiting Admin Mode\n");

        riderMode(head);

        // If admin re-logs in, display averages and day summary
        // calculateCategoryAverages(rideShare.rideShareRatings, rideShare.surveyCount, rideShare.categoryAverages);
        // displayRideShareSummary(&rideShare, rideShare.categoryAverages);
    }

    else {
        puts("Exiting Rideshare\n");
    }
}

void insertRideShare(RideShare** headPtr) {

    RideShare *newRideSharePtr = malloc(sizeof(RideShare));

    if (newRideSharePtr != NULL) {

        setupRideShare(newRideSharePtr, MIN_DOLLARS, MAX_DOLLARS);

        RideShare* previousPtr = NULL;
        RideShare* currentPtr = *headPtr;

        while (currentPtr != NULL && strcmp(currentPtr->companyName, newRideSharePtr->companyName) <= 0) {

            previousPtr = currentPtr;
            currentPtr = currentPtr->nextPtr;
        }
        
        if (previousPtr == NULL) {

            newRideSharePtr->nextPtr = *headPtr;
            *headPtr = newRideSharePtr;
        }

        else {

            previousPtr->nextPtr = newRideSharePtr;
            newRideSharePtr->nextPtr = currentPtr;
        }
    }
}

// Initializing data in ride share structure
void setupRideShare(RideShare* rideSharePtr, const double min, const double max) {

    puts("Input base fare");
    rideSharePtr->baseFare = getValidDouble(min, max);

    puts("\nInput cost per minute");
    rideSharePtr->costPerMinute = getValidDouble(min, max);

    puts("\nInput cost per mile");
    rideSharePtr->costPerMile = getValidDouble(min, max);

    puts("\nInput minimum flat rate");
    rideSharePtr->minFlatRate = getValidDouble(min, max);

    rideSharePtr->totalMiles = 0.0;

    rideSharePtr->surveyCount = 0;

    // Set these to zero so garbage values aren't printed in displayRideShareSummary()
    rideSharePtr->totalFares = 0.0;
    rideSharePtr->totalMiles = 0.0;
    rideSharePtr->totalMinutes = 0;
    rideSharePtr->riderCount = 0;

    puts("\nInput company name");
    fgetsRemoveNewLine(rideSharePtr->companyName, sizeof(rideSharePtr->companyName));
}

// Compared entered ID and Pass to defined credentials
bool loginAdmin(const char* correctID, const char* correctPassword, unsigned int maxAttempts) {

    puts("ADMIN LOGIN");
    puts("-----------");

    unsigned int attempts = 1;
    bool validLogin = false;
    char enteredID[STRING_LENGTH];
    char enteredPass[STRING_LENGTH];

    // While the user hasn't run out of login attempts and the valid login hasn't been entered
    while (attempts <= maxAttempts && !validLogin) {

        puts("\nEnter user login ID followed by the enter key");

        fgetsRemoveNewLine(enteredID, sizeof(enteredID));
       
        puts("\nEnter your passcode followed by the enter key");

        fgetsRemoveNewLine(enteredPass, sizeof(enteredID));

        puts("");

        // Compare entered ID to correct ID using strcmp()
        if (strcmp(enteredID, correctID) == 0) {

            if (strcmp(enteredPass, correctPassword) == 0) {
                validLogin = true;
            }

            else {
                puts("\nIncorrect Password, Please Try Again");
            }
        }

        else {
            puts("Incorrect UserID, Please Try Again");
        }

        attempts++;
    }

    return validLogin;
}

// Ensure a double value is within a given min and max range
double getValidDouble(const double min, const double max) {

    char buffer[STRING_LENGTH];

    double tempValue;

    fgetsRemoveNewLine(buffer, sizeof(buffer));

    // Check if input is a valid double
    bool isValidDouble = scanDouble(buffer, &tempValue);
    while (isValidDouble == false) {

        fgetsRemoveNewLine(buffer, sizeof(buffer));
        isValidDouble = scanDouble(buffer, &tempValue);
    }

    // As long as sentinel value isn't enetered, check if within min and max bounds
    if (tempValue != SENTINAL_NEG1) {

        // As long as value is out of range, re-promtp and update value
        while (!(tempValue >= min && tempValue <= max)) {

            puts("Value is out of range please try again");

            fgetsRemoveNewLine(buffer, sizeof(buffer));

            while (!scanDouble(buffer, &tempValue));
        }
    }

    return tempValue;
}

// Ensure entered data follows rules of a double
bool scanDouble(const char* buffer, double* output) {

    char* end;
    errno = 0;
    double testNum = 0;
    bool isValid = false;

    testNum = strtod(buffer, &end);

    // If string is not empty and last index is new null character and value is within range of a double, push output and set isValid to true
    if (buffer != end && *end == '\0' && errno != ERANGE) {

        *output = testNum;
        isValid = true;
    }

    else if (errno == ERANGE) {
        puts("Invalid Input: VALUE IS OUT OF RANGE OF A DOUBLE\n");
    }

    else {
        puts("Invlaid Input, Try Again\n");
    }

    return isValid;
}

// Replaces scanf, collects input with fgets and removes the new line character
void fgetsRemoveNewLine(char* str, unsigned int maxSize) {

    // Output string passed with sizeof(str) for maxSize and using standard in (keyboard) to collect input
    fgets(str, maxSize, stdin);

    size_t strLength = strlen(str);

    // Replace last new line character with null character
    if (strLength > 0 && str[strLength - 1] == '\n') {

        str[strLength - 1] = '\0';
    }
}

// Collect and verify input for a yes/no response to a question
char getYesNo() {

    char buffer[STRING_LENGTH];

    fgetsRemoveNewLine(buffer, sizeof(buffer));

    // If enetered string is not a y or n, re-prompt and update data
    while (strcmp(buffer, "y") != 0 && strcmp(buffer, "n") != 0 &&
        strcmp(buffer, "Y") != 0 && strcmp(buffer, "N") != 0) {

        puts("Invlaid input try again");
        fgetsRemoveNewLine(buffer, sizeof(buffer));
    }

    return buffer[0];
}

void printRideShares(RideShare* head) {
    RideShare* current = head;

    while (current != NULL) {
        printf("Company Name: %s\n", current->companyName);
        current = current->nextPtr;
    }
}

RideShare* matchRideShares(RideShare* head, char* name) {

    RideShare* currentPtr = head;
    RideShare* correctPtr = NULL;

    while (currentPtr != NULL) {

        char *lowercaseRideshareName = currentPtr->companyName;
        strToLowerCase(lowercaseRideshareName);
        strToLowerCase(name);

        if (strcmp(lowercaseRideshareName, name) == 0) {

            // Since the input matches the name, return the current rideshare
            correctPtr = currentPtr;
        }

        // Move to next rideshare in linked list
        currentPtr = currentPtr->nextPtr;
    }

    return correctPtr; // Rideshare name not found
}

// Simulates the rider experience of the given rideshare
void riderMode(RideShare* rideShare) {

    bool loginStatus = false;


    do {

        displayRideShareRatings(rideShare);
        printf("%s\n", "Please input the name of the rideshare organization you would like to ride in");

        char inputtedOrgName[STRING_LENGTH];
        fgetsRemoveNewLine(inputtedOrgName, sizeof(inputtedOrgName));

        RideShare* selectedRideShare = matchRideShares(rideShare, inputtedOrgName);

        while (selectedRideShare == NULL) {

            printf("Name does not match any ride shares, please retry\n");
            fgetsRemoveNewLine(inputtedOrgName, sizeof(inputtedOrgName));
            selectedRideShare = matchRideShares(rideShare, inputtedOrgName);
        }


        printf("\n%s%s%s\n", "Welcome to ", selectedRideShare->companyName, " We can only provide services for rides from 1 to 100 miles");
        puts("\n");
        

        puts("Do you want to request a ride share? Input y/Y or n/N");
        char wantsToRide = getYesNo();

        if (wantsToRide == 'y' || wantsToRide == 'Y') {

            puts("Input the amount of miles your trip requires");

            double miles = getValidDouble(MIN_MILES, MAX_MILES);

            if (miles == SENTINAL_NEG1) {

                loginStatus = loginAdmin(CORRECT_ID, CORRECT_PASSCODE, LOGIN_MAX_ATTEMPTS);
            }

            else {

                unsigned int minutes = estimatedTimeOfArrival(miles, MIN_RAND_MINUTES_FACTOR, MAX_RAND_MINUTES_FACTOR);
                printf("%s%d%s\n", "Estimated Time of Arrival: ", minutes, " Minutes");

                double fare = calculateFare(rideShare, minutes, miles);
                printf("%s%.2f\n", "Total Fare: ", fare);

                puts("Would you like to rate your rideshare experience? Input y/Y or n/N");
                char wantsToRate = getYesNo();

                if (wantsToRate == 'y' || wantsToRate == 'Y') {

                    getRating(rideShare->rideShareRatings, &rideShare->surveyCount, SURVEY_CATEGORIES, MIN_SURVEY_RATING, MAX_SURVEY_RATING);
                }

                else {
                    puts("Thanks for riding with us!");
                }

                // Add current riders values to total values
                rideShare->totalMinutes += minutes;
                rideShare->totalFares += fare;
                rideShare->totalMiles += miles;
                rideShare->riderCount++;
            }
        }

        else {
            puts("Hope to see you again!\n\n");
        }
    }

    while (loginStatus == false);
}

void displayRideShareRatings(const RideShare *head) {


    RideShare* currentPtr = head;

    while (currentPtr != NULL) {

        printf("%s%s%s\n\n", "RideShare Organization ", currentPtr->companyName, " Ratings");

        // Check if no surveys have been entered
        if (currentPtr->surveyCount == 0) {
            puts("There are no ratings\n");
        }

        else {

            // Print category titles
            printf("%10s", "Rating Categories:");
            for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {
                printf("%17s", surveyCategories[i]);
            }

            puts("\n");

            for (size_t i = 0; i < currentPtr->surveyCount; i++) {
                printf("%s%zu%s", "Survey ", i + 1, ":");

                for (size_t z = 0; z < SURVEY_CATEGORIES; z++) {
                    printf("%20d", currentPtr->rideShareRatings[i][z]);
                }

                puts("\n");
            }

        }

        currentPtr = currentPtr->nextPtr;
   
    }
}

void strToLowerCase(char* str) {

    size_t currentChar = 0;

    while (str[currentChar] != '\0') {

        char replacementCharacter = tolower(str[currentChar]);
        str[currentChar] = replacementCharacter;
        currentChar++;

    }
}

// Displayed summary of enetered values by admin
void displayRideShare(const RideShare* head) {

    RideShare* currentRideShare = head;

    while (currentRideShare != NULL) {
       
        printf("\n%s\n", currentRideShare->companyName);
        puts("---------------------------------\n");

        printf("%s%.2f\n", "baseFare = $", currentRideShare->baseFare);

        printf("%s%.2f\n", "costPerMinute = $", currentRideShare->costPerMinute);

        printf("%s%.2f\n", "costPerMile = $", currentRideShare->costPerMile);

        printf("%s%.2f\n", "minFlatRate = $", currentRideShare->minFlatRate);

        currentRideShare = currentRideShare->nextPtr;
    }
}

// Calculate estimated travel time based off miles and randomly generated scalar
int estimatedTimeOfArrival(double miles, double min, double max) {

    double maxMinutes = miles * max;
    double minMinutes = miles * min;
    int randomMinutes = 0;

    // Ensure that maxMinutes is larger than minMinutes
    if (maxMinutes > minMinutes) {

        // Seed rand() function to calculate random ETA within given range
        // Cast time_t to unsigned int to fix clang-tidy warning
        srand((unsigned int)time(0));
        randomMinutes = (int)(rand() % (int)(maxMinutes - minMinutes + 1)) + minMinutes;
    }

    else {
        printf("%s", "ERROR: MIN MINUTES CANNOT BE GREATER THAN MAX MINUTES");
    }

    return randomMinutes;
}

// Plug rideshare details into formula to calculate total fare
double calculateFare(const RideShare* rideSharePtr, unsigned int minutes, double miles) {

    double totalFare = 0.0;

    double calcuatedFare = rideSharePtr->baseFare + (rideSharePtr->costPerMinute * minutes) + (rideSharePtr->costPerMile * miles);

    // Ensuring the base rate is always charged regardless of length of trip
    if (calcuatedFare < rideSharePtr->minFlatRate) {
        totalFare = rideSharePtr->minFlatRate;
    }

    else {
        totalFare = calcuatedFare;
    }

    return totalFare;
}

void getRating(unsigned int survey[][SURVEY_CATEGORIES], unsigned int* surveyCount, size_t totalCategories, int min, int max) {

    for (size_t categories = 0; categories < totalCategories; categories++) {

        printf("%s%s%s\n", "Please input a rating between 1-5 for ", surveyCategories[categories], ": ");
        survey[*surveyCount][categories] = (int)getValidDouble(min, max);
    }

    *surveyCount = *surveyCount + 1;
}

void displayRideShareSummary(const RideShare* rideSharePtr, double categoryAverages[SURVEY_CATEGORIES]) {

    printf("%s%s\n", rideSharePtr->companyName, " Summary Report:");

    printf("%-10s%-20s%-20s%-20s\n", "Riders", "Number of Miles", "Number of Mintes", "Ride Fare Amount");
    printf("%-10d%-20.2f%-20u%-20.2f\n", rideSharePtr->riderCount, rideSharePtr->totalMiles, rideSharePtr->totalMinutes, rideSharePtr->totalFares);

    printf("\n%s", "Category Rating Averages:\n\n");

    for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {
        printf("%-20s", surveyCategories[i]);
    }

    puts("");

    for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {
        printf("%-20.2f", categoryAverages[i]);
    }
}

void calculateCategoryAverages(const unsigned int rideshareSurvey[][SURVEY_CATEGORIES], unsigned int surveyCount, double categoryAverages[SURVEY_CATEGORIES]) {

    for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {
        categoryAverages[i] = 0.0;
    }

    for (size_t i = 0; i < surveyCount; i++) {

        for (size_t z = 0; z < SURVEY_CATEGORIES; z++) {

            categoryAverages[z] += rideshareSurvey[i][z];
        }
    }

    for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {

        categoryAverages[i] /= surveyCount;
    }

}