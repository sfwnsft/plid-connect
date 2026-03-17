#include <stdio.h>
#include <string.h>

#define DATA_FILE "patients.txt"

void showMenu() {
    printf("--------------------------------------------\n");
    printf("PLID Patient Management System\n");
    printf("--------------------------------------------\n");
    printf("1. Add New Patient\n");
    printf("2. View All Patients\n");
    printf("3. About PLID\n");
    printf("4. Exit\n");
    printf("--------------------------------------------\n");
}

void aboutPlid() {
    printf("\n--------------------------------------------\n");
    printf("ABOUT PLID\n");
    printf("--------------------------------------------\n");
    printf("PLID means Prolapsed Lumbar Intervertebral Disc.\n");
    printf("It happens when a disc in the lower back moves out\n");
    printf("of place and presses nearby nerves.\n\n");

    printf("Common Causes:\n");
    printf("1. Repeated heavy lifting with poor posture\n");
    printf("2. Sudden twisting or bending movements\n");
    printf("3. Age-related disc wear and tear\n");
    printf("4. Back injury or accident\n\n");

    printf("Risk Factors:\n");
    printf("1. Overweight or obesity (high BMI)\n");
    printf("2. Long hours of sitting\n");
    printf("3. Weak core and back muscles\n");
    printf("4. Smoking and low physical activity\n");
    printf("5. Family history of spine problems\n");
}

void printPlidGuidance(int severity, float bmi) {
    printf("\nPLID Guidance:\n");

    if (severity == 1) {
        printf("1. Mild PLID: start with rest and physiotherapy.\n");
    } else if (severity == 2) {
        printf("1. Moderate PLID: regular physiotherapy is needed.\n");
    } else {
        printf("1. Severe PLID: consult a spinal specialist quickly.\n");
    }

    if (bmi < 18.5) {
        printf("2. Low BMI may reduce muscle support for spine.\n");
        printf("3. Improve nutrition and core-strength exercises.\n");
    } else if (bmi < 25.0) {
        printf("2. BMI is normal, continue spine-safe routine.\n");
        printf("3. Maintain posture and regular stretching.\n");
    } else if (bmi < 30.0) {
        printf("2. Higher BMI increases pressure on lumbar discs.\n");
        printf("3. Start gradual weight loss and low-impact exercise.\n");
    } else {
        printf("2. High BMI strongly increases PLID strain and pain risk.\n");
        printf("3. Weight reduction should be part of PLID treatment plan.\n");
    }

    if (severity == 3 && bmi >= 30.0) {
        printf("4. Severe PLID + high BMI: urgent specialist follow-up advised.\n");
    } else if (severity >= 2 && bmi >= 25.0) {
        printf("4. Moderate/Severe PLID + high BMI: strict rehab and weight control needed.\n");
    } else {
        printf("4. Continue follow-up and monitor pain changes weekly.\n");
    }

    printf("5. Avoid lifting heavy objects and sudden bending/twisting.\n");
    printf("6. Use proper sitting posture; avoid long sitting without breaks.\n");
    printf("7. Do daily gentle walking and approved back exercises.\n");
    printf("8. Sleep on a firm mattress and avoid very soft cushions.\n");
    printf("9. Seek urgent care if there is leg weakness, numbness, or bladder issues.\n");

    if (severity == 1) {
        printf("10. Re-check in 4-6 weeks if pain does not improve.\n");
    } else if (severity == 2) {
        printf("10. Re-check in 2-4 weeks and track pain score daily.\n");
    } else {
        printf("10. Keep specialist appointments and follow treatment strictly.\n");
    }
}

void addPatient() {
    char name[50];
    int age;
    char gender[10];
    char discLevel[20];
    int severity;
    char severityText[20];
    float height;
    float weight;
    float bmi;
    float heightMeters;
    char bmiText[20];
    FILE *fp;

    printf("\n--------------------------------------------\n");
    printf("ADD NEW PATIENT\n");
    printf("--------------------------------------------\n");

    printf("Name: ");
    scanf(" %49[^\n]", name);

    printf("Age: ");
    scanf("%d", &age);
    while (age < 1 || age > 120) {
        printf("Invalid age. Enter age (1-120): ");
        scanf("%d", &age);
    }

    printf("Gender (M/F): ");
    scanf(" %9s", gender);

    printf("Disc Level (e.g. L4-L5): ");
    scanf(" %19s", discLevel);

    printf("Severity (1=Mild, 2=Moderate, 3=Severe): ");
    scanf("%d", &severity);
    while (severity < 1 || severity > 3) {
        printf("Invalid severity. Enter 1, 2, or 3: ");
        scanf("%d", &severity);
    }

    printf("Height in feet (e.g. 5.7): ");
    scanf("%f", &height);
    while (height <= 0) {
        printf("Invalid height. Enter height in feet: ");
        scanf("%f", &height);
    }

    printf("Weight in kg (e.g. 68): ");
    scanf("%f", &weight);
    while (weight <= 0) {
        printf("Invalid weight. Enter weight in kg: ");
        scanf("%f", &weight);
    }

    heightMeters = height * 0.3048;
    bmi = weight / (heightMeters * heightMeters);

    if (severity == 1) {
        strcpy(severityText, "Mild");
    } else if (severity == 2) {
        strcpy(severityText, "Moderate");
    } else {
        strcpy(severityText, "Severe");
    }

    if (bmi < 18.5) {
        strcpy(bmiText, "Underweight");
    } else if (bmi < 25.0) {
        strcpy(bmiText, "Normal");
    } else if (bmi < 30.0) {
        strcpy(bmiText, "Overweight");
    } else {
        strcpy(bmiText, "Obese");
    }

    fp = fopen(DATA_FILE, "a");
    if (fp == NULL) {
        printf("\nERROR: Could not open file.\n");
        return;
    }

    fprintf(fp, "--------------------------------------------\n");
    fprintf(fp, "Name: %s\n", name);
    fprintf(fp, "Age: %d\n", age);
    fprintf(fp, "Gender: %s\n", gender);
    fprintf(fp, "Disc Level: %s\n", discLevel);
    fprintf(fp, "Severity: %s\n", severityText);
    fprintf(fp, "BMI: %.2f (%s)\n", bmi, bmiText);
    fprintf(fp, "--------------------------------------------\n\n");
    fclose(fp);

    printf("\nPatient saved successfully!\n");
    printPlidGuidance(severity, bmi);
}

void viewPatients() {
    char line[200];
    FILE *fp;

    fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        printf("\nNo records found. Add a patient first.\n");
        return;
    }

    printf("\n--------------------------------------------\n");
    printf("ALL PATIENT RECORDS\n");
    printf("--------------------------------------------\n");

    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }

    fclose(fp);
}

int main(void) {
    int choice;

    do {
        showMenu();
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                viewPatients();
                break;
            case 3:
                aboutPlid();
                break;
            case 4:
                printf("\nGoodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please enter 1, 2, 3, or 4.\n");
        }

        if (choice != 4) {
            printf("\nPress Enter to continue...");
            while (getchar() != '\n');
            getchar();
            printf("\n");
        }

    } while (choice != 4);

    return 0;
}
