#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "items.h"
#include "logger.h"
#include <ctype.h>

#define ITEMS_FILE "items.txt"

Item *head = NULL;

void initializeItems()
{
    loadItemsFromFile();
}

void addItem()
{
    Item *newItem = (Item *)malloc(sizeof(Item));
    if (!newItem)
    {
        printAndLog("Memory allocation failed!\n");
        return;
    }

    printAndLog("Enter Serial Number: ");
    scanf("%d", &newItem->serialNumber);
    printAndLog("Enter Brand Name: ");
    scanf("%s", newItem->BrandName);
    printAndLog("Enter Product Name: ");
    scanf("%s", newItem->ProductName);

    printAndLog("Enter Price: ");
    while (1)
    {
        if (scanf("%f", &newItem->Price) == 1)
        {
            break; 
        }
        else
        {
            printAndLog("Invalid input. Enter a valid number: ");
            while (getchar() != '\n')
                ; 
        }
    }

    printAndLog("Enter Availablity (0 or 1): ");
    while (1)
    {
        if (scanf("%d", &newItem->Availablity) == 1 &&
            (newItem->Availablity == 0 || newItem->Availablity == 1))
        {
            break; 
        }
        else
        {
            printAndLog("Invalid input. Enter 0 or 1: ");
            while (getchar() != '\n')
                ;
        }
    }

    char date[11];
    do
    {
        printAndLog("Enter Date (DD-MM-YYYY): ");
        scanf("%s", date);
        if (!validateDate(date))
        {
            printAndLog("Invalid date format. Try again.\n");
        }
    } while (!validateDate(date));
    strcpy(newItem->date, date);

    newItem->next = head;
    head = newItem;

    saveItemsToFile();
    printAndLog("Item added successfully!\n");
}

void viewItems()
{
    Item *current = head;
    if (!current)
    {
        printAndLog("No items available.\n");
        return;
    }

    printAndLog("Items:\n");
    printAndLog("Serial No. | Brand Name        | Product Name       | Price  | Availablity | Date\n");
    printAndLog("-------------------------------------------------------------------\n");
    while (current)
    {
        printAndLog("%-10d | %-12s | %-12s | %-8.2f | %-7d | %-10s\n",
                    current->serialNumber, current->BrandName, current->ProductName,
                    current->Price, current->Availablity, current->date);
        current = current->next;
    }
}


int validateDate(const char *date)
{
    if (strlen(date) != 10)
    {
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        if ((i == 2 || i == 5) && date[i] != '-')
        {
            return 0;
        }
        else if ((i != 2 && i != 5) && !isdigit(date[i]))
        {
            return 0;
        }
    }

    int day = atoi(date);
    int month = atoi(date + 3);
    int year = atoi(date + 6);

    if (year < 1900 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        return 0;
    }

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
    {
        return 0;
    }
    if (month == 2)
    {
        int isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (day > (isLeap ? 29 : 28))
        {
            return 0;
        }
    }

    return 1;
}

void saveItemsToFile()
{
    if (!head || !head->next)
    {
    }
    else
    {
        Item *i, *j;
        for (i = head; i && i->next; i = i->next)
        {
            for (j = i->next; j; j = j->next)
            {
                if (i->serialNumber > j->serialNumber)
                {
                    int tempSerial = i->serialNumber;
                    i->serialNumber = j->serialNumber;
                    j->serialNumber = tempSerial;

                    float tempPrice = i->Price;
                    i->Price = j->Price;
                    j->Price = tempPrice;

                    int tempAvail = i->Availablity;
                    i->Availablity = j->Availablity;
                    j->Availablity = tempAvail;

                    char tempBrand[50];
                    char tempProd[50];
                    char tempDate[11];

                    strcpy(tempBrand, i->BrandName);
                    strcpy(i->BrandName, j->BrandName);
                    strcpy(j->BrandName, tempBrand);

                    strcpy(tempProd, i->ProductName);
                    strcpy(i->ProductName, j->ProductName);
                    strcpy(j->ProductName, tempProd);

                    strcpy(tempDate, i->date);
                    strcpy(i->date, j->date);
                    strcpy(j->date, tempDate);
                }
            }
        }
    }

    FILE *file = fopen(ITEMS_FILE, "w");
    if (!file)
    {
        perror("Error opening items file");
        return;
    }

    Item *current = head;
    while (current)
    {
        fwrite(current, sizeof(Item), 1, file);
        current = current->next;
    }

    fclose(file);
    printAndLog("Items saved to file successfully (sorted by Serial Number).\n");
}

