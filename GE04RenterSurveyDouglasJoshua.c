#include <stdio.h>

#define NUM_RENTERS 7
#define SURVEY_CATEGORIES 3
#define MAX_RATING 5
#define MIN_RATING 1

int main(void) {

    int rideshareSurvey[NUM_RENTERS][SURVEY_CATEGORIES];

    double categoryAverages[SURVEY_CATEGORIES];

    const char *surveyCategories[SURVEY_CATEGORIES] = {"Safety", "Cleanliness", "Comfort"}; 

    int riderCount = 0;

    for (int i = 0; i < NUM_RENTERS; i++) {

        if (riderCount == 0) {
            printf("%s\n", "No ratings currently");
        }

        if (riderCount != 0) {
            printRatings(rideshareSurvey, riderCount);
        }
            printf("%s\n", "We want to know  how your experience was on your ride today.Using the rating system 1 to 5 enter your rating for each category:");
            printCategories(surveyCategories, SURVEY_CATEGORIES);
            getRatings(rideshareSurvey, riderCount);
            riderCount = riderCount + 1;
        
    }

    
    
}

void printCategories(const char *categories[], size_t totalCategories) 

{ 

    //loop to display each category horizontally 

    printf("%s", "Rating Categories:\t"); 

    for (size_t surveyCategory = 0; surveyCategory < totalCategories; ++surveyCategory) 

    { 

        printf("\t%zu.%s\t", surveyCategory+1, categories[surveyCategory]); 

    } 

    puts(""); // start new line of output 

} 

void getRatings (int rideshareSurvey[NUM_RENTERS][SURVEY_CATEGORIES], size_t riderCount) {

    for(int i = 0; i < SURVEY_CATEGORIES; i++) {
        printf("\n%s%d\n", "Enter your rating for category ", i + 1);
        scanf("%d", &rideshareSurvey[riderCount][i]);
        while ((getchar()) != '\n');
    }

}

void printRatings (int rideshareSurvey[NUM_RENTERS][SURVEY_CATEGORIES], int riderCount) {

        for (int i = 0; i < riderCount; i++) {

            printf("%s%d%s", "Survey ", i + 1, ":");
        
            for (int j = 0; j < SURVEY_CATEGORIES; j++) {

                printf("%10d", rideshareSurvey[i][j]);
            }
            puts("\n");
        }
}

void calculateCategoryAverages(int rideshareSurvey[NUM_RENTERS][SURVEY_CATEGORIES], int categoryAverages[SURVEY_CATEGORIES], int riderCount) {

    for (size_t i = 0; i < riderCount; i++) {

        double average = 0.0;

        for (size_t z = 0; z < SURVEY_CATEGORIES; z++) {

            average += rideshareSurvey[i][z];
        }

        average = average / riderCount;

        categoryAverages[i] = average;
    }
}