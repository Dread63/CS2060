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
RideShare* matchRideShares(RideShare* head, const char* name);

int main(void) {

    RideShare *head = NULL; // MOVED INTO SECOND IF CONDITION?

    // Check if accurate details enerted, otherwise exit program
    bool validLogin = loginAdmin(CORRECT_ID, CORRECT_PASSCODE, LOGIN_MAX_ATTEMPTS);

    // Setup rideshare and enter rider mode
    if (validLogin == true) {

        char wantsToContinue;

        do {
            
            insertRideShare(&head);
            puts("Would you like to input another rideShare?");
            wantsToContinue = getYesNo();

        } while (wantsToContinue == 'y' || wantsToContinue == 'Y');
        

        // displayRideShare(&rideShare);

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

RideShare* matchRideShares(RideShare* head, const char* name) {

    RideShare* currentPtr = head;
    RideShare* correctPtr = NULL;

    while (currentPtr != NULL) {

        if (strcmp(currentPtr->companyName, name) == 0) {

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


    // PRINT ALL RIDESHARES AND THEIR RATINGS
    printf("%s\n", "Please input the name of the rideshare organization you would like to ride in");

    char inputtedOrgName[STRING_LENGTH];
    fgetsRemoveNewLine(inputtedOrgName, sizeof(inputtedOrgName));

    RideShare *selectedRideShare = matchRideShares(rideShare, inputtedOrgName);
    
    while (selectedRideShare == NULL) {

        printf("Name does not match any ride shares, please retry\n");
        fgetsRemoveNewLine(inputtedOrgName, sizeof(inputtedOrgName));
        selectedRideShare = matchRideShares(rideShare, inputtedOrgName);
    }

    do {

        printf("\n%s%s%s\n", "Welcome to ", selectedRideShare->companyName, " We can only provide services for rides from 1 to 100 miles");
        puts("\n");
        displayRideShareRatings(&rideShare->surveyCount, rideShare->rideShareRatings, rideShare->companyName);

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

void displayRideShareRatings(unsigned int* surveyCount, const unsigned int surveys[SURVEY_RIDER_ROWS][SURVEY_CATEGORIES], const char companyName[STRING_LENGTH]) {

    printf("%s%s%s\n\n", "RideShare Organization ", companyName, " Ratings");


    // Check if no surveys have been entered
    if (*surveyCount == 0) {
        puts("There are no ratings\n");
    }

    else {

        // Print category titles
        printf("%10s", "Rating Categories:");
        for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {
            printf("%17s", surveyCategories[i]);
        }

        puts("\n");

        for (size_t i = 0; i < *surveyCount; i++) {
            printf("%s%zu%s", "Survey ", i + 1, ":");

            for (size_t z = 0; z < SURVEY_CATEGORIES; z++) {
                printf("%20d", surveys[i][z]);
            }

            puts("\n");
        }
    }
}

void strToLowerCase(char* str) {

    size_t currentChar = 0;

    while (str[currentChar] != '\0') {

        tolower(str[currentChar]);
        currentChar++;
    }
}