#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct patient {
    char username[100];
    int age;
    int severity;
    float height;
    float weight;
    float bmi;
    float heightMeters;
    char name[100];
    char gender[50];
    char discLevel[50];
    char severityText[50];
    char bmiText[50];
};

struct user {
    char username[100];
    char email[100];
    char password[100];
};

// Simple password hashing function (basic obfuscation for demo)
void hashPassword(char *password, char *hashed) {
    strcpy(hashed, "");
    for (int i = 0; password[i] != '\0'; i++) {
        char buffer[10];
        sprintf(buffer, "%d", (int)password[i] + 7);
        strcat(hashed, buffer);
    }
}

// Verify password by comparing hashes
int verifyPassword(char *password, char *hashed) {
    char computedHash[500];
    hashPassword(password, computedHash);
    return strcmp(computedHash, hashed) == 0;
}

// Register a new user
void registerUser() {
    struct user newUser;
    FILE *userFile;
    char usernameLine[200];

    printf("\n--------------------------------------------------\n");
    printf("Register New User\n");
    printf("--------------------------------------------------\n");

    printf("Enter username: ");
    scanf("%s", newUser.username);

    // Check if username already exists
    userFile = fopen("users.txt", "r");
    if (userFile != NULL) {
        while (fgets(usernameLine, sizeof(usernameLine), userFile)) {
            if (strstr(usernameLine, "Username: ") && strstr(usernameLine, newUser.username)) {
                printf("Username already exists. Please choose a different username.\n\n");
                fclose(userFile);
                return;
            }
        }
        fclose(userFile);
    }

    printf("Enter email: ");
    scanf("%s", newUser.email);

    printf("Enter password: ");
    scanf("%s", newUser.password);

    // Append new user to users.txt
    userFile = fopen("users.txt", "a");
    if (userFile == NULL) {
        printf("Error: Could not open users file.\n\n");
        return;
    }

    char hashedPassword[500];
    hashPassword(newUser.password, hashedPassword);

    fprintf(userFile, "Username: %s\n", newUser.username);
    fprintf(userFile, "Email: %s\n", newUser.email);
    fprintf(userFile, "Password: %s\n\n", hashedPassword);
    fclose(userFile);

    printf("\nRegistration successful! You can now login with your credentials.\n\n");
}

// Login user - returns 1 if successful, 0 if failed
int loginUser(char *loggedInUsername) {
    char username[100];
    char password[100];
    FILE *userFile;
    char line[200];
    int found = 0;
    int passwordMatches = 0;
    char storedPassword[500];

    printf("\n--------------------------------------------------\n");
    printf("Login\n");
    printf("--------------------------------------------------\n");

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    userFile = fopen("users.txt", "r");
    if (userFile == NULL) {
        printf("No user accounts found. Please register first.\n\n");
        return 0;
    }

    // Find user and verify password
    while (fgets(line, sizeof(line), userFile)) {
        if (strstr(line, "Username: ") && strstr(line, username)) {
            found = 1;
            strcpy(loggedInUsername, username);
            
            // Read password line
            if (fgets(line, sizeof(line), userFile)) {
                // Skip email line
            }
            if (fgets(line, sizeof(line), userFile)) {
                if (strstr(line, "Password: ")) {
                    sscanf(line, "Password: %s", storedPassword);
                    if (verifyPassword(password, storedPassword)) {
                        passwordMatches = 1;
                    }
                }
            }
            break;
        }
    }

    fclose(userFile);

    if (!found) {
        printf("Username not found. Please register first.\n\n");
        return 0;
    }

    if (!passwordMatches) {
        printf("Incorrect password.\n\n");
        return 0;
    }

    printf("Login successful! Welcome, %s.\n\n", username);
    return 1;
}