void loadItemsFromFile()
{
    FILE *file = fopen(ITEMS_FILE, "r");
    if (!file)
    {
        return; 
    }

    Item temp;
    while (fread(&temp, sizeof(Item), 1, file))
    {
        Item *newItem = (Item *)malloc(sizeof(Item));
        if (!newItem)
        {
            printAndLog("Memory allocation failed!\n");
            fclose(file);
            return;
        }
        *newItem = temp;
        newItem->next = head;
        head = newItem;
    }

    fclose(file);
}

void updateItem()
{
    if (!head)
    {
        printAndLog("No items available to update.\n");
        return;
    }

    int serialNumber;
    printAndLog("Enter the Serial Number of the item to update: ");
    scanf("%d", &serialNumber);

    Item *current = head;
    while (current)
    {
        if (current->serialNumber == serialNumber)
        {
            printAndLog("Enter new details:\n");
            printAndLog("Brand Name: ");
            scanf("%s", current->BrandName);
            printAndLog("Product Name: ");
            scanf("%s", current->ProductName);
            printAndLog("Price: ");
            scanf("%f", &current->Price);
            printAndLog("Boolean Value (0 or 1): ");
            scanf("%d", &current->Availablity);
            printAndLog("Date (YYYY-MM-DD): ");
            scanf("%s", current->date);

            saveItemsToFile();
            printAndLog("Item updated successfully!\n");
            return;
        }
        current = current->next;
    }

    printAndLog("Item with Serial Number %d not found.\n", serialNumber);
}

void updateItemBySerialNumber(int serialNumber)
{
    if (!head)
    {
        printAndLog("No items available to update.\n");
        return;
    }

    Item *current = head;
    while (current)
    {
        if (current->serialNumber == serialNumber)
        {
            printAndLog("Editing item with Serial Number: %d\n", serialNumber);

            printAndLog("Enter new Brand Name: ");
            scanf("%s", current->BrandName);
            printAndLog("Enter new Product Name: ");
            scanf("%s", current->ProductName);
            printAndLog("Enter new Price: ");
            scanf("%f", &current->Price);
            printAndLog("Enter new Availability (0 or 1): ");
            scanf("%d", &current->Availablity);
            printAndLog("Enter new Date (DD-MM-YYYY): ");
            scanf("%s", current->date);

            saveItemsToFile();
            printAndLog("Item updated successfully.\n");
            return;
        }
        current = current->next;
    }

    printAndLog("Item with Serial Number %d not found.\n", serialNumber);
}

