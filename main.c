#include <stdio.h>
#include <string.h>

void showMenu() { 
    printf("--------------------------------------------------\n");
    printf("Welcome to PLID Connect\n");
    printf("--------------------------------------------------\n");
    printf("1. Add a New Patient\n");
    printf("2. Doctor Recommendations\n");
    printf("3. About PLID & PLID Connect\n");
    printf("4. Exit\n");
    printf("--------------------------------------------------\n");
}

void aboutPlid() {
    printf("\n--------------------------------------------------\n");
    printf("About PLID\n");
    printf("--------------------------------------------------\n");
    printf("PLID means Prolapsed Lumbar Intervertebral Disc.\n");
    printf("This disorder happens when a disc in the lower back moves out of place and presses nearby nerves.\n\n");

    printf("Few Common Causes:\n");
    printf("1. Repeated heavy lifting with poor posture\n");
    printf("2. Sudden twisting or bending movements\n");
    printf("3. Age-related disc wear and tear\n");
    printf("4. Back injury or accident\n\n");

    printf("Risk Factors:\n");
    printf("1. Overweight or obesity\n");
    printf("2. Long duration of sitting\n");
    printf("3. Weak core and back muscles\n");
    printf("4. Smoking and low physical activity\n");
    printf("5. Family history of spine problems\n\n");

    printf("\n--------------------------------------------------\n");
    printf("About PLID Connect\n");
    printf("--------------------------------------------------\n");
    printf("PLID Connect is a solo project developed & maintained by Safwan Safat.\n");
    printf("It takes the user's data with consent and gives doctor recommendations, guidance, and healthcare tips based on the data.\n\n");
}

void docRecom() {
    printf("\n--------------------------------------------------\n");
    printf("Doctor Recommendations\n");
    printf("--------------------------------------------------\n");
    printf("1. Dr. Shah Alam, MBBS (BSMRMU)\n");
    printf("2. Dr. Nazrul Islam, MBBS (DMC)\n");
    printf("3. Dr. Irin Akter, MBBS (RMC)\n\n");
}

void PlidGuidance(int severity, float bmi) {
    printf("\nObservation & Guidance:\n");

    if (severity == 1) {
        printf("1. Mild condition: Start with rest and physiotherapy.\n");
    } 
    else if (severity == 2) {
        printf("1. Moderate condition: Regular physiotherapy is needed.\n");
    } 
    else {
        printf("1. Severe condition: Consult a spinal specialist quickly.\n");
    }

    if (bmi < 18.0) {
        printf("2. Low BMI may reduce muscle support for spine.\n");
        printf("3. Improve nutrition and core-strength exercises.\n");
    } 
    else if (bmi < 25.0) {
        printf("2. BMI is normal, continue spine-safe routine.\n");
        printf("3. Maintain posture and regular stretching.\n");
    } 
    else if (bmi < 30.0) {
        printf("2. Higher BMI increases pressure on lumbar discs.\n");
        printf("3. Start gradual weight loss and low-impact exercise.\n");
    } 
    else {
        printf("2. High BMI strongly increases PLID strain and pain risk.\n");
        printf("3. Weight reduction should be part of PLID treatment plan.\n");
    }

    if (severity == 3 && bmi >= 30.0) {
        printf("4. Severe PLID + high BMI: urgent specialist follow-up advised.\n");
    } 
    else if (severity >= 2 && bmi >= 25.0) {
        printf("4. Moderate/Severe PLID + high BMI: strict rehab and weight control needed.\n");
    } 
    else {
        printf("4. Continue follow-up and monitor pain changes weekly.\n");
    }

    printf("5. Avoid lifting heavy objects and sudden bending/twisting.\n");
    printf("6. Use proper sitting posture; avoid long sitting without breaks.\n");
    printf("7. Do daily gentle walking and approved back exercises.\n");
    printf("8. Sleep on a firm mattress and avoid very soft cushions.\n");
    printf("9. Seek urgent care if there is leg weakness, numbness, or bladder issues.\n");

    if (severity == 1) {
        printf("10. Re-check in 4-6 weeks if pain does not improve.\n\n");
    } 
    else if (severity == 2) {
        printf("10. Re-check in 2-4 weeks and track pain score daily.\n\n");
    } 
    else {
        printf("10. Keep specialist appointments and follow treatment strictly.\n\n");
    }
}

void addPatient() {
    int age;
    int severity;
    float height;
    float weight;
    float bmi;
    float heightMeters;
    char name[50];
    char gender[20];
    char discLevel[20];
    char severityText[20];
    char bmiText[20];
    FILE *dataFile;

    printf("\n--------------------------------------------------\n");
    printf("Add New Patient\n");
    printf("--------------------------------------------------\n");

    printf("Name: ");
    scanf(" %[^\n]", name);

    printf("Age: ");
    scanf("%d", &age);
    
    printf("Gender (M/F): ");
    scanf(" %s", gender);

    printf("Disc Level (e.g. L1-L2, L2-L3): ");
    scanf(" %s", discLevel);

    printf("Severity (1 = Mild, 2 = Moderate, 3 = Severe): ");
    scanf("%d", &severity);
    
    printf("Height in feet (e.g. 5.7): ");
    scanf("%f", &height);
    
    printf("Weight in kg: ");
    scanf("%f", &weight);
    
    heightMeters = height * 0.3048;
    bmi = weight / (heightMeters * heightMeters);

    if (severity == 1) {
        strcpy(severityText, "Mild");
    } 
    else if (severity == 2) {
        strcpy(severityText, "Moderate");
    } 
    else {
        strcpy(severityText, "Severe");
    }

    if (bmi < 18.0) {
        strcpy(bmiText, "Underweight");
    } 
    else if (bmi < 25.0) {
        strcpy(bmiText, "Normal");
    } 
    else if (bmi < 30.0) {
        strcpy(bmiText, "Overweight");
    } 
    else {
        strcpy(bmiText, "Obese");
    }

    dataFile = fopen("patient-data.txt", "a");
    
    if (dataFile == NULL) {
        printf("Error: Could not open data file for writing.\n\n");
        return;
    }

    fprintf(dataFile, "Name: %s\n", name);
    fprintf(dataFile, "Age: %d\n", age);
    fprintf(dataFile, "Gender: %s\n", gender);
    fprintf(dataFile, "Disc Level: %s\n", discLevel);
    fprintf(dataFile, "Severity: %s\n", severityText);
    fprintf(dataFile, "BMI: %.2f (%s)\n\n", bmi, bmiText);
    fclose(dataFile);

    printf("\nPatient's data saved successfully!\n");
    PlidGuidance(severity, bmi);
}

int main() {
    int choice;

    showMenu();
    printf("Your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            addPatient();
            break;
        case 2:
            docRecom();
            break;
        case 3:
            aboutPlid();
            break;
        case 4:
            printf("\nTake care & stay connected with PLID Connect.\n");
            break;
        default:
            printf("\nInvalid choice.\n");
    }
    return 0;
}
