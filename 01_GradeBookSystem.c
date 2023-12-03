// Zandryn Epan
// Dr. Steinberg
// COP 3223c Section 01
// Large Program 4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// maximum number of students
#define MAX 5

// class typedef
typedef struct {
    char name[50];
    int ID;
    double homework;
    double quiz;
    double exam;
    double weighted;
} class_t; 

// functions
void menu();
void addStudent(class_t student[], int *classCount, FILE *file);
void removeStudent(class_t student[], int *classCount);
void printGrade(class_t student[], int classCount);
void updateGrade(class_t student[], int classCount);
void missingGrades(class_t student[], int classCount);
int searchStudent(class_t student[], int classCount);
void displayAll(class_t student[], int classCount);

// string helper functions
void chomp(char word[]);
void clearBuffer();

int main() 
{
    class_t student[MAX];
    int classCount = 0;
    char choice;
    FILE *file; 

    printf("Welcome to your gradebook management system!\n");

    do {
        menu();
        printf("Selection: ");
        scanf(" %c", &choice);

        // changes input to upper to match switch case
        choice = toupper(choice);

        // directs to proper function
        switch(choice) {
            case 'A':
                if(classCount >= MAX) 
                {
                    printf("Your gradebook is full!\n");
                    break;
                }
                addStudent(student, &classCount, file);
                break;
            case 'R':
                removeStudent(student, &classCount);
                break;
            case 'P':
                printGrade(student, classCount);
                break;
            case 'U':
                updateGrade(student, classCount);
                break;
            case 'M':
                missingGrades(student, classCount);
                break;
            case 'S':
                searchStudent(student, classCount);
                break;
            case 'D':
                displayAll(student, classCount);
                break;
            case 'E':
                printf("Exiting the Gradebook System and exporting all records into a file called gradebook.txt.\n");
                break;
            default:
                printf("Invalid choice. Try again!\n");
        }
    } while(choice!= 'E');

    // printing final report onto a text file
    file = fopen("gradebook.txt", "w"); 

    for(int i = 0; i < classCount; i++)
    {
        fprintf(file, "Name: %s\n", student[i].name);
        fprintf(file, "ID: %d\n", student[i].ID);
        fprintf(file, "Homework Average: %.2lf\n", student[i].homework);
        fprintf(file, "Quiz Average: %.2lf\n", student[i].quiz);
        fprintf(file, "Exam Average: %.2lf\n", student[i].exam);
        fprintf(file, "Weighted Average: %.2lf\n", student[i].weighted);
        fprintf(file, "--------------------------------------------------------\n");
    }

    fclose(file);

    return 0;
}

// ADD STUDENT 
void addStudent(class_t student[], int *classCount, FILE *file) 
{
    printf("Enter the Student's Full Name: ");

    // user input on student name
    clearBuffer();
    fgets(student[*classCount].name, sizeof(student[*classCount].name), stdin);
    chomp(student[*classCount].name);

    //  generating ID number 
    student[*classCount].ID = rand() % 100 + 1;
    printf("Student ID#: %d\n", student[*classCount].ID);


    student[*classCount].homework = 0.0;
    student[*classCount].quiz = 0.0;
    student[*classCount].exam = 0.0;
    student[*classCount].weighted = 0.0;

    (*classCount)++;
}

// MENU SELECTIONS
void menu()
{
    printf("**********************************************\n");
    printf("What would you like to do in the gradebook?\n");
    printf("A: Add a Student to the Gradebook\n");
    printf("R: Remove a Student from the Gradebook\n");
    printf("P: Print Grades from the Gradebook\n");
    printf("U: Update a Student's Grade\n");
    printf("M: Determine who is missing Grades\n");
    printf("S: Search for a Student in the Gradebook\n");
    printf("D: Display all names and associated IDs\n");
    printf("E: Exit\n");
    printf("**********************************************\n");
}

