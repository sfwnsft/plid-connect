#include <stdio.h>
#include <string.h>

// -------------------------------------------------------
// STRUCTURES
// -------------------------------------------------------

struct Patient {
    char username[100];
    char name[100];
    char gender[50];
    char discLevel[50];
    char severityText[50];
    char bmiText[50];
    int  age;
    int  severity;
    float height;
    float weight;
    float heightMeters;
    float bmi;
};

struct User {
    char username[100];
    char email[100];
    char password[100];
};

// -------------------------------------------------------
// GLOBAL
// -------------------------------------------------------

char loggedInUser[100] = "";

// -------------------------------------------------------
// HELPERS
// -------------------------------------------------------

// Checks if a line starts with a given label
// e.g. hasLabel("Username: john\n", "Username: ") => 1
int hasLabel(char line[], char label[]) {
    int len = strlen(label);
    int i;
    for (i = 0; i < len; i++) {
        if (line[i] != label[i]) {
            return 0;
        }
    }
    return 1;
}

// Extracts the value after a label in a line
// e.g. getValue("Username: john\n", "Username: ", out) => out = "john"
void getValue(char line[], char label[], char out[]) {
    int start = strlen(label);
    int len   = strlen(line);
    int i = 0;
    while (start + i < len && line[start + i] != '\n' && line[start + i] != '\0') {
        out[i] = line[start + i];
        i++;
    }
    out[i] = '\0';
}

// -------------------------------------------------------
// AUTH
// -------------------------------------------------------

void registerUser() {
    struct User u;
    char line[200];
    char fileUser[100];
    int exists;
    FILE *f;

    printf("\n--------------------------------------------------\n");
    printf("Register New User\n");
    printf("--------------------------------------------------\n");

    printf("Enter username: ");
    scanf("%s", u.username);

    // Check if username already exists
    exists = 0;
    f = fopen("users.txt", "r");
    if (f != NULL) {
        while (fgets(line, sizeof(line), f)) {
            if (hasLabel(line, "Username: ")) {
                getValue(line, "Username: ", fileUser);
                if (strcmp(fileUser, u.username) == 0) {
                    exists = 1;
                    break;
                }
            }
        }
        fclose(f);
    }

    if (exists == 1) {
        printf("Username already exists. Please choose a different username.\n\n");
        return;
    }

    printf("Enter email: ");
    scanf("%s", u.email);

    printf("Enter password: ");
    scanf("%s", u.password);

    f = fopen("users.txt", "a");
    if (f == NULL) {
        printf("Error: Could not open users file.\n\n");
        return;
    }

    fprintf(f, "Username: %s\n", u.username);
    fprintf(f, "Email: %s\n",    u.email);
    fprintf(f, "Password: %s\n\n", u.password);
    fclose(f);

    printf("\nRegistration successful! You can now login.\n\n");
}

// Returns 1 if login is successful, 0 if failed
int loginUser() {
    struct User u;
    char line[200];
    char fileUser[100];
    char filePass[100];
    int found;
    int passMatch;
    FILE *f;

    printf("\n--------------------------------------------------\n");
    printf("Login\n");
    printf("--------------------------------------------------\n");

    printf("Enter username: ");
    scanf("%s", u.username);

    printf("Enter password: ");
    scanf("%s", u.password);

    f = fopen("users.txt", "r");
    if (f == NULL) {
        printf("No user accounts found. Please register first.\n\n");
        return 0;
    }

    found     = 0;
    passMatch = 0;

    while (fgets(line, sizeof(line), f)) {
        if (hasLabel(line, "Username: ")) {
            getValue(line, "Username: ", fileUser);
            if (strcmp(fileUser, u.username) == 0) {
                found = 1;
                fgets(line, sizeof(line), f); // skip Email line
                fgets(line, sizeof(line), f); // read Password line
                if (hasLabel(line, "Password: ")) {
                    getValue(line, "Password: ", filePass);
                    if (strcmp(u.password, filePass) == 0) {
                        passMatch = 1;
                    }
                }
                break;
            }
        }
    }

    fclose(f);

    if (found == 0) {
        printf("Username not found. Please register first.\n\n");
        return 0;
    }

    if (passMatch == 0) {
        printf("Incorrect password.\n\n");
        return 0;
    }

    strcpy(loggedInUser, u.username);
    printf("Login successful! Welcome, %s.\n\n", u.username);
    return 1;
}

