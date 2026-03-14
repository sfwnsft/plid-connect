#include <stdio.h>
#define DATA_FILE "patients.txt"

int main() {
    int choice;

    while (1) {
        printf("--------------------------------------------\n");
        printf("PLID Patient Management System\n");
        printf("--------------------------------------------\n");
        printf("1. Add New Patient\n");
        printf("2. View All Patients\n");
        printf("3. Exit\n");
        printf("--------------------------------------------\n");
        printf("Your choice: ");
        scanf("%d", &choice);

           // OPTION 1 - Add New Patient
        if (choice == 1) {

            char name[50];
            int  age;
            char gender[10];
            char disc_level[10];
            int  severity;
            char severity_label[20];

            printf("\n--------------------------------------------\n");
            printf("ADD NEW PATIENT\n");
            printf("--------------------------------------------\n");

            printf("Name: ");
            scanf(" %[^\n]", name);

            printf("Age: ");
            scanf("%d", &age);
            while (age <= 0 || age > 120) {
                printf("  Invalid age. Please enter a valid age: ");
                scanf("%d", &age);
            }

            printf("Gender (M/F): ");
            scanf(" %s", gender);

            printf("Disc Level (e.g. L4-L5): ");
            scanf(" %s", disc_level);

            // Validated severity input
            printf("Severity (1=Mild, 2=Moderate, 3=Severe): ");
            scanf("%d", &severity);
            while (severity < 1 || severity > 3) {
                printf("Invalid! Please enter 1, 2, or 3: ");
                scanf("%d", &severity);
            }

            // Severity label
            if (severity == 1) {
                severity_label[0]='M'; severity_label[1]='i';
                severity_label[2]='l'; severity_label[3]='d';
                severity_label[4]='\0';
            } else if (severity == 2) {
                severity_label[0]='M'; severity_label[1]='o';
                severity_label[2]='d'; severity_label[3]='e';
                severity_label[4]='r'; severity_label[5]='a';
                severity_label[6]='t'; severity_label[7]='e';
                severity_label[8]='\0';
            } else {
                severity_label[0]='S'; severity_label[1]='e';
                severity_label[2]='v'; severity_label[3]='e';
                severity_label[4]='r'; severity_label[5]='e';
                severity_label[6]='\0';
            }

            // Save to file
            FILE *fp;
            fp = fopen(DATA_FILE, "a");

            if (fp == NULL) {
                printf("\nERROR: Could not open file.\n");
            } else {
                fprintf(fp, "--------------------------------------------\n");
                fprintf(fp, "Name: %s\n", name);
                fprintf(fp, "Age: %d\n", age);
                fprintf(fp, "Gender: %s\n", gender);
                fprintf(fp, "Disc Level: %s\n", disc_level);
                fprintf(fp, "Severity: %s\n", severity_label);
                fprintf(fp, "--------------------------------------------\n\n");
                fclose(fp);
                printf("\nPatient saved successfully!\n");
            }

            // Guidance based on severity
            printf("\nGuidance:\n");
            if (severity == 1) {
                printf("1. Rest and avoid heavy lifting.\n");
                printf("2. Do physiotherapy exercises.\n");
                printf("3. Take pain relief as prescribed.\n");
            } else if (severity == 2) {
                printf("1. See a physiotherapist regularly.\n");
                printf("2. Consider epidural injection if pain persists.\n");
                printf("3. Avoid bending and twisting movements.\n");
            } else {
                printf("1. Consult a spinal surgeon immediately.\n");
                printf("2. Surgery (microdiscectomy) may be needed.\n");
                printf("3. Strict rest and pain management required.\n");
            }

            printf("\nGoodbye!\n\n");
            break;

           // OPTION 2 - View All Patients
        } else if (choice == 2) {

            char line[150];
            FILE *fp;
            fp = fopen(DATA_FILE, "r");

            if (fp == NULL) {
                printf("\nNo records found. Add a patient first.\n");
            } 

            else {
                printf("\n--------------------------------------------\n");
                printf("ALL PATIENT RECORDS\n");
                printf("--------------------------------------------\n");
                while (fgets(line, sizeof(line), fp) != NULL) {
                    printf("  %s", line);
                }
                fclose(fp);
            }

          // OPTION 3 - Exit
        } else if (choice == 3) {
            printf("\nGoodbye!\n\n");
            break;

        } else {
            printf("\nInvalid choice. Please enter 1, 2, or 3.\n");
        }

        printf("\nPress Enter to continue...");
        while (getchar() != '\n');  /* clear leftover input */
        getchar();                  /* wait for Enter */
        printf("\n");
    }

    return 0;
}
