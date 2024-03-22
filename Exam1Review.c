#include <stdio.h>

double convertValue(double x);

int main (void) {

    // CHAPTER 2

    // Print and Receive input

    puts("Example output\n");
    
    // We always use a format specifier with printf
    printf("%s\n", "Example output");

    int receivedValue = 0;

    int successfulScan = scanf("%d", &receivedValue);

    while((getchar()) != '\n')

    if (successfulScan = 1) {
        puts("Value was read successfully");
    }

    else {
        puts("Value wasn't read correctly");
    }


    printf("%s%d", "Received Value is ", receivedValue);

    
    // CHAPTER 3

    // Implement different kinds of if statements

    int i = 1;
    int z = 5;

    if (i < z) {
        puts("First value is less than second value");
    }

    if (z < i) {
        puts("Second value is less than first value");
    }

    else if (z == i) {
         puts("Values are equal to eachother");
    }

    else {
        puts("I have no idea what values are what lol");
    }

    int testConvert = 5;
    int changedValue = convertValue(5);
    printf("%f", changedValue);
}

double convertValue(double x) {
    return x;
}