// -------------------------------------------------------
// PATIENT FEATURES
// -------------------------------------------------------

// Prints health tips based on severity and BMI
void showGuidance(int severity, float bmi) {
    printf("\nObservation & Guidance:\n");

    if (severity == 1) {
        printf("1. Mild condition: Start with rest and physiotherapy.\n");
    } else if (severity == 2) {
        printf("1. Moderate condition: Regular physiotherapy is needed.\n");
    } else {
        printf("1. Severe condition: Consult a spinal specialist quickly.\n");
    }

    if (bmi < 18.0) {
        printf("2. Low BMI may reduce muscle support for the spine.\n");
        printf("3. Improve nutrition and core-strength exercises.\n");
    } else if (bmi < 25.0) {
        printf("2. BMI is normal. Continue your spine-safe routine.\n");
        printf("3. Maintain posture and do regular stretching.\n");
    } else if (bmi < 30.0) {
        printf("2. Higher BMI increases pressure on lumbar discs.\n");
        printf("3. Start gradual weight loss and low-impact exercise.\n");
    } else {
        printf("2. High BMI strongly increases PLID strain and pain risk.\n");
        printf("3. Weight reduction should be part of your treatment plan.\n");
    }

    if (severity == 3 && bmi >= 30.0) {
        printf("4. Severe PLID + high BMI: urgent specialist follow-up advised.\n");
    } else if (severity >= 2 && bmi >= 25.0) {
        printf("4. Moderate/Severe PLID + high BMI: strict rehab and weight control needed.\n");
    } else {
        printf("4. Continue follow-up and monitor pain changes weekly.\n");
    }

    printf("5. Avoid lifting heavy objects and sudden bending or twisting.\n");
    printf("6. Use proper sitting posture. Avoid long sitting without breaks.\n");
    printf("7. Do daily gentle walking and approved back exercises.\n");
    printf("8. Sleep on a firm mattress and avoid very soft cushions.\n");
    printf("9. Seek urgent care if you notice leg weakness, numbness, or bladder issues.\n");

    if (severity == 1) {
        printf("10. Re-check in 4-6 weeks if pain does not improve.\n\n");
    } else if (severity == 2) {
        printf("10. Re-check in 2-4 weeks and track your pain score daily.\n\n");
    } else {
        printf("10. Keep all specialist appointments and follow treatment strictly.\n\n");
    }
}

void addPatient() {
    struct Patient p;
    FILE *f;

    printf("\n--------------------------------------------------\n");
    printf("Add New Patient\n");
    printf("--------------------------------------------------\n");

    printf("Username (e.g. john123): ");
    scanf("%s", p.username);

    printf("Nickname: ");
    scanf("%s", p.name);

    printf("Age: ");
    scanf("%d", &p.age);

    printf("Gender (M/F): ");
    scanf("%s", p.gender);

    printf("Disc Level (e.g. L1-L2, L4-L5): ");
    scanf("%s", p.discLevel);

    printf("Severity (1 = Mild, 2 = Moderate, 3 = Severe): ");
    scanf("%d", &p.severity);

    printf("Height in feet (e.g. 5.7): ");
    scanf("%f", &p.height);

    printf("Weight in kg: ");
    scanf("%f", &p.weight);

    // Calculate BMI
    p.heightMeters = p.height * 0.3048;
    p.bmi          = p.weight / (p.heightMeters * p.heightMeters);

    // Severity to text
    if (p.severity == 1) {
        strcpy(p.severityText, "Mild");
    } else if (p.severity == 2) {
        strcpy(p.severityText, "Moderate");
    } else {
        strcpy(p.severityText, "Severe");
    }

    // BMI to category text
    if (p.bmi < 18.0) {
        strcpy(p.bmiText, "Underweight");
    } else if (p.bmi < 25.0) {
        strcpy(p.bmiText, "Normal");
    } else if (p.bmi < 30.0) {
        strcpy(p.bmiText, "Overweight");
    } else {
        strcpy(p.bmiText, "Obese");
    }

    f = fopen("patient-data.txt", "a");
    if (f == NULL) {
        printf("Error: Could not open data file for writing.\n\n");
        return;
    }

    fprintf(f, "Username: %s\n",     p.username);
    fprintf(f, "Name: %s\n",         p.name);
    fprintf(f, "Age: %d\n",          p.age);
    fprintf(f, "Gender: %s\n",       p.gender);
    fprintf(f, "Disc Level: %s\n",   p.discLevel);
    fprintf(f, "Severity: %s\n",     p.severityText);
    fprintf(f, "BMI: %.2f (%s)\n\n", p.bmi, p.bmiText);
    fclose(f);

    printf("\nPatient data saved successfully!\n");
    printf("Your Username is: %s (Use this to view your data later)\n", p.username);

    showGuidance(p.severity, p.bmi);
}