// Print the main menu options
void showMenu() { 
    printf("--------------------------------------------------\n");
    printf("Welcome to PLID Connect\n");
    printf("--------------------------------------------------\n");
    printf("1. Add a New Patient\n");
    printf("2. Search Your Records\n");
    printf("3. Update Patient Record\n");
    printf("4. Delete Patient Record\n");
    printf("5. Doctor Recommendations\n");
    printf("6. About PLID & PLID Connect\n");
    printf("7. Exit\n");
    printf("--------------------------------------------------\n");
}

// Show background info about PLID and this app
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

// Show doctor recommendations list
void docRecom() {
    printf("\n--------------------------------------------------\n");
    printf("Doctor Recommendations\n");
    printf("--------------------------------------------------\n");
    printf("1. Dr. Shah Alam, MBBS (BSMRMU)\n");
    printf("2. Dr. Nazrul Islam, MBBS (DMC)\n");
    printf("3. Dr. Irin Akter, MBBS (RMC)\n\n");
}

// Find and display a single patient's record by username
void viewPatientDataById() {
    char username[100];
    FILE *dataFile;
    char line[100];
    int found = 0;
    int inPatientRecord = 0;

    printf("\n--------------------------------------------------\n");
    printf("View Your Data\n");
    printf("--------------------------------------------------\n");
    
    // Ask for username to match in the file
    printf("Enter your Username: ");
    scanf("%s", username);

    // Open the data file for reading
    dataFile = fopen("patient-data.txt", "r");
    if (dataFile == NULL) {
        printf("No patient records found.\n\n");
        return;
    }

    // Read file line by line and print only the matching record
    while (fgets(line, sizeof(line), dataFile)) {
        if (inPatientRecord) {
            printf("%s", line);
            if (strlen(line) == 1 && line[0] == '\n') {
                inPatientRecord = 0;
            }
        } 
        else if (strstr(line, "Username: ") && strstr(line, username)) {
            found = 1;
            inPatientRecord = 1;
            printf("\n------ Your Patient Record ------\n");
            printf("%s", line); 
        }
    }

    fclose(dataFile);

    if (!found) {
        printf("No patient record found with username: %s\n", username);
        printf("Please check your username and try again.\n\n");
    } else {
        printf("--------------------------------------------------\n\n");
    }
}

// Print guidance based on severity and BMI
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

// Collect user input, calculate BMI, and save a new patient record
void addPatient() {
    struct patient p = {0};
    FILE *dataFile;

    printf("\n--------------------------------------------------\n");
    printf("Add New Patient\n");
    printf("--------------------------------------------------\n");

    // Basic patient info
    printf("Username (e.g. john123): ");
    scanf("%s", p.username);

    printf("Nickname: ");
    scanf("%s", p.name);

    printf("Age: ");
    scanf("%d", &p.age);
    
    printf("Gender (M/F): ");
    scanf("%s", p.gender);

    printf("Disc Level (e.g. L1-L2, L2-L3, L4-L5, L4-S1): ");
    scanf("%s", p.discLevel);

    printf("Severity (1 = Mild, 2 = Moderate, 3 = Severe): ");
    scanf("%d", &p.severity);
    
    printf("Height in feet (e.g. 5.7): ");
    scanf("%f", &p.height);
    
    printf("Weight in kg: ");
    scanf("%f", &p.weight);
    
    // BMI calculation
    p.heightMeters = p.height * 0.3048;
    p.bmi = p.weight / (p.heightMeters * p.heightMeters);

    if (p.severity == 1) {
        strcpy(p.severityText, "Mild");
    } 
    else if (p.severity == 2) {
        strcpy(p.severityText, "Moderate");
    } 
    else {
        strcpy(p.severityText, "Severe");
    }

    if (p.bmi < 18.0) {
        strcpy(p.bmiText, "Underweight");
    } 
    else if (p.bmi < 25.0) {
        strcpy(p.bmiText, "Normal");
    } 
    else if (p.bmi < 30.0) {
        strcpy(p.bmiText, "Overweight");
    } 
    else {
        strcpy(p.bmiText, "Obese");
    }

    // Append the new record to the data file
    dataFile = fopen("patient-data.txt", "a");
    if (dataFile == NULL) {
        printf("Error: Could not open data file for writing.\n\n");
        return;
    }
    
    fprintf(dataFile, "Username: %s\n", p.username);
    fprintf(dataFile, "Name: %s\n", p.name);
    fprintf(dataFile, "Age: %d\n", p.age);
    fprintf(dataFile, "Gender: %s\n", p.gender);
    fprintf(dataFile, "Disc Level: %s\n", p.discLevel);
    fprintf(dataFile, "Severity: %s\n", p.severityText);
    fprintf(dataFile, "BMI: %.2f (%s)\n\n", p.bmi, p.bmiText);
    fclose(dataFile);

    printf("\nPatient's data saved successfully!\n");
    printf("Your Username is: %s (Use this to view your data later)\n", p.username);
    PlidGuidance(p.severity, p.bmi);
}

