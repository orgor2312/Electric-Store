/*
Assigned by:
Or Goraly #211571922
*/

#define _CRT_SECURE_NO_WARNINGS
#ifndef USERS_H
#define USERS_H

#define USERNAME_LENGTH 15
#define PASSWORD_LENGTH 15
#define FULLNAME_LENGTH 20

typedef struct
{
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    int level; 
    char fullname[FULLNAME_LENGTH];
} User;

void initializeUsers();
int authenticateUser(User *loggedInUser);
void addUser(const char *username, const char *password, int level, const char *fullname);
void viewUsers();
void editUser();
void deleteUser();
#endif