void searchRecord() {
    char username[100];
    char fileUser[100];
    char line[200];
    int  found;
    int  inRecord;
    FILE *f;

    printf("\n--------------------------------------------------\n");
    printf("Search Your Records\n");
    printf("--------------------------------------------------\n");

    printf("Enter your Username: ");
    scanf("%s", username);

    f = fopen("patient-data.txt", "r");
    if (f == NULL) {
        printf("No patient records found.\n\n");
        return;
    }

    found    = 0;
    inRecord = 0;

    while (fgets(line, sizeof(line), f)) {
        if (hasLabel(line, "Username: ")) {
            getValue(line, "Username: ", fileUser);
            if (strcmp(fileUser, username) == 0) {
                found    = 1;
                inRecord = 1;
                printf("\n------ Your Patient Record ------\n");
            } else {
                inRecord = 0;
            }
        }
        if (inRecord == 1) {
            printf("%s", line);
        }
    }

    fclose(f);

    if (found == 0) {
        printf("No record found with username: %s\n", username);
        printf("Please check your username and try again.\n\n");
    } else {
        printf("--------------------------------------------------\n\n");
    }
}

void updateRecord() {
    char username[100];
    char fileUser[100];
    char newValue[100];
    char line[200];
    char field;
    int  found;
    int  inRecord;
    FILE *f;
    FILE *temp;

    printf("\n--------------------------------------------------\n");
    printf("Update Patient Record\n");
    printf("--------------------------------------------------\n");

    printf("Enter username to update: ");
    scanf("%s", username);

    printf("Which field to update?\n");
    printf("(N = Name, A = Age, G = Gender, D = Disc Level, S = Severity): ");
    scanf(" %c", &field);

    // Accept lowercase too
    if (field >= 'a' && field <= 'z') {
        field = field - 'a' + 'A';
    }

    printf("Enter new value: ");
    scanf("%s", newValue);

    f    = fopen("patient-data.txt", "r");
    temp = fopen("temp.txt", "w");

    if (f == NULL || temp == NULL) {
        printf("Error: Could not open files.\n\n");
        if (f    != NULL) fclose(f);
        if (temp != NULL) fclose(temp);
        return;
    }

    found    = 0;
    inRecord = 0;

    while (fgets(line, sizeof(line), f)) {
        if (hasLabel(line, "Username: ")) {
            getValue(line, "Username: ", fileUser);
            if (strcmp(fileUser, username) == 0) {
                found    = 1;
                inRecord = 1;
            } else {
                inRecord = 0;
            }
            fprintf(temp, "%s", line);
        }
        else if (inRecord == 1 && hasLabel(line, "Name: ")       && field == 'N') {
            fprintf(temp, "Name: %s\n", newValue);
        }
        else if (inRecord == 1 && hasLabel(line, "Age: ")        && field == 'A') {
            fprintf(temp, "Age: %s\n", newValue);
        }
        else if (inRecord == 1 && hasLabel(line, "Gender: ")     && field == 'G') {
            fprintf(temp, "Gender: %s\n", newValue);
        }
        else if (inRecord == 1 && hasLabel(line, "Disc Level: ") && field == 'D') {
            fprintf(temp, "Disc Level: %s\n", newValue);
        }
        else if (inRecord == 1 && hasLabel(line, "Severity: ")   && field == 'S') {
            fprintf(temp, "Severity: %s\n", newValue);
        }
        else {
            fprintf(temp, "%s", line);
        }
    }

    fclose(f);
    fclose(temp);

    if (found == 1) {
        remove("patient-data.txt");
        rename("temp.txt", "patient-data.txt");
        printf("Record updated successfully!\n\n");
    } else {
        remove("temp.txt");
        printf("Username not found.\n\n");
    }
}

