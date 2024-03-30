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

struct RideShare {

    double baseFare;
    double costPerMinute;
    double costPerMile;
    double minFlatRate;
    double totalMiles;
    char companyName[STRING_LENGTH];
};

int main(void) {

    struct RideShare rideShare;

    setupRideShare(&rideShare, MIN_DOLLARS, MAX_DOLLARS);

    bool validLogin = loginAdmin(CORRECT_ID, CORRECT_PASSCODE, LOGIN_MAX_ATTEMPTS);

    if (validLogin == true) {
        puts("Successful Login");
    }

    if (validLogin == false) {
        puts("Failed Login");
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
        puts("Invalid Input: VALUE IS OUT OF RANGE OF A DOUBLE");
    }

    else {
        puts("Invlaid input");
    }

    return isValid;
}

bool loginAdmin (const char *correctID, const char *correctPassword, unsigned int maxAttempts) {

    unsigned int attempts = 1;
    bool validLogin = false;
    char enteredID[STRING_LENGTH];
    char enteredPass[STRING_LENGTH];

    while (attempts <= maxAttempts && !validLogin) {

        puts("\nEnter user login ID followed by the enter key");

        fgetsRemoveNewLine(enteredID, 80);

        puts("\nEnter passcode followed by the enter key");

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

    char buffer[STRING_LENGTH];

    puts("Input base fare");
    rideSharePtr->baseFare = getValidDouble(min, max);
    //fgetsRemoveNewLine(buffer, sizeof(buffer));
    //while (!scanDouble(buffer, &rideSharePtr->baseFare));

    puts("Input cost per minute");
    //fgetsRemoveNewLine(buffer, sizeof(buffer));
    //while (!scanDouble(buffer, &rideSharePtr->costPerMinute));

    puts("Input cost per mile");
    //fgetsRemoveNewLine(buffer, sizeof(buffer));
    //while (!scanDouble(buffer, &rideSharePtr->costPerMile));
    
    puts("Input minimum flat rate");
    //fgetsRemoveNewLine(buffer, sizeof(buffer));
    //while (!scanDouble(buffer, &rideSharePtr->minFlatRate));

    rideSharePtr->totalMiles = 0.0;
}
