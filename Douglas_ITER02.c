#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <float.h>

#define INVALID_INPUT 1;

bool scanDouble(const char *buffer, double *output);
bool loginAdmin (const char *correctID, const char *correctPassword, unsigned int maxAttempts);
void fgetsRemoveNewLine (char *str, unsigned int maxSize);

int main(void) {

    const char correctID[80] = "Josh";
    const char correctPassword[80] = "Pizza2020";

    char testString[80] = "3.14159";

    double input = 0.0;

    bool validScan = scanDouble(testString, &input);

    printf("%f", input);

    bool validLogin = loginAdmin(correctID, correctPassword, 3);

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
    char enteredID[80];
    char enteredPass[80];

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