void deleteRecord() {
    char username[100];
    char fileUser[100];
    char line[200];
    int  found;
    int  inRecord;
    FILE *f;
    FILE *temp;

    printf("\n--------------------------------------------------\n");
    printf("Delete Patient Record\n");
    printf("--------------------------------------------------\n");

    printf("Enter username to delete: ");
    scanf("%s", username);

    f    = fopen("patient-data.txt", "r");
    temp = fopen("temp.txt", "w");

    if (f == NULL || temp == NULL) {
        printf("Error: Could not open files.\n\n");
        if (f    != NULL) fclose(f);
        if (temp != NULL) fclose(temp);
        return;
    }

    found    = 0;
    inRecord = 0;

    while (fgets(line, sizeof(line), f)) {
        if (hasLabel(line, "Username: ")) {
            getValue(line, "Username: ", fileUser);
            if (strcmp(fileUser, username) == 0) {
                found    = 1;
                inRecord = 1;
                printf("Found record for %s. Deleting...\n", username);
            } else {
                inRecord = 0;
            }
        }
        if (inRecord == 0) {
            fprintf(temp, "%s", line);
        }
    }

    fclose(f);
    fclose(temp);

    if (found == 1) {
        remove("patient-data.txt");
        rename("temp.txt", "patient-data.txt");
        printf("Record deleted successfully!\n\n");
    } else {
        remove("temp.txt");
        printf("Username not found.\n\n");
    }
}

// -------------------------------------------------------
// INFO
// -------------------------------------------------------

void showDoctors() {
    printf("\n--------------------------------------------------\n");
    printf("Doctor Recommendations\n");
    printf("--------------------------------------------------\n");
    printf("1. Dr. Shah Alam, MBBS (BSMRMU)\n");
    printf("2. Dr. Nazrul Islam, MBBS (DMC)\n");
    printf("3. Dr. Irin Akter, MBBS (RMC)\n\n");
}

void showAbout() {
    printf("\n--------------------------------------------------\n");
    printf("About PLID\n");
    printf("--------------------------------------------------\n");
    printf("PLID means Prolapsed Lumbar Intervertebral Disc.\n");
    printf("This happens when a disc in the lower back moves out of place and presses nearby nerves.\n\n");

    printf("Common Causes:\n");
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

    printf("--------------------------------------------------\n");
    printf("About PLID Connect\n");
    printf("--------------------------------------------------\n");
    printf("PLID Connect is a solo project developed & maintained by Safwan Safat.\n");
    printf("It takes the user's data with consent and gives doctor recommendations,\n");
    printf("guidance, and healthcare tips based on the data.\n\n");
}

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

// -------------------------------------------------------
// MAIN
// -------------------------------------------------------

int main() {
    int choice;
    int authChoice;
    int loggedIn;

    loggedIn = 0;

    // Authentication loop
    while (loggedIn == 0) {
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
                if (loginUser() == 1) {
                    loggedIn = 1;
                }
                break;
            case 3:
                printf("\nThank you for using PLID Connect. Goodbye!\n");
                return 0;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }

    // Main menu loop
    while (1) {
        showMenu();
        printf("Your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addPatient();
                break;
            case 2:
                searchRecord();
                break;
            case 3:
                updateRecord();
                break;
            case 4:
                deleteRecord();
                break;
            case 5:
                showDoctors();
                break;
            case 6:
                showAbout();
                break;
            case 7:
                printf("\nTake care & stay connected with PLID Connect.\n");
                printf("Logged out as: %s\n\n", loggedInUser);
                return 0;
            default:
                printf("\nInvalid choice.\n");
        }
    }

    return 0;
}