// REMOVING STUDENTS
void removeStudent(class_t student[], int *classCount) 
{
    printf("You are now removing a student.\n");

    int person = searchStudent(student, *classCount);

    if (person >= 0 && person < *classCount) 
    {
        // shift elements down left
        for (int i = person; i < *classCount - 1; i++) 
        {
            student[i] = student[i + 1];
        }

        // clear out deleted student
        strcpy(student[*classCount - 1].name, "");
        student[*classCount - 1].ID = 0;          
        student[*classCount - 1].homework = 0.0;   
        student[*classCount - 1].quiz = 0.0;       
        student[*classCount - 1].exam = 0.0;       
        student[*classCount - 1].weighted = 0.0;   

        (*classCount)--; 
        printf("Student has been removed.\n");
    } 
}

// UPDATING GRADES
void updateGrade(class_t student[], int classCount)
{
    printf("You are now updating a grade. Which student would you like to update?\n");

    int person = searchStudent(student, classCount);

    printf("You are about to update the grades entered for %s.\n", student[person].name);
    printf("Please enter the respective score for each assessment group.\n");

    // update homework
    printf("Enter the overall homework average: ");
    scanf("%lf", &student[person].homework); // 50% overall

    // update quiz
    printf("Enter the overall quiz average: ");
    scanf("%lf", &student[person].quiz); // 20% overall

    // update exam 
    printf("Enter the overall exam average: ");
    scanf("%lf", &student[person].exam); // 30% overall
    
    student[person].weighted = (student[person].homework * 0.50) + (student[person].quiz * 0.20) + (student[person].exam * 0.30);

    printf("Success! %s's grades entered are now updated in your gradebook!\n", student[person].name);
}

// DETERMINE MISSING GRADES
void missingGrades(class_t student[], int classCount)
{
    printf("The following students have missing grades not entered into the book.\n");
    printf("--------------------------------------------------------\n");

    // runs through array and finds students with missing grades
    for(int i = 0; i < classCount; i++) 
    {
        if(student[i].name[0] != '\0' && (student[i].homework == 0.0 || student[i].quiz == 0.0 || student[i].exam == 0.0))
        {
            printf("%s\n", student[i].name);
        }
    }
}

// SEARCH FOR STUDENT
int searchStudent(class_t student[], int classCount)
{
    // displaying all student information 
    displayAll(student, classCount);

    // asks user to search for which student
    int search;
    printf("Please enter the ID of the Student: ");
    scanf("%d", &search);

    // finds the student 
    int index;
    for(int i = 0; i < MAX; i++)
    {
        if(student[i].ID == search)
        {
            printf("You searched for %s\n\n", student[i].name);
            index = i;
        }
    }

    return index; 
}


// DISPLAY STUDENT NAME & ID
void displayAll(class_t student[], int classCount) 
{
    printf("Here are the list of students and their IDs.\n");

    // runs through array and prints student info
    for(int i = 0; i < classCount; i++)
    {
        if(student[i].name[0] == '\0')
        {
            break;
        }
        else
        {
            printf("[%d] Name: %s \t ID: %d\n", i + 1, student[i].name, student[i].ID);
        }
    }
    printf("\n");
}

// PRINTING GRADES
void printGrade(class_t student[], int classCount) 
{
    // runs through array and prints report
    for(int i = 0; i < classCount; i++)
    {
        if(student[i].homework != 0 && student[i].quiz != 0 && student[i].exam != 0)
        {
            printf("Name: %s\n", student[i].name);
            printf("ID: %d\n", student[i].ID);
            printf("Homework Average: %.2lf%%\n", student[i].homework);
            printf("Quiz Average: %.2lf%%\n", student[i].quiz);
            printf("Exam Average: %.2lf%%\n", student[i].exam);
            printf("Weighted Average: %.2lf%%\n", student[i].weighted);
            printf("--------------------------------------------------------\n");
        }
    }

    // runs through array print ones with missing grades
    for(int i = 0; i < classCount; i++)
    {
        if(student[i].homework == 0 || student[i].quiz == 0 || student[i].exam == 0)
        {
            printf("%s does not have all grades entered yet.\n", student[i].name);
        }
    }
    
}


// STRING HELPER FUNCTIONS
void chomp(char word[])
{
    if(word[strlen(word) - 1] == '\n')
    {
        word[strlen(word) - 1] = '\0';
    }
}


void clearBuffer()
{
    int i = 0;
    while(getchar() != '\n' && i != EOF);
}
