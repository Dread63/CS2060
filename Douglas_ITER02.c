#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <float.h>
#include <string.h>

#define STRING_LENGTH 80
#define SURVEY_RIDER_ROWS 5
#define SURVEY_CATEGORIES 3
#define MIN_DOLLARS 0.1
#define MAX_DOLLARS 50.0
#define MIN_RAND_MINUTES_FACTOR 1.2
#define MAX_RAND_MINUTES_FACTOR 1.5

// LOGIN AND SENTINEL VALUES
#define CORRECT_ID "id1"
#define CORRECT_PASSCODE "ABCD"
#define LOGIN_MAX_ATTEMPTS 3
#define SENTINAL_NEG1 -1

const char *surveyCategories[SURVEY_CATEGORIES] = {"Safety", "Cleanliness", "Comfort"};

struct RideShare;

bool scanDouble(const char *buffer, double *output);
bool loginAdmin (const char *correctID, const char *correctPassword, unsigned int maxAttempts);
void fgetsRemoveNewLine (char *str, unsigned int maxSize);
void setupRideShare(struct RideShare *rideSharePtr, const double min, const double max);
void displayRideShare(const struct RideShare *rideSharePtr);
void displayRideShareRatings(size_t surveyCount, const unsigned int surveys[SURVEY_RIDER_ROWS][SURVEY_CATEGORIES]);

struct RideShare {

    double baseFare;
    double costPerMinute;
    double costPerMile;
    double minFlatRate;
    double totalMiles;
    char companyName[STRING_LENGTH];
    unsigned int rideShareRatings[SURVEY_RIDER_ROWS][SURVEY_CATEGORIES];
};

int main(void) {

    struct RideShare rideShare;
    unsigned int riderCount = 1;

    bool validLogin = loginAdmin(CORRECT_ID, CORRECT_PASSCODE, LOGIN_MAX_ATTEMPTS);
    bool endRideShare = false;

    // ADMIN MODE
    if (validLogin == true) {

        setupRideShare(&rideShare, MIN_DOLLARS, MAX_DOLLARS);
        displayRideShare(&rideShare);

        puts("Exiting Admin Mode\n");

        printf("%s%s%s\n", "Welcome to ", rideShare.companyName, " We can only provide services for rides from 1 to 100 miles");

        displayRideShareRatings(riderCount, rideShare.rideShareRatings);
    }

    else {
        puts("Exiting Rideshare\n");
    }
}

bool scanDouble (const char *buffer, double *output) {

    char *end;
    errno = 0;
    double testNum = 0;
    bool isValid = false;

    testNum = strtod(buffer, &end);

    if (buffer != end && *end == '\0' && errno != ERANGE) {

        *output = testNum;
        isValid = true;
    }

    else if (errno == ERANGE) {
        puts("Invalid Input: VALUE IS OUT OF RANGE OF A DOUBLE\n");
    }

    else {
        puts("Invlaid Input\n");
    }

    return isValid;
}

bool loginAdmin (const char *correctID, const char *correctPassword, unsigned int maxAttempts) {

    puts("ADMIN LOGIN\n");
    puts("-----------\n");

    unsigned int attempts = 1;
    bool validLogin = false;
    char enteredID[STRING_LENGTH];
    char enteredPass[STRING_LENGTH];

    while (attempts <= maxAttempts && !validLogin) {

        puts("\nEnter user login ID followed by the enter key");

        fgetsRemoveNewLine(enteredID, 80);

        puts("\nEnter your passcode followed by the enter key");

        fgetsRemoveNewLine(enteredPass, 80);

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

void fgetsRemoveNewLine (char *str, unsigned int maxSize) {

    fgets(str, maxSize, stdin);

    size_t strLength = strlen(str);

    if (strLength > 0 && str[strLength - 1] == '\n') {

        str[strLength - 1] = '\0';
    }
}

double getValidDouble(const double min, const double max) {

    char buffer[STRING_LENGTH];

    double tempValue;

    fgetsRemoveNewLine(buffer, sizeof(buffer));

    while (!scanDouble(buffer, &tempValue));

    while (tempValue < min || tempValue > max) {
        puts("Value is out of range please try again");

        fgetsRemoveNewLine(buffer, sizeof(buffer));

        while (!scanDouble(buffer, &tempValue));
    }

    return tempValue;
}

void setupRideShare(struct RideShare *rideSharePtr, const double min, const double max) {

    puts("Input base fare");
    rideSharePtr->baseFare = getValidDouble(min, max);

    puts("Input cost per minute");
    rideSharePtr->costPerMinute = getValidDouble(min, max);

    puts("Input cost per mile");
    rideSharePtr->costPerMile = getValidDouble(min, max);
    
    puts("Input minimum flat rate");
    rideSharePtr->minFlatRate = getValidDouble(min, max);

    rideSharePtr->totalMiles = 0.0;

    puts("Input company name");
    fgetsRemoveNewLine(rideSharePtr->companyName, sizeof(rideSharePtr->companyName));
}

void displayRideShare(const struct RideShare *rideSharePtr) {

    printf("%s\n", rideSharePtr->companyName);
    puts("---------------------------------\n");

    printf("%s%f\n", "baseFare = $", rideSharePtr->baseFare);

    printf("%s%f\n", "costPerMinute = $", rideSharePtr->costPerMinute);

    printf("%s%f\n", "costPerMile = $", rideSharePtr->costPerMile);

    printf("%s%f\n", "minFlatRate = $", rideSharePtr->minFlatRate);
}

void displayRideShareRatings(size_t surveyCount, const unsigned int surveys[SURVEY_RIDER_ROWS][SURVEY_CATEGORIES]) {

    if (surveyCount == 0) {
        puts("There are no ratings\n");
    }

    else {

        for (size_t i = 0; i < SURVEY_CATEGORIES; i++) {
            printf("%-10s", surveyCategories[i]);
        }

        puts("\n");

        for (size_t i = 0; i < surveyCount; i++) {
            printf("%s%d%s", "Survey ", i + 1, ":");
            
            for (size_t z = 0; z < SURVEY_CATEGORIES; z++) {
                printf("%-10d", surveys[i][z]);
            }

            puts("\n");
        }
    }
}