// Update an existing patient's record
void updatePatient() {
    char username[100];
    FILE *inputFile, *tempFile;
    char line[200];
    int found = 0;
    int inTargetRecord = 0;
    int fieldChoice;
    char newValue[100];

    printf("\n--------------------------------------------------\n");
    printf("Update Patient Record\n");
    printf("--------------------------------------------------\n");

    printf("Enter username to update: ");
    scanf("%s", username);

    inputFile = fopen("patient-data.txt", "r");
    if (inputFile == NULL) {
        printf("No patient records found.\n\n");
        return;
    }

    tempFile = fopen("patient-data.txt.tmp", "w");
    if (tempFile == NULL) {
        printf("Error: Could not create temporary file.\n\n");
        fclose(inputFile);
        return;
    }

    // First pass: find and display the record
    while (fgets(line, sizeof(line), inputFile)) {
        if (strstr(line, "Username: ") && strstr(line, username)) {
            found = 1;
            inTargetRecord = 1;
            printf("\nPatient found! Current record:\n");
            printf("%s", line);
            fprintf(tempFile, "%s", line);
        }
        else if (inTargetRecord) {
            printf("%s", line);
            if (strlen(line) == 1 && line[0] == '\n') {
                inTargetRecord = 0;
            }
            fprintf(tempFile, "%s", line);
        }
        else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(inputFile);

    if (found) {
        printf("\nChoose field to update:\n");
        printf("1. Name\n");
        printf("2. Age\n");
        printf("3. Gender\n");
        printf("4. Disc Level\n");
        printf("5. Severity\n");
        printf("Your choice: ");
        scanf("%d", &fieldChoice);

        printf("Enter new value: ");
        scanf("%s", newValue);

        // Second pass: update the specific field
        inputFile = fopen("patient-data.txt", "r");
        rewind(tempFile);
        fclose(tempFile);
        tempFile = fopen("patient-data.txt.tmp", "w");
        inTargetRecord = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            if (strstr(line, "Username: ") && strstr(line, username)) {
                inTargetRecord = 1;
                fprintf(tempFile, "%s", line);
            }
            else if (inTargetRecord) {
                if (strlen(line) == 1 && line[0] == '\n') {
                    inTargetRecord = 0;
                    fprintf(tempFile, "\n");
                }
                else if (fieldChoice == 1 && strstr(line, "Name: ")) {
                    fprintf(tempFile, "Name: %s\n", newValue);
                }
                else if (fieldChoice == 2 && strstr(line, "Age: ")) {
                    fprintf(tempFile, "Age: %s\n", newValue);
                }
                else if (fieldChoice == 3 && strstr(line, "Gender: ")) {
                    fprintf(tempFile, "Gender: %s\n", newValue);
                }
                else if (fieldChoice == 4 && strstr(line, "Disc Level: ")) {
                    fprintf(tempFile, "Disc Level: %s\n", newValue);
                }
                else if (fieldChoice == 5 && strstr(line, "Severity: ")) {
                    fprintf(tempFile, "Severity: %s\n", newValue);
                }
                else {
                    fprintf(tempFile, "%s", line);
                }
            }
            else {
                fprintf(tempFile, "%s", line);
            }
        }

        fclose(inputFile);
        fclose(tempFile);

        remove("patient-data.txt");
        rename("patient-data.txt.tmp", "patient-data.txt");
        printf("\nRecord updated successfully!\n\n");
    }
    else {
        fclose(tempFile);
        remove("patient-data.txt.tmp");
        printf("No patient record found with username: %s\n\n", username);
    }
}

