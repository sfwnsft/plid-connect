#include <stdio.h>
#include <string.h>

struct Patient {
    char username[100];
    char name[100];
    char gender[50];
    char discLevel[50];
    char severityText[50];
    char bmiText[50];
    int age;
    int severity;
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

// checks if a line from the file starts with a given label
int lineStartsWith(char line[], char label[]) {
    int len = strlen(label);           // counts how many characters are in the label
    for (int i = 0; i < len; i++) {    // loops through each character of the label one by one
        if (line[i] != label[i]) {     // if any character doesn't match
            return 0;                  // return 0 meaning false - it does NOT start with this label
        }
    }
    return 1;                          // if all characters matched, return 1 meaning true
}

// extracts the value after a label from a line and stores it in out[]
void extractValue(char line[], char label[], char out[]) {
    int start = strlen(label);     // skip past the label to where the value begins
    int i = 0;                     // counter for copying characters one by one
    char ch = line[start];         // grab the first character of the value

    while (ch != '\n' && ch != '\0') {    // keep going until end of line or end of string
        out[i] = ch;                      // copy this character into out
        i++;                              // move to the next position
        ch = line[start + i];             // grab the next character
    }

    out[i] = '\0';    // mark the end of the string so C knows where it stops
}

void registerUser() {
    struct User u;
    char line[200];        // temporary storage for each line read from the file
    char fileUser[100];    // temporary storage for a username read from the file
    int exists;            // flag: will be 1 if the username already exists, 0 if not
    FILE *f;

    printf("\n--------------------------------------------------\n");
    printf("Register New User\n");
    printf("--------------------------------------------------\n");

    printf("Enter username: ");       // asks the user to type a username
    scanf("%s", u.username); 

    // Checks for availability of already registered username.
    exists = 0;                        // assume username does not exist yet
    f = fopen("users.txt", "r");       // opens users.txt for reading
    if (f != NULL) {                   // only proceed if the file was opened successfully
        while (fgets(line, sizeof(line), f)) {           // reads the file one line at a time
            if (lineStartsWith(line, "Username: ")) {    // checks if this line is a username line
                extractValue(line, "Username: ", fileUser);          // extracts the username from the line
                if (strcmp(fileUser, u.username) == 0) {             // checks if it matches the new username
                    exists = 1;        // username already taken, so exists = 1
                    break;             // found, so stopped.
                }
            }
        }
        fclose(f);    // closes the file after reading
    }

    // if username was already found
    if (exists == 1) {                                                    
        printf("Username already exists. Please choose a different username.\n\n");
        return;
    }

    printf("Enter email: ");
    scanf("%s", u.email);

    printf("Enter password: ");
    scanf("%s", u.password);

    f = fopen("users.txt", "a");    // opens users.txt in append mode
    if (f == NULL) {                // if the file couldn't be opened
        printf("Error: Could not open users file.\n\n");
        return;                                             // exit the function early
    }

    fprintf(f, "Username: %s\n", u.username);      // writes the username to the file
    fprintf(f, "Email: %s\n", u.email);         // writes the email to the file
    fprintf(f, "Password: %s\n\n", u.password);    // writes the password and a blank line after
    fclose(f);                                     // closes the file after writing

    printf("\nRegistration successful! You can now login.\n\n");
}

// handles logging in with an existing account, returns 1 if successful, 0 if failed
int loginUser() {
    struct User u;
    char line[200];         // temporary storage for each line read from the file
    char fileUser[100];     // temporary storage for a username read from the file
    char filePass[100];     // temporary storage for a password read from the file
    int found;              // 1 if the username was found in the file, 0 if not
    int passMatch;          // 1 if the password matched, 0 if not
    FILE *f;

    printf("\n--------------------------------------------------\n");
    printf("Login\n");
    printf("--------------------------------------------------\n");

    printf("Enter username: ");
    scanf("%s", u.username);

    printf("Enter password: ");
    scanf("%s", u.password);

    f = fopen("users.txt", "r");    // opens users.txt for reading
    if (f == NULL) {                // if the file doesn't exist yet (no users registered)
        printf("No user accounts found. Please register first.\n\n");
        return 0;                   // return 0 meaning login failed
    }

    found = 0;        // assume username not found yet
    passMatch = 0;    // assume password doesn't match yet

    while (fgets(line, sizeof(line), f)) {           // reads the file one line at a time
        if (lineStartsWith(line, "Username: ")) {    // checks if this line contains a username
            extractValue(line, "Username: ", fileUser);         // extracts the username from the line
            if (strcmp(fileUser, u.username) == 0) {
                found = 1;                                      // username was found, so 1
                fgets(line, sizeof(line), f);                   // skips the Email line (we don't need it)
                fgets(line, sizeof(line), f);                   // reads the next line which should be the Password line
                if (lineStartsWith(line, "Password: ")) {       // confirms this line is the password line
                    extractValue(line, "Password: ", filePass); // extracts the password from the line
                    if (strcmp(u.password, filePass) == 0) {    // compares typed password with stored password
                        passMatch = 1;                          // passwords match, set flag to 1
                    }
                }
                break;    // stop reading the file, password found & matched.
            }
        }
    }

    fclose(f);

    // if username was never found
    if (found == 0) {
        printf("Username not found. Please register first.\n\n");
        return 0;                                                  
    }

    // if password did not match
    if (passMatch == 0) {                      
        printf("Incorrect password.\n\n");
        return 0;
    }

    printf("Login successful! Welcome, %s.\n\n", u.username);
    return 1;
}

// prints personalized health guidance based on severity and BMI
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

// collects patient information, saves it to file, and shows guidance
void addPatient() {
    struct Patient p;
    FILE *f;
    int inches;

    printf("\n--------------------------------------------------\n");
    printf("Add New Patient\n");
    printf("--------------------------------------------------\n");

    printf("Username: ");
    scanf("%s", p.username);

    printf("Full Name: ");
    scanf(" ");                            // removes the leftover \n from the input buffer
    fgets(p.name, 100, stdin);

    printf("Age: ");
    scanf("%d", &p.age);

    printf("Gender (M/F): ");
    scanf("%s", p.gender);

    printf("Disc Level (e.g. L1-L2, L4-L5) (Uppercase/Lowercase): ");
    scanf("%s", p.discLevel);

    printf("Severity (1. Mild, 2. Moderate or 3. Severe): ");
    scanf("%d", &p.severity);

    printf("Height (Feet): ");
    scanf("%f", &p.height);

    printf("Height (Inches): ");
    scanf("%d", &inches);

    printf("Weight (Kg): ");
    scanf("%f", &p.weight);

    p.heightMeters = (p.height * 12 + inches) * 0.0254;   // converts feet and inches to meters
    p.bmi = p.weight / (p.heightMeters * p.heightMeters); // calculates BMI using weight/height * height

    if (p.severity == 1) {
        strcpy(p.severityText, "Mild");           // store the word "Mild" in severityText
    } else if (p.severity == 2) {
        strcpy(p.severityText, "Moderate");       // store the word "Moderate"
    } else {
        strcpy(p.severityText, "Severe");         // store the word "Severe"
    }

    if (p.bmi < 18.0) {
        strcpy(p.bmiText, "Underweight");
    } else if (p.bmi < 25.0) {
        strcpy(p.bmiText, "Normal");
    } else if (p.bmi < 30.0) {
        strcpy(p.bmiText, "Overweight");
    } else {
        strcpy(p.bmiText, "Obese");
    }

    f = fopen("patient-data.txt", "a");    // opens the patient data file in append mode
    if (f == NULL) {
        printf("Error: Could not open data file for writing.\n\n");
        return;
    }

    fprintf(f, "Username: %s\n", p.username);
    fprintf(f, "Name: %s\n", p.name);
    fprintf(f, "------ General Stats ------\n");
    fprintf(f, "Age: %d\n", p.age);
    fprintf(f, "Gender: %s\n", p.gender);
    fprintf(f, "Disc Level: %s\n", p.discLevel);
    fprintf(f, "Severity: %s\n", p.severityText);
    fprintf(f, "BMI: %.2f (%s)\n\n", p.bmi, p.bmiText);
    fclose(f);

    printf("\nPatient data saved successfully!\n");
    printf("Use your username: %s to view your data later\n", p.username);

    showGuidance(p.severity, p.bmi);    // calls the guidance function to show health tips
}

// searches for and displays a patient's record by username
void searchRecord() {
    char username[100];
    char fileUser[100];    // temporary storage for usernames read from the file
    char line[200];        // temporary storage for each line read from the file
    int found;             // 1 if the record was found, 0 if not
    int inRecord;          // 1 if we are currently inside the matching record, 0 if not
    FILE *f;

    printf("\n--------------------------------------------------\n");
    printf("Search Your Records\n");
    printf("--------------------------------------------------\n");

    printf("Enter your Username: ");
    scanf("%s", username);

    f = fopen("patient-data.txt", "r");    // opens the patient data file for reading
    if (f == NULL) {
        printf("No patient records found.\n\n");
        return;
    }

    found = 0;    // assume record not found yet
    inRecord = 0;    // assume we are not inside any record yet

    while (fgets(line, sizeof(line), f)) {                  // reads the file one line at a time
        if (lineStartsWith(line, "Username: ")) {           // if this line is a username line
            extractValue(line, "Username: ", fileUser);     // extract the username from the line
            if (strcmp(fileUser, username) == 0) {          // if it matches the searched username
                found = 1;                                  // mark as found
                inRecord = 1;                               // we are now inside the matching record
                printf("\n------ Your Record ------\n");
            } 
            else {
                inRecord = 0;
            }
        }
        if (inRecord == 1) {          // if we are currently inside the matching record
            printf("%s", line);       // print this line to the screen
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

// updates a specific field in a patient's record
void updateRecord() {
    char username[100];
    char fileUser[100];
    char newValue[100];     // stores the new value the user wants to set
    char line[200];         
    char field;             // stores the letter choice of which field to update (N, A, G, D, S)
    int found;              // 1 if the username was found, 0 if not
    int inRecord;           // 1 if we are currently inside the matching record, 0 if not
    FILE *f;                
    FILE *temp;             // file pointer for the temporary file used during update

    printf("\n--------------------------------------------------\n");
    printf("Update Patient Record\n");
    printf("--------------------------------------------------\n");

    printf("Enter username to update: ");
    scanf("%s", username);

    printf("Which field do you want to update?\n");
    
    printf("Name (N), Age (A), Gender (G), Disc Level (D), Severity (S): ");
    scanf(" %c", &field);                       // reads a single character (space before %c removes leftover \n)

    if (field >= 'a' && field <= 'z') {         // if the user typed a lowercase letter
        field = field - 'a' + 'A';              // convert it to uppercase so both cases work
    }

    printf("Enter new value: ");
    scanf("%s", newValue);

    f = fopen("patient-data.txt", "r");       // opens the original file for reading
    temp = fopen("temp.txt", "w");            // creates a temporary file for writing

    if (f == NULL || temp == NULL) {                   // if either file couldn't be opened
        printf("Error: Could not open files.\n\n");    // print error message
        
        if (f != NULL) {
            fclose(f);      // close original file if it was opened
        }
        if (temp != NULL) {
            fclose(temp);   // close temp file if it was opened
        }                
        return;
    }

    found = 0;       // assume username not found yet
    inRecord = 0;    // assume we are not inside any record yet

    while (fgets(line, sizeof(line), f)) {               // reads the file one line at a time
        if (lineStartsWith(line, "Username: ")) {        // if this line is a username line
            extractValue(line, "Username: ", fileUser);  // extract the username from the line
            if (strcmp(fileUser, username) == 0) {       
                found = 1;                               
                inRecord = 1;                            
            } 
            else {                                       // if it belongs to a different patient
                inRecord = 0;                            // we are no longer inside the matching record
            }

            fprintf(temp, "%s", line);                   // always write the username line unchanged
        }
        
        else if (inRecord == 1 && lineStartsWith(line, "Name: ") && field == 'N') {
            fprintf(temp, "Name: %s\n", newValue);
        }
        else if (inRecord == 1 && lineStartsWith(line, "Age: ")  && field == 'A') {
            fprintf(temp, "Age: %s\n", newValue);
        }
        else if (inRecord == 1 && lineStartsWith(line, "Gender: ")     && field == 'G') {
            fprintf(temp, "Gender: %s\n", newValue);
        }
        else if (inRecord == 1 && lineStartsWith(line, "Disc Level: ") && field == 'D') {
            fprintf(temp, "Disc Level: %s\n", newValue);
        }
        else if (inRecord == 1 && lineStartsWith(line, "Severity: ")   && field == 'S') {
            fprintf(temp, "Severity: %s\n", newValue);
        }
        else {
            fprintf(temp, "%s", line);    // for all other lines, copy them unchanged to temp file
        }
    }

    fclose(f);       // closes the original file
    fclose(temp);    // closes the temp file

    if (found == 1) {                               // if the username was found and updated
        remove("patient-data.txt");                 // deletes the original file
        rename("temp.txt", "patient-data.txt");     // renames temp file to replace the original
        printf("Record updated successfully!\n\n"); // confirms the update worked
    } 
    else {                                          // if the username was not found
        remove("temp.txt");                         // deletes the unused temp file
        printf("Username not found.\n\n");          // tells the user the username didn't exist
    }
}

// deletes a patient's entire record from the file
void deleteRecord() {
    char username[100];
    char fileUser[100];
    char line[200];
    int found;
    int inRecord;
    FILE *f;
    FILE *temp;

    printf("\n--------------------------------------------------\n");
    printf("Delete Patient Record\n");
    printf("--------------------------------------------------\n");

    printf("Enter username to delete: ");
    scanf("%s", username);

    f = fopen("patient-data.txt", "r");
    temp = fopen("temp.txt", "w");

    if (f == NULL || temp == NULL) {                   // if either file couldn't be opened
        printf("Error: Could not open files.\n\n");
        if (f != NULL) {
            fclose(f);
        }
        if (temp != NULL) {
            fclose(temp);
        }
        return;
    }

    found = 0;
    inRecord = 0;

    while (fgets(line, sizeof(line), f)) {
        if (lineStartsWith(line, "Username: ")) {
            extractValue(line, "Username: ", fileUser);
            if (strcmp(fileUser, username) == 0) {
                found = 1;
                inRecord = 1;
                printf("Found record for %s. Deleting...\n", username);
            } 
            else {                                     
                inRecord = 0;
            }
        }
        if (inRecord == 0) {
            fprintf(temp, "%s", line);     // copy this line to the temp file (keep it)
        }
        // if inRecord == 1, we do nothing — the line is skipped and effectively deleted
    }

    fclose(f);
    fclose(temp);

    if (found == 1) {                               // if the username was found and deleted
        remove("patient-data.txt");
        rename("temp.txt", "patient-data.txt");
        printf("Record deleted successfully!\n\n");
    } 
    else {                                          // if the username was not found
        remove("temp.txt");                         // deletes the unused temp file
        printf("Username not found.\n\n");
    }
}

// displays a list of recommended doctors
void showDoctors() {
    printf("\n--------------------------------------------------\n");
    printf("Verified PLID & Spine Specialists\n");
    printf("--------------------------------------------------\n");

    printf("1. Prof. Dr. Md. Shah Alam\n");
    printf("   Specialty: Orthopedic & Spine Surgery\n");
    printf("   Credentials: MBBS, MS (Ortho), FCPS (Surgery), FRCS (Ortho)\n");
    printf("   Details: Former Head of Ortho-Spine at Dhaka Medical College Hospital.\n\n");

    printf("2. Prof. Dr. M. Ali\n");
    printf("   Specialty: Orthopedic Surgery (Knee & Joint)\n");
    printf("   Credentials: MBBS, MS (Ortho), PhD\n");
    printf("   Details: Coordinator & Senior Consultant at Evercare Hospital, Dhaka.\n\n");

    printf("3. Prof. Dr. Md. Taslim Uddin\n");
    printf("   Specialty: Physical Medicine & Rehabilitation\n");
    printf("   Credentials: MBBS, FCPS (Physical Medicine)\n");
    printf("   Details: Professor at BSMMU; Expert in non-surgical PLID management.\n\n");

    printf("4. Prof. Dr. Mohammad Hossain\n");
    printf("   Specialty: Neurosurgery (Brain & Spine)\n");
    printf("   Credentials: MBBS, MS (Neurosurgery), FICS\n");
    printf("   Details: Professor at BSMMU; Specialist in Endoscopic Spine Surgery.\n\n");

    printf("--------------------------------------------------\n");
    printf("Consult a specialist based on your Severity Level.\n");
    printf("--------------------------------------------------\n\n");
}

// displays information about PLID and about this project
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
    printf("PLID Connect is a data-driven TUI based CRUD application developed & maintained by Safwan Safat.\n");
    printf("It manages user's records and provides automated specialist recommendations,\n");
    printf("guidance, healthcare tips based on the data.\n\n");
}

// displays the main menu options
void showMenu() {
    printf("--------------------------------------------------\n");
    printf("PLID Connect - Dashboard\n");
    printf("--------------------------------------------------\n");
    printf("1. Add a New Record\n");
    printf("2. Search Your Record\n");
    printf("3. Update Your Record\n");
    printf("4. Delete Your Record\n");
    printf("5. Doctor Recommendations\n");
    printf("6. About PLID & PLID Connect\n");
    printf("7. Exit\n");
    printf("--------------------------------------------------\n");
}

// the main function
int main() {
    int choice;
    int authChoice;      // stores the user's choice on the login/register screen
    int loggedIn = 0;    // at 0 (not logged in), 1 after successful login

    while (loggedIn == 0) {
        printf("\n--------------------------------------------------\n");
        printf("Welcome to PLID Connect!\n");
        printf("--------------------------------------------------\n");
        printf("Your Personalized PLID & Spine Health Manager\n");
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
                    loggedIn = 1;          // set loggedIn to 1 so the while loop ends
                }
                break;
            case 3:
                printf("\nThank you for using PLID Connect. Goodbye!\n");
                return 0;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }

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
                printf("\nTake care & stay connected with PLID Connect.\n\n");
                return 0;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }

    return 0;
}