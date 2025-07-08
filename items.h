#define _CRT_SECURE_NO_WARNINGS
#ifndef ITEMS_H
#define ITEMS_H

#define TEXT_FIELD_LENGTH 50

typedef struct Item
{
    int serialNumber;
    char BrandName[TEXT_FIELD_LENGTH];
    char ProductName[TEXT_FIELD_LENGTH];
    float Price; 
    int Availablity;   
    char date[11];      
    struct Item *next;  
} Item;

void initializeItems();
void addItem();
void viewItems();
void updateItem();
void deleteItem();
void searchItems();
void saveItemsToFile();
void loadItemsFromFile();
int validateDate(const char *date);
void updateItemBySerialNumber(int serialNumber);
#endif
