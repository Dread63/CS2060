#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>
#include <float.h>

#define INVALID_INPUT 1;

bool scanDouble(const char *buffer, double *output);

int main(void) {

    char testString[80] = "3.14159";

    double input = 0.0;

    bool validScan = scanDouble(testString, &input);

    printf("%f", input);
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