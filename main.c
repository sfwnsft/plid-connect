#include <stdio.h>
#include <string.h>

struct Patient {
    char username[100];
    char name[100];
    char gender[50];
    char discLevel[50];     // stores the disc level (e.g. L4-L5)
    char severityText[50];  // stores severity as text (Mild, Moderate, Severe)
    char bmiText[50];       // stores BMI category as text (Normal, Obese, etc.)
    int  age;               // stores age as a whole number
    int  severity;          // stores severity as a number (1, 2, or 3)
    float height;           // stores height in feet
    float weight;           // stores weight in kilograms
    float heightMeters;     // stores height converted to meters (calculated later)
    float bmi;              // stores the calculated BMI value
};

// blueprint for storing one user's login account
struct User {
    char username[100];   // stores the login username
    char email[100];      // stores the email address
    char password[100];   // stores the password
};

// checks if a line from the file starts with a given label
int lineStartsWith(char line[], char label[]) {
    int len = strlen(label);            // counts how many characters are in the label
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

// handles registering a new user account
void registerUser() {
    struct User u;         // creates a User variable to hold the new user's data
    char line[200];        // temporary storage for each line read from the file
    char fileUser[100];    // temporary storage for a username read from the file
    int exists;            // flag: will be 1 if the username already exists, 0 if not
    FILE *f;               // file pointer used to open and read/write files

    printf("\n--------------------------------------------------\n");    // prints a divider line
    printf("Register New User\n");                                        // prints the section title
    printf("--------------------------------------------------\n");       // prints another divider

    printf("Enter username: ");       // asks the user to type a username
    scanf("%s", u.username);          // reads the typed username and stores it in u.username

    exists = 0;                        // assume username does not exist yet
    f = fopen("users.txt", "r");       // opens users.txt for reading
    if (f != NULL) {                   // only proceed if the file was opened successfully
        while (fgets(line, sizeof(line), f)) {           // reads the file one line at a time
            if (lineStartsWith(line, "Username: ")) {    // checks if this line is a username line
                extractValue(line, "Username: ", fileUser);          // extracts the username from the line
                if (strcmp(fileUser, u.username) == 0) {             // checks if it matches the new username
                    exists = 1;        // username already taken, set flag to 1
                    break;             // stop searching, no need to read further
                }
            }
        }
        fclose(f);    // closes the file after reading
    }

    if (exists == 1) {                                                              // if username was already found
        printf("Username already exists. Please choose a different username.\n\n"); // tell the user
        return;                                                                     // exit the function early
    }

    printf("Enter email: ");      // asks the user to type their email
    scanf("%s", u.email);         // reads and stores the email

    printf("Enter password: ");   // asks the user to type a password
    scanf("%s", u.password);      // reads and stores the password

    f = fopen("users.txt", "a");    // opens users.txt in append mode (adds to end without erasing)
    if (f == NULL) {                // if the file couldn't be opened
        printf("Error: Could not open users file.\n\n");    // print an error message
        return;                                             // exit the function early
    }

    fprintf(f, "Username: %s\n", u.username);      // writes the username to the file
    fprintf(f, "Email: %s\n",    u.email);         // writes the email to the file
    fprintf(f, "Password: %s\n\n", u.password);    // writes the password and a blank line after
    fclose(f);                                     // closes the file after writing

    printf("\nRegistration successful! You can now login.\n\n");    // tells the user registration worked
}

// handles logging in with an existing account, returns 1 if successful, 0 if failed
int loginUser() {
    struct User u;          // creates a User variable to hold the login attempt data
    char line[200];         // temporary storage for each line read from the file
    char fileUser[100];     // temporary storage for a username read from the file
    char filePass[100];     // temporary storage for a password read from the file
    int found;              // flag: 1 if the username was found in the file, 0 if not
    int passMatch;          // flag: 1 if the password matched, 0 if not
    FILE *f;                // file pointer for opening the users file

    printf("\n--------------------------------------------------\n");    // prints a divider
    printf("Login\n");                                                    // prints the section title
    printf("--------------------------------------------------\n");       // prints another divider

    printf("Enter username: ");     // asks the user to type their username
    scanf("%s", u.username);        // reads and stores the username

    printf("Enter password: ");     // asks the user to type their password
    scanf("%s", u.password);        // reads and stores the password

    f = fopen("users.txt", "r");    // opens users.txt for reading
    if (f == NULL) {                // if the file doesn't exist yet (no users registered)
        printf("No user accounts found. Please register first.\n\n");    // inform the user
        return 0;                                                        // return 0 meaning login failed
    }

    found     = 0;    // assume username not found yet
    passMatch = 0;    // assume password doesn't match yet

    while (fgets(line, sizeof(line), f)) {           // reads the file one line at a time
        if (lineStartsWith(line, "Username: ")) {    // checks if this line contains a username
            extractValue(line, "Username: ", fileUser);         // extracts the username from the line
            if (strcmp(fileUser, u.username) == 0) {            // checks if it matches what the user typed
                found = 1;                                      // username was found, set flag to 1
                fgets(line, sizeof(line), f);                   // skips the Email line (we don't need it)
                fgets(line, sizeof(line), f);                   // reads the next line which should be the Password line
                if (lineStartsWith(line, "Password: ")) {       // confirms this line is the password line
                    extractValue(line, "Password: ", filePass); // extracts the password from the line
                    if (strcmp(u.password, filePass) == 0) {    // compares typed password with stored password
                        passMatch = 1;                          // passwords match, set flag to 1
                    }
                }
                break;    // stop reading the file, we already found what we needed
            }
        }
    }

    fclose(f);    // closes the file after reading

    if (found == 0) {                                              // if username was never found
        printf("Username not found. Please register first.\n\n"); // tell the user
        return 0;                                                  // return 0 meaning login failed
    }

    if (passMatch == 0) {                      // if password did not match
        printf("Incorrect password.\n\n");     // tell the user
        return 0;                              // return 0 meaning login failed
    }

    printf("Login successful! Welcome, %s.\n\n", u.username);    // greet the user by name
    return 1;                                                     // return 1 meaning login succeeded
}

// prints personalized health guidance based on severity and BMI
void showGuidance(int severity, float bmi) {
    printf("\nObservation & Guidance:\n");    // prints the section heading

    if (severity == 1) {                                                  // if severity is mild
        printf("1. Mild condition: Start with rest and physiotherapy.\n");
    } else if (severity == 2) {                                           // if severity is moderate
        printf("1. Moderate condition: Regular physiotherapy is needed.\n");
    } else {                                                              // if severity is severe
        printf("1. Severe condition: Consult a spinal specialist quickly.\n");
    }

    if (bmi < 18.0) {                                                          // if BMI is underweight
        printf("2. Low BMI may reduce muscle support for the spine.\n");
        printf("3. Improve nutrition and core-strength exercises.\n");
    } else if (bmi < 25.0) {                                                   // if BMI is normal
        printf("2. BMI is normal. Continue your spine-safe routine.\n");
        printf("3. Maintain posture and do regular stretching.\n");
    } else if (bmi < 30.0) {                                                   // if BMI is overweight
        printf("2. Higher BMI increases pressure on lumbar discs.\n");
        printf("3. Start gradual weight loss and low-impact exercise.\n");
    } else {                                                                   // if BMI is obese
        printf("2. High BMI strongly increases PLID strain and pain risk.\n");
        printf("3. Weight reduction should be part of your treatment plan.\n");
    }

    if (severity == 3 && bmi >= 30.0) {           // if both severity and BMI are at their worst
        printf("4. Severe PLID + high BMI: urgent specialist follow-up advised.\n");
    } else if (severity >= 2 && bmi >= 25.0) {    // if moderate/severe with high BMI
        printf("4. Moderate/Severe PLID + high BMI: strict rehab and weight control needed.\n");
    } else {                                       // all other combinations
        printf("4. Continue follow-up and monitor pain changes weekly.\n");
    }

    // these tips apply to every patient regardless of severity or BMI
    printf("5. Avoid lifting heavy objects and sudden bending or twisting.\n");
    printf("6. Use proper sitting posture. Avoid long sitting without breaks.\n");
    printf("7. Do daily gentle walking and approved back exercises.\n");
    printf("8. Sleep on a firm mattress and avoid very soft cushions.\n");
    printf("9. Seek urgent care if you notice leg weakness, numbness, or bladder issues.\n");

    if (severity == 1) {                                              // follow-up time for mild cases
        printf("10. Re-check in 4-6 weeks if pain does not improve.\n\n");
    } else if (severity == 2) {                                       // follow-up time for moderate cases
        printf("10. Re-check in 2-4 weeks and track your pain score daily.\n\n");
    } else {                                                          // follow-up reminder for severe cases
        printf("10. Keep all specialist appointments and follow treatment strictly.\n\n");
    }
}

// collects patient information, saves it to file, and shows guidance
void addPatient() {
    struct Patient p;    // creates a Patient variable to hold all the entered data
    FILE *f;             // file pointer for opening the patient data file
    int inches;          // stores the inches part of the patient's height separately

    printf("\n--------------------------------------------------\n");    // prints a divider
    printf("Add New Patient\n");                                          // prints the section title
    printf("--------------------------------------------------\n");       // prints another divider

    printf("Username (e.g. john123): ");    // asks for a username
    scanf("%s", p.username);               // reads and stores the username

    printf("Full Name: ");                 // asks for the full name
    scanf(" ");                            // flushes the leftover newline from the input buffer
    fgets(p.name, 100, stdin);             // reads the full name including spaces

    printf("Age: ");                       // asks for age
    scanf("%d", &p.age);                   // reads and stores age as a whole number

    printf("Gender (M/F): ");              // asks for gender
    scanf("%s", p.gender);                 // reads and stores gender

    printf("Disc Level (e.g. L1-L2, L4-L5): ");    // asks for the affected disc level
    scanf("%s", p.discLevel);                       // reads and stores the disc level

    printf("Severity (1 = Mild, 2 = Moderate, 3 = Severe): ");    // asks for severity number
    scanf("%d", &p.severity);                                      // reads and stores severity as a number

    printf("Height - Feet: ");         // asks for the feet part of height
    scanf("%f", &p.height);            // reads and stores feet as a decimal number

    printf("Height - Inches: ");       // asks for the inches part of height
    scanf("%d", &inches);              // reads and stores inches as a whole number

    printf("Weight in kg: ");          // asks for weight
    scanf("%f", &p.weight);            // reads and stores weight in kilograms

    p.heightMeters = (p.height * 12 + inches) * 0.0254;           // converts feet and inches to meters
    p.bmi          = p.weight / (p.heightMeters * p.heightMeters); // calculates BMI using weight / height squared

    if (p.severity == 1) {                        // if severity number is 1
        strcpy(p.severityText, "Mild");           // store the word "Mild" in severityText
    } else if (p.severity == 2) {                 // if severity number is 2
        strcpy(p.severityText, "Moderate");       // store the word "Moderate"
    } else {                                      // if severity number is anything else (3)
        strcpy(p.severityText, "Severe");         // store the word "Severe"
    }

    if (p.bmi < 18.0) {                           // if BMI is below 18
        strcpy(p.bmiText, "Underweight");         // label it as Underweight
    } else if (p.bmi < 25.0) {                    // if BMI is between 18 and 24.9
        strcpy(p.bmiText, "Normal");              // label it as Normal
    } else if (p.bmi < 30.0) {                    // if BMI is between 25 and 29.9
        strcpy(p.bmiText, "Overweight");          // label it as Overweight
    } else {                                      // if BMI is 30 or above
        strcpy(p.bmiText, "Obese");               // label it as Obese
    }

    f = fopen("patient-data.txt", "a");    // opens the patient data file in append mode
    if (f == NULL) {                       // if the file couldn't be opened
        printf("Error: Could not open data file for writing.\n\n");    // print error
        return;                                                        // exit the function early
    }

    fprintf(f, "Username: %s\n",     p.username);        // writes username to file
    fprintf(f, "Name: %s\n",         p.name);            // writes full name to file
    fprintf(f, "------ General Stats ------\n");          // writes a separator line to file
    fprintf(f, "Age: %d\n",          p.age);             // writes age to file
    fprintf(f, "Gender: %s\n",       p.gender);          // writes gender to file
    fprintf(f, "Disc Level: %s\n",   p.discLevel);       // writes disc level to file
    fprintf(f, "Severity: %s\n",     p.severityText);    // writes severity text to file
    fprintf(f, "BMI: %.2f (%s)\n\n", p.bmi, p.bmiText); // writes BMI value and category, then a blank line
    fclose(f);                                           // closes the file after writing

    printf("\nPatient data saved successfully!\n");                                   // confirms save
    printf("Your Username is: %s (Use this to view your data later)\n", p.username); // reminds user of their username

    showGuidance(p.severity, p.bmi);    // calls the guidance function to show health tips
}

// searches for and displays a patient's record by username
void searchRecord() {
    char username[100];     // stores the username typed by the user
    char fileUser[100];     // temporary storage for usernames read from the file
    char line[200];         // temporary storage for each line read from the file
    int  found;             // flag: 1 if the record was found, 0 if not
    int  inRecord;          // flag: 1 if we are currently inside the matching record, 0 if not
    FILE *f;                // file pointer for opening the patient data file

    printf("\n--------------------------------------------------\n");    // prints a divider
    printf("Search Your Records\n");                                      // prints the section title
    printf("--------------------------------------------------\n");       // prints another divider

    printf("Enter your Username: ");    // asks the user to type their username
    scanf("%s", username);              // reads and stores the username

    f = fopen("patient-data.txt", "r");    // opens the patient data file for reading
    if (f == NULL) {                       // if the file doesn't exist yet
        printf("No patient records found.\n\n");    // tell the user
        return;                                     // exit the function early
    }

    found    = 0;    // assume record not found yet
    inRecord = 0;    // assume we are not inside any record yet

    while (fgets(line, sizeof(line), f)) {              // reads the file one line at a time
        if (lineStartsWith(line, "Username: ")) {       // if this line is a username line
            extractValue(line, "Username: ", fileUser); // extract the username from the line
            if (strcmp(fileUser, username) == 0) {      // if it matches the searched username
                found    = 1;                           // mark as found
                inRecord = 1;                           // we are now inside the matching record
                printf("\n------ Your Record ------\n");// print a header before showing the record
            } else {                                    // if it belongs to a different patient
                inRecord = 0;                           // we are no longer inside the matching record
            }
        }
        if (inRecord == 1) {          // if we are currently inside the matching record
            printf("%s", line);       // print this line to the screen
        }
    }

    fclose(f);    // closes the file after reading

    if (found == 0) {                                               // if the username was never found
        printf("No record found with username: %s\n", username);   // tell the user
        printf("Please check your username and try again.\n\n");    // suggest they check their username
    } else {                                                        // if the record was found and printed
        printf("--------------------------------------------------\n\n");    // print a closing divider
    }
}

// updates a specific field in a patient's record
void updateRecord() {
    char username[100];     // stores the username of the record to update
    char fileUser[100];     // temporary storage for usernames read from the file
    char newValue[100];     // stores the new value the user wants to set
    char line[200];         // temporary storage for each line read from the file
    char field;             // stores the letter choice of which field to update (N, A, G, D, S)
    int  found;             // flag: 1 if the username was found, 0 if not
    int  inRecord;          // flag: 1 if we are currently inside the matching record, 0 if not
    FILE *f;                // file pointer for the original patient data file
    FILE *temp;             // file pointer for the temporary file used during update

    printf("\n--------------------------------------------------\n");    // prints a divider
    printf("Update Patient Record\n");                                    // prints section title
    printf("--------------------------------------------------\n");       // prints another divider

    printf("Enter username to update: ");    // asks for the username whose record to update
    scanf("%s", username);                   // reads and stores the username

    printf("Which field to update?\n");                                         // shows the prompt
    printf("(N = Name, A = Age, G = Gender, D = Disc Level, S = Severity): "); // shows available options
    scanf(" %c", &field);                    // reads a single character (space before %c flushes leftover newline)

    if (field >= 'a' && field <= 'z') {      // if the user typed a lowercase letter
        field = field - 'a' + 'A';          // convert it to uppercase so both cases work
    }

    printf("Enter new value: ");    // asks for the new value to replace the old one
    scanf("%s", newValue);          // reads and stores the new value

    f    = fopen("patient-data.txt", "r");    // opens the original file for reading
    temp = fopen("temp.txt", "w");            // creates a temporary file for writing the updated content

    if (f == NULL || temp == NULL) {                   // if either file couldn't be opened
        printf("Error: Could not open files.\n\n");    // print error message
        if (f    != NULL) fclose(f);                   // close original file if it was opened
        if (temp != NULL) fclose(temp);                // close temp file if it was opened
        return;                                        // exit the function early
    }

    found    = 0;    // assume username not found yet
    inRecord = 0;    // assume we are not inside any record yet

    while (fgets(line, sizeof(line), f)) {               // reads the file one line at a time
        if (lineStartsWith(line, "Username: ")) {        // if this line is a username line
            extractValue(line, "Username: ", fileUser);  // extract the username from the line
            if (strcmp(fileUser, username) == 0) {       // if it matches the target username
                found    = 1;                            // mark as found
                inRecord = 1;                            // we are now inside the matching record
            } else {                                     // if it belongs to a different patient
                inRecord = 0;                            // we are no longer inside the matching record
            }
            fprintf(temp, "%s", line);                   // always write the username line unchanged
        }
        else if (inRecord == 1 && lineStartsWith(line, "Name: ")       && field == 'N') {
            fprintf(temp, "Name: %s\n", newValue);       // replace old name with new value
        }
        else if (inRecord == 1 && lineStartsWith(line, "Age: ")        && field == 'A') {
            fprintf(temp, "Age: %s\n", newValue);        // replace old age with new value
        }
        else if (inRecord == 1 && lineStartsWith(line, "Gender: ")     && field == 'G') {
            fprintf(temp, "Gender: %s\n", newValue);     // replace old gender with new value
        }
        else if (inRecord == 1 && lineStartsWith(line, "Disc Level: ") && field == 'D') {
            fprintf(temp, "Disc Level: %s\n", newValue); // replace old disc level with new value
        }
        else if (inRecord == 1 && lineStartsWith(line, "Severity: ")   && field == 'S') {
            fprintf(temp, "Severity: %s\n", newValue);   // replace old severity with new value
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
    } else {                                        // if the username was not found
        remove("temp.txt");                         // deletes the unused temp file
        printf("Username not found.\n\n");          // tells the user the username didn't exist
    }
}

// deletes a patient's entire record from the file
void deleteRecord() {
    char username[100];     // stores the username of the record to delete
    char fileUser[100];     // temporary storage for usernames read from the file
    char line[200];         // temporary storage for each line read from the file
    int  found;             // flag: 1 if the username was found, 0 if not
    int  inRecord;          // flag: 1 if we are currently inside the record being deleted
    FILE *f;                // file pointer for the original patient data file
    FILE *temp;             // file pointer for the temporary file

    printf("\n--------------------------------------------------\n");    // prints a divider
    printf("Delete Patient Record\n");                                    // prints the section title
    printf("--------------------------------------------------\n");       // prints another divider

    printf("Enter username to delete: ");    // asks for the username to delete
    scanf("%s", username);                   // reads and stores the username

    f    = fopen("patient-data.txt", "r");    // opens the original file for reading
    temp = fopen("temp.txt", "w");            // creates a temporary file for writing

    if (f == NULL || temp == NULL) {                   // if either file couldn't be opened
        printf("Error: Could not open files.\n\n");    // print error message
        if (f    != NULL) fclose(f);                   // close original file if it was opened
        if (temp != NULL) fclose(temp);                // close temp file if it was opened
        return;                                        // exit the function early
    }

    found    = 0;    // assume username not found yet
    inRecord = 0;    // assume we are not inside any record yet

    while (fgets(line, sizeof(line), f)) {               // reads the file one line at a time
        if (lineStartsWith(line, "Username: ")) {        // if this line is a username line
            extractValue(line, "Username: ", fileUser);  // extract the username from the line
            if (strcmp(fileUser, username) == 0) {       // if it matches the username to delete
                found    = 1;                            // mark as found
                inRecord = 1;                            // we are now inside the record to delete
                printf("Found record for %s. Deleting...\n", username);    // inform the user
            } else {                                     // if it belongs to a different patient
                inRecord = 0;                            // we are no longer inside the record to delete
            }
        }
        if (inRecord == 0) {               // if we are NOT inside the record being deleted
            fprintf(temp, "%s", line);     // copy this line to the temp file (keep it)
        }
        // if inRecord == 1, we do nothing — the line is skipped and effectively deleted
    }

    fclose(f);       // closes the original file
    fclose(temp);    // closes the temp file

    if (found == 1) {                               // if the username was found and deleted
        remove("patient-data.txt");                 // deletes the original file
        rename("temp.txt", "patient-data.txt");     // renames temp file to replace the original
        printf("Record deleted successfully!\n\n"); // confirms the deletion worked
    } else {                                        // if the username was not found
        remove("temp.txt");                         // deletes the unused temp file
        printf("Username not found.\n\n");          // tells the user the username didn't exist
    }
}

// displays a list of recommended doctors
void showDoctors() {
    printf("\n--------------------------------------------------\n");    // prints a divider
    printf("Doctor Recommendations\n");                                   // prints the section title
    printf("--------------------------------------------------\n");       // prints another divider
    printf("1. Dr. Shah Alam, MBBS (BSMRMU)\n");     // prints first doctor
    printf("2. Dr. Nazrul Islam, MBBS (DMC)\n");      // prints second doctor
    printf("3. Dr. Irin Akter, MBBS (RMC)\n\n");     // prints third doctor with blank line after
}

// displays information about PLID and about this project
void showAbout() {
    printf("\n--------------------------------------------------\n");    // prints a divider
    printf("About PLID\n");                                               // prints the section title
    printf("--------------------------------------------------\n");       // prints another divider
    printf("PLID means Prolapsed Lumbar Intervertebral Disc.\n");                                          // explains what PLID stands for
    printf("This happens when a disc in the lower back moves out of place and presses nearby nerves.\n\n");// explains what PLID is

    printf("Common Causes:\n");                                        // heading for causes section
    printf("1. Repeated heavy lifting with poor posture\n");           // cause 1
    printf("2. Sudden twisting or bending movements\n");               // cause 2
    printf("3. Age-related disc wear and tear\n");                     // cause 3
    printf("4. Back injury or accident\n\n");                          // cause 4

    printf("Risk Factors:\n");                                         // heading for risk factors section
    printf("1. Overweight or obesity\n");                              // risk factor 1
    printf("2. Long duration of sitting\n");                           // risk factor 2
    printf("3. Weak core and back muscles\n");                         // risk factor 3
    printf("4. Smoking and low physical activity\n");                  // risk factor 4
    printf("5. Family history of spine problems\n\n");                 // risk factor 5

    printf("--------------------------------------------------\n");    // prints a divider
    printf("About PLID Connect\n");                                    // prints sub-section title
    printf("--------------------------------------------------\n");    // prints another divider
    printf("PLID Connect is a solo project developed & maintained by Safwan Safat.\n");    // credits the developer
    printf("It takes the user's data with consent and gives doctor recommendations,\n");   // describes what the app does
    printf("guidance, and healthcare tips based on the data.\n\n");                        // continues the description
}

// displays the main menu options
void showMenu() {
    printf("--------------------------------------------------\n");    // prints a divider
    printf("Welcome to PLID Connect\n");                               // prints the app name
    printf("--------------------------------------------------\n");    // prints another divider
    printf("1. Add a New Patient\n");          // menu option 1
    printf("2. Search Your Records\n");        // menu option 2
    printf("3. Update Patient Record\n");      // menu option 3
    printf("4. Delete Patient Record\n");      // menu option 4
    printf("5. Doctor Recommendations\n");     // menu option 5
    printf("6. About PLID & PLID Connect\n"); // menu option 6
    printf("7. Exit\n");                       // menu option 7
    printf("--------------------------------------------------\n");    // prints a closing divider
}

// the main function — this is where the program starts running
int main() {
    int choice;          // stores the user's menu choice after login
    int authChoice;      // stores the user's choice on the login/register screen
    int loggedIn = 0;    // flag: starts at 0 (not logged in), becomes 1 after successful login

    while (loggedIn == 0) {    // keep showing the authentication screen until the user logs in
        printf("\n--------------------------------------------------\n");    // prints a divider
        printf("PLID Connect - Authentication\n");                            // prints the screen title
        printf("--------------------------------------------------\n");       // prints another divider
        printf("Manage your PLID condition with personalized\n");              // prints app description line 1
        printf("health guidance and professional recommendations.\n");         // prints app description line 2
        printf("--------------------------------------------------\n");       // prints another divider
        printf("1. Register\n");     // shows register option
        printf("2. Login\n");        // shows login option
        printf("3. Exit\n");         // shows exit option
        printf("--------------------------------------------------\n");       // prints a closing divider
        printf("Your choice: ");     // prompts the user to pick
        scanf("%d", &authChoice);    // reads the user's number choice

        switch (authChoice) {        // checks what the user chose
            case 1:
                registerUser();      // calls the register function
                break;               // exits the switch after handling
            case 2:
                if (loginUser() == 1) {    // calls login and checks if it returned 1 (success)
                    loggedIn = 1;          // set loggedIn to 1 so the while loop ends
                }
                break;                     // exits the switch after handling
            case 3:
                printf("\nThank you for using PLID Connect. Goodbye!\n");    // farewell message
                return 0;                                                     // ends the program
            default:
                printf("\nInvalid choice. Please try again.\n");    // handles any other number typed
        }
    }

    while (1) {                   // infinite loop — keeps showing the main menu until user exits
        showMenu();               // displays the main menu
        printf("Your choice: ");  // prompts the user to pick
        scanf("%d", &choice);     // reads the user's menu choice

        switch (choice) {         // checks what the user chose
            case 1:
                addPatient();     // calls the add patient function
                break;            // exits the switch
            case 2:
                searchRecord();   // calls the search record function
                break;            // exits the switch
            case 3:
                updateRecord();   // calls the update record function
                break;            // exits the switch
            case 4:
                deleteRecord();   // calls the delete record function
                break;            // exits the switch
            case 5:
                showDoctors();    // calls the doctor recommendations function
                break;            // exits the switch
            case 6:
                showAbout();      // calls the about section function
                break;            // exits the switch
            case 7:
                printf("\nTake care & stay connected with PLID Connect.\n\n");    // farewell message
                return 0;         // ends the program
            default:
                printf("\nInvalid choice.\n");    // handles any other number typed
        }
    }

    return 0;    // program ends here (this line is never actually reached due to the infinite loop above)
}