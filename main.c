#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "users.h"
#include "items.h"
#include "logger.h"

void showMenu(int accessLevel);

int main()
{
    User loggedInUser;

    initializeUsers();
    initializeItems();

    printAndLog("Welcome to the Information Management System!\n");
    if (!authenticateUser(&loggedInUser))
    {
        printAndLog("Invalid credentials. Exiting.\n");
        logAction("System Access", "Failed login attempt");
        return 1;
    }

    printAndLog("Login successful! Welcome, %s.\n", loggedInUser.fullname);
    char logDetails[100];
    snprintf(logDetails, sizeof(logDetails), "User logged in: %s (Level %d)", loggedInUser.fullname, loggedInUser.level);
    logAction("System Access", logDetails);

    showMenu(loggedInUser.level);

    return 0;
}

void handleAddUser()
{
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    char fullname[FULLNAME_LENGTH];
    int level;

    printAndLog("Enter new user's username: ");
    scanf("%s", username);
    printAndLog("Enter new user's password: ");
    scanf("%s", password);
    printAndLog("Enter new user's full name: ");
    scanf(" %[^\n]", fullname); 
    printAndLog("Enter access level (1: Basic, 2: Manager, 3: Admin): ");
    while (scanf("%d", &level) != 1 || level < 1 || level > 3)
    {
        printAndLog("Invalid input. Enter a valid level (1, 2, or 3): ");
        while (getchar() != '\n')
            ; 
    }

    addUser(username, password, level, fullname);
}

void showMenu(int accessLevel)
{
    int choice;

    do
    {
        printAndLog("\nMenu:\n");
        printAndLog("1. Add Item\n");
        printAndLog("2. View Items\n");
        if (accessLevel >= 2)
        {
            printAndLog("3. Update Item\n");
            printAndLog("4. Delete Item\n");
        }
        printAndLog("5. Search Items\n");
        if (accessLevel == 3) 
        {
            printAndLog("6. Add User\n");
            printAndLog("7. View All Users\n");
            printAndLog("8. Edit User\n");
            printAndLog("9. Delete User\n");
        }
        printAndLog("0. Exit\n");
        printAndLog("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printAndLog("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n')
                ; 
            continue;
        }

        switch (choice)
        {
        case 1:
            addItem();
            break;
        case 2:
            viewItems();
            break;
        case 3:
            if (accessLevel >= 2)
            {
                updateItem();
            }
            else
            {
                printAndLog("Permission denied.\n");
            }
            break;
        case 4:
            if (accessLevel >= 2)
            {
                deleteItem();
            }
            else
            {
                printAndLog("Permission denied.\n");
            }
            break;
        case 5: 
            searchItems();
            if (accessLevel >= 2)
            {
                int editChoice;
                printAndLog("Do you want to edit an item? (1: Yes, 0: No): ");
                scanf("%d", &editChoice);

                if (editChoice == 1)
                {
                    int serialNumber;
                    printAndLog("Enter the Serial Number of the item to edit: ");
                    scanf("%d", &serialNumber);
                    updateItemBySerialNumber(serialNumber);
                }
            }
            else
            {
                printAndLog("Permission denied for editing items.\n");
            }
            break;

            break;
        case 6:
            if (accessLevel == 3)
            {
                handleAddUser();
            }
            else
            {
                printAndLog("Permission denied.\n");
            }
            break;
        case 7:
            if (accessLevel == 3)
            {
                viewUsers();
            }
            else
            {
                printAndLog("Permission denied.\n");
            }
            break;
        case 8:
            if (accessLevel == 3)
            {
                editUser();
            }
            else
            {
                printAndLog("Permission denied.\n");
            }
            break;
        case 9:
            if (accessLevel == 3)
            {
                deleteUser();
            }
            else
            {
                printAndLog("Permission denied.\n");
            }
            break;
        case 0:
            printAndLog("Exiting program. Goodbye!\n");
            break;
        default:
            printAndLog("Invalid choice. Try again.\n");
        }
    } while (choice != 0);
}