void deleteItem()
{
    if (!head)
    {
        printAndLog("No items available to delete.\n");
        return;
    }

    int serialNumber;
    printAndLog("Enter the Serial Number of the item to delete: ");
    scanf("%d", &serialNumber);

    Item *current = head;
    Item *prev = NULL;

    while (current)
    {
        if (current->serialNumber == serialNumber)
        {
            if (prev)
            {
                prev->next = current->next;
            }
            else
            {
                head = current->next;
            }
            free(current);
            saveItemsToFile();
            printAndLog("Item deleted successfully!\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printAndLog("Item with Serial Number %d not found.\n", serialNumber);
}
void searchItems()
{
    if (!head)
    {
        printAndLog("No items available to search.\n");
        return;
    }

    int choice;
    printAndLog("Search by:\n");
    printAndLog("1. Brand Name or Product Name\n");
    printAndLog("2. Price\n");
    printAndLog("3. Availability\n");
    printAndLog("4. Date\n");
    printAndLog("Enter your choice: ");
    scanf("%d", &choice);

    Item *current = head;
    int found = 0; 

    switch (choice)
    {
    case 1:
    { 
        char searchText[TEXT_FIELD_LENGTH];
        printAndLog("Enter text to search (Brand Name or Product Name): ");
        scanf(" %[^\n]s", searchText);

        for (int i = 0; searchText[i]; i++)
            searchText[i] = tolower(searchText[i]);

        printAndLog("Search Results:\n");
        printAndLog("Serial No. | Brand Name        | Product Name       | Price  | Availability | Date\n");
        printAndLog("---------------------------------------------------------------------------\n");

        while (current)
        {
            char brandLower[TEXT_FIELD_LENGTH], productLower[TEXT_FIELD_LENGTH];
            strcpy(brandLower, current->BrandName);
            strcpy(productLower, current->ProductName);

            for (int i = 0; brandLower[i]; i++)
                brandLower[i] = tolower(brandLower[i]);
            for (int i = 0; productLower[i]; i++)
                productLower[i] = tolower(productLower[i]);

            if (strstr(brandLower, searchText) || strstr(productLower, searchText))
            {
                found = 1;
                printAndLog("%-10d | %-16s | %-18s | %-6.2f | %-12d | %-10s\n",
                            current->serialNumber, current->BrandName, current->ProductName,
                            current->Price, current->Availablity, current->date);
            }
            current = current->next;
        }
        break;
    }

    case 2:
    { 
        float value;
        int priceChoice;

        printAndLog("Enter Price to search: ");
        scanf("%f", &value);

        printAndLog("Do you want to search for:\n");
        printAndLog("1. Items with Prices greater than or equal to the entered price\n");
        printAndLog("2. Items with Prices less than or equal to the entered price\n");
        printAndLog("Enter your choice: (1/2) ");
        scanf("%d", &priceChoice);

        printAndLog("Search Results:\n");
        printAndLog("Serial No. | Brand Name        | Product Name       | Price  | Availability | Date\n");
        printAndLog("---------------------------------------------------------------------------\n");

        while (current)
        {
            if ((priceChoice == 1 && current->Price >= value) || (priceChoice == 2 && current->Price <= value))
            {
                found = 1;
                printAndLog("%-10d | %-16s | %-18s | %-6.2f | %-12d | %-10s\n",
                    current->serialNumber, current->BrandName, current->ProductName,
                    current->Price, current->Availablity, current->date);
            }
            current = current->next;
        }
        break;
    }


    case 3:
    {
        int boolValue;
        printAndLog("Enter Availability to search (0 or 1): ");
        scanf("%d", &boolValue);

        printAndLog("Search Results:\n");
        printAndLog("Serial No. | Brand Name        | Product Name       | Price  | Availability | Date\n");
        printAndLog("---------------------------------------------------------------------------\n");

        while (current)
        {
            if (current->Availablity == boolValue)
            {
                found = 1;
                printAndLog("%-10d | %-16s | %-18s | %-6.2f | %-12d | %-10s\n",
                            current->serialNumber, current->BrandName, current->ProductName,
                            current->Price, current->Availablity, current->date);
            }
            current = current->next;
        }
        break;
    }

    case 4:
    { 
        char date[11];
        int dateChoice;

        printAndLog("Enter Date to search (DD-MM-YYYY): ");
        scanf("%s", date);

        printAndLog("Do you want to search for:\n");
        printAndLog("1. Items with Dates greater than or equal to the entered date\n");
        printAndLog("2. Items with Dates less than or equal to the entered date\n");
        printAndLog("Enter your choice: (1/2) ");
        scanf("%d", &dateChoice);

        printAndLog("Search Results:\n");
        printAndLog("Serial No. | Brand Name        | Product Name       | Price  | Availability | Date\n");
        printAndLog("---------------------------------------------------------------------------\n");

        while (current)
        {
            if ((dateChoice == 1 && strcmp(current->date, date) >= 0) || (dateChoice == 2 && strcmp(current->date, date) <= 0))
            {
                found = 1;
                printAndLog("%-10d | %-16s | %-18s | %-6.2f | %-12d | %-10s\n",
                    current->serialNumber, current->BrandName, current->ProductName,
                    current->Price, current->Availablity, current->date);
            }
            current = current->next;
        }
        break;
    }

    default:
        printAndLog("Invalid choice.\n");
        return;
    }

    if (!found)
    {
        printAndLog("No matching items found.\n");
    }
    else
    {
        printAndLog("---------------------------------------------------------------------------\n");
    }
}
