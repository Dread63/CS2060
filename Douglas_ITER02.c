#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <float.h>
#include <string.h>

#define INVALID_INPUT 1
#define STRING_LENGTH 80
#define SURVEY_RIDER_ROWS 5
#define SURVEY_CATEGORIES 3

// LOGIN AND SENTINEL VALUES
#define CORRECT_ID "id1"
#define CORRECT_PASSCODE "ABCD"
#define LOGIN_MAX_ATTEMPTS 3
#define SENTINAL_NEG1 -1

const char *surveyCategories[SURVEY_CATEGORIES] = {"Safety", "Cleanliness", "Comfort"};

bool scanDouble(const char *buffer, double *output);
bool loginAdmin (const char *correctID, const char *correctPassword, unsigned int maxAttempts);
void fgetsRemoveNewLine (char *str, unsigned int maxSize);
void setupRideShare(struct RideShare *rideSharePtr, unsigned int min, unsigned int max);

int main(void) {

    struct RideShare rideShare;

    setupRideShare(&rideShare, 1, 100);

    /*
    bool validLogin = loginAdmin(&CORRECT_ID, &CORRECT_PASSCODE, LOGIN_MAX_ATTEMPTS);

    if (validLogin == true) {
        puts("Successful Login");
    }

    if (validLogin == false) {
        puts("Failed Login");
    }
    */
}

struct RideShare {

    unsigned int minMiles;
    unsigned int maxMiles;
    double baseFare;
    double costPerMinute;
	double costPerMile;
	double minFlatRate;
    double totalMiles;
    char companyName[STRING_LENGTH];
};

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

    int strLength = strlen(str);

    if (strLength > 0 && str[strLength - 1] == '\n') {

        str[strLength - 1] = '\0';
    }
}

void setupRideShare(struct RideShare *rideSharePtr, unsigned int min, unsigned int max) {

    char buffer[STRING_LENGTH];

    puts("Input base fare");
    rideSharePtr -> baseFare = scanDouble(min, max);

    puts("Input cost per minte");
    rideSharePtr -> costPerMinute = scanDouble(min, max);

    puts("Input cost per mile");
    rideSharePtr -> costPerMile = scanDouble(min, max);

    puts("Input minimum flat rate");
    rideSharePtr -> minFlatRate = scanDouble(min, max);

    rideSharePtr -> totalMiles = 0.0;

    fgetsRemoveNewLine(rideSharePtr -> companyName, max);
}