// Delete a patient's record
void deletePatient() {
    char username[100];
    char confirm;
    FILE *inputFile, *tempFile;
    char line[200];
    int found = 0;
    int inTargetRecord = 0;

    printf("\n--------------------------------------------------\n");
    printf("Delete Patient Record\n");
    printf("--------------------------------------------------\n");

    printf("Enter username to delete: ");
    scanf("%s", username);

    inputFile = fopen("patient-data.txt", "r");
    if (inputFile == NULL) {
        printf("No patient records found.\n\n");
        return;
    }

    tempFile = fopen("patient-data.txt.tmp", "w");
    if (tempFile == NULL) {
        printf("Error: Could not create temporary file.\n\n");
        fclose(inputFile);
        return;
    }

    while (fgets(line, sizeof(line), inputFile)) {
        if (strstr(line, "Username: ") && strstr(line, username)) {
            found = 1;
            inTargetRecord = 1;
            printf("\nPatient found! Current record:\n");
            printf("%s", line);
        }
        else if (inTargetRecord) {
            printf("%s", line);
            if (strlen(line) == 1 && line[0] == '\n') {
                inTargetRecord = 0;
            }
        }
        else {
            fprintf(tempFile, "%s", line);
        }
    }

    fclose(inputFile);

    if (found) {
        printf("\nAre you sure you want to delete this record? (y/n): ");
        scanf(" %c", &confirm);

        if (confirm == 'y' || confirm == 'Y') {
            fclose(tempFile);
            remove("patient-data.txt");
            rename("patient-data.txt.tmp", "patient-data.txt");
            printf("Record deleted successfully!\n\n");
        }
        else {
            fclose(tempFile);
            remove("patient-data.txt.tmp");
            printf("Deletion cancelled.\n\n");
        }
    }
    else {
        fclose(tempFile);
        remove("patient-data.txt.tmp");
        printf("No patient record found with username: %s\n\n", username);
    }
}

// App entry point and main loop
int main() {
    int choice;
    int authChoice;
    char loggedInUsername[100];
    int isLoggedIn = 0;

    // Authentication loop
    while (!isLoggedIn) {
        printf("\n--------------------------------------------------\n");
        printf("PLID Connect - Authentication\n");
        printf("--------------------------------------------------\n");
        printf("Manage your PLID condition with personalized\n");
        printf("health guidance and professional recommendations.\n");
        printf("--------------------------------------------------\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("--------------------------------------------------\n");
        printf("Your choice: ");
        scanf("%d", &authChoice);

        switch (authChoice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser(loggedInUsername)) {
                    isLoggedIn = 1;
                }
                break;
            case 3:
                printf("\nThank you for using PLID Connect. Goodbye!\n");
                return 0;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }

    // Main application loop (after successful login)
    while (1) {
        showMenu();
        printf("Your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                viewPatientDataById();
                break;
            case 3:
                updatePatient();
                break;
            case 4:
                deletePatient();
                break;
            case 5:
                docRecom();
                break;
            case 6:
                aboutPlid();
                break;
            case 7:
                printf("\nTake care & stay connected with PLID Connect.\n");
                printf("Logged out as: %s\n\n", loggedInUsername);
                return 0;
            default:
                printf("\nInvalid choice.\n");
        }
    }

    return 0;
}
