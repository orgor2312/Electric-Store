#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "users.h"
#include "logger.h" 


#define USERS_FILE "users.txt"


void initializeUsers()
{
    FILE *file = fopen(USERS_FILE, "r");
    if (!file)    
    {
        file = fopen(USERS_FILE, "w");
        if (!file)
        {
            perror("Error creating users file");
            exit(1);
        }

        const char *adminData = "admin,admin,System_Manager,3\n";
        fprintf(file, "%s", adminData);

        printAndLog("Admin user created: Username: admin, Password: admin\n");
        logAction("Initialize Users", "Admin user created successfully");

        fclose(file);
    }
    else
    {
        fclose(file);
    }
}

int authenticateUser(User *loggedInUser)
{
    FILE *file = fopen(USERS_FILE, "r");
    if (!file)
    {
        perror("Error opening users file");
        return 0;
    }

    int attempts = 0;
    char username[USERNAME_LENGTH], password[PASSWORD_LENGTH];

    while (attempts < 3)
    {
        printAndLog("Enter username: ");
        scanf("%s", username);
        printAndLog("Enter password: ");
        scanf("%s", password);

        char fileUsername[USERNAME_LENGTH], filePassword[PASSWORD_LENGTH], fullname[FULLNAME_LENGTH];
        int level;

        rewind(file); 

        while (fscanf(file, "%[^,],%[^,],%[^,],%d\n", fileUsername, filePassword, fullname, &level) == 4)
        {
            if (strcmp(fileUsername, username) == 0 && strcmp(filePassword, password) == 0)
            {
                
                strcpy(loggedInUser->username, fileUsername);
                strcpy(loggedInUser->password, filePassword);
                strcpy(loggedInUser->fullname, fullname);
                loggedInUser->level = level;

                logAction("Login", "Successful login");
                fclose(file);
                return 1;
            }
        }

        attempts++;
        printAndLog("Invalid credentials. You have %d attempt(s) remaining.\n", 3 - attempts);
        logAction("Login", "Failed login attempt");
    }

    fclose(file);
    printAndLog("Maximum login attempts exceeded. Access denied.\n");
    logAction("Login", "Maximum login attempts exceeded");
    return 0;
}


void addUser(const char *username, const char *password, int level, const char *fullname)
{
    if (strlen(username) >= USERNAME_LENGTH || strlen(password) >= PASSWORD_LENGTH || strlen(fullname) >= FULLNAME_LENGTH)
    {
        printAndLog("Error: Input exceeds allowed length.\n");
        logAction("Add User", "Failed to add user due to input length");
        return;
    }

    FILE *file = fopen(USERS_FILE, "a");
    if (!file)
    {
        perror("Error opening users file");
        return;
    }

    fprintf(file, "%s,%s,%s,%d\n", username, password, fullname, level);

    fclose(file);
    printAndLog("User %s added successfully.\n", username);
    logAction("Add User", "New user added successfully");
}


void viewUsers()
{
    FILE *file = fopen(USERS_FILE, "r");
    if (!file)
    {
        printAndLog("Error opening users file.\n");
        return;
    }

    printAndLog("User List:\n");
    printAndLog("Username       | Full Name              | Level\n");
    printAndLog("-----------------------------------------------\n");

    char username[USERNAME_LENGTH], password[PASSWORD_LENGTH], fullname[FULLNAME_LENGTH];
    int level;

    while (fscanf(file, "%[^,],%[^,],%[^,],%d\n", username, password, fullname, &level) == 4)
    {
        printAndLog("%-15s | %-20s | %d\n", username, fullname, level);
    }

    fclose(file);
}


void editUser()
{
    char username[USERNAME_LENGTH];
    printAndLog("Enter the username of the user to edit: ");
    scanf("%s", username);

    FILE *file = fopen(USERS_FILE, "r");
    FILE *tempFile = fopen("temp_users.txt", "w");

    if (!file || !tempFile)
    {
        printAndLog("Error opening users file.\n");
        return;
    }

    char fileUsername[USERNAME_LENGTH], password[PASSWORD_LENGTH], fullname[FULLNAME_LENGTH];
    int level;
    int found = 0;

    while (fscanf(file, "%[^,],%[^,],%[^,],%d\n", fileUsername, password, fullname, &level) == 4)
    {
        if (strcmp(fileUsername, username) == 0)
        {
            found = 1;
            printAndLog("Editing user: %s\n", username);

            printAndLog("Enter new full name: ");
            scanf(" %[^\n]", fullname);
            printAndLog("Enter new access level (1: Basic, 2: Manager, 3: Admin): ");
            scanf("%d", &level);
        }

        fprintf(tempFile, "%s,%s,%s,%d\n", fileUsername, password, fullname, level);
    }

    fclose(file);
    fclose(tempFile);

    remove(USERS_FILE);
    rename("temp_users.txt", USERS_FILE);

    if (found)
    {
        printAndLog("User updated successfully.\n");
    }
    else
    {
        printAndLog("User not found.\n");
    }
}

void deleteUser()
{
    char username[USERNAME_LENGTH];
    printAndLog("Enter the username of the user to delete: ");
    scanf("%s", username);

    FILE *file = fopen(USERS_FILE, "r");
    FILE *tempFile = fopen("temp_users.txt", "w");

    if (!file || !tempFile)
    {
        printAndLog("Error opening users file.\n");
        return;
    }

    char fileUsername[USERNAME_LENGTH], password[PASSWORD_LENGTH], fullname[FULLNAME_LENGTH];
    int level;
    int found = 0;

    while (fscanf(file, "%[^,],%[^,],%[^,],%d\n", fileUsername, password, fullname, &level) == 4)
    {
        if (strcmp(fileUsername, username) == 0)
        {
            found = 1;
            continue; 
        }
        fprintf(tempFile, "%s,%s,%s,%d\n", fileUsername, password, fullname, level);
    }

    fclose(file);
    fclose(tempFile);

    remove(USERS_FILE);
    rename("temp_users.txt", USERS_FILE);

    if (found)
    {
        printAndLog("User deleted successfully.\n");
    }
    else
    {
        printAndLog("User not found.\n");
    }
}
