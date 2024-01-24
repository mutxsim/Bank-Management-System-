#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CREDENTIALS 100
#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50

// Data structures
typedef struct {
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char email[MAX_NAME_LENGTH];
    char socialSecurityNumber[MAX_NAME_LENGTH];
    char address[MAX_NAME_LENGTH];
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    int role; // 0 for bank employee, 1 for customer
} User;


typedef struct {
    char accountHolder[MAX_NAME_LENGTH];
    double balance;
} Account;

// Function prototypes
void displayMenu(int role);
void login(User *users, int *currentUser);
void createUser(User *users, int *numUsers);
void deposit(Account *accounts, int numAccounts);
void withdraw(Account *accounts, int numAccounts);
void viewBalance(Account *accounts, int numAccounts);
void accountSearch(Account *accounts, int numAccounts);
void saveAccountData(User *users, int numUsers, const char *filename);
void loadAccountData(Account *accounts, int *numAccounts, const char *filename);

// Admin-specific functionalities (placeholders)
void addCustomer(Account *accounts, int *numAccounts);
void removeCustomer(Account *accounts, int *numAccounts);
void sortAccounts(Account *accounts, int numAccounts);

int main() {
    User users[MAX_CREDENTIALS];
    Account accounts[MAX_ACCOUNTS];
    int numAccounts = 0;
    int currentUser = -1; // -1 indicates no user logged in

    // Load data from file (if available)
    loadAccountData(accounts, &numAccounts, "account_data.txt");

    // Initialize some dummy data (you can replace this with file I/O)
    strcpy(users[0].username, "employee");
    strcpy(users[0].password, "password");
    users[0].role = 0; // bank employee

    // Main program loop
    int choice;
    char yn;
    do {
        printf("------JAK Bank------\n");
        printf("Do you have an account? (y/n):");
        scanf("%c", &yn);
        
        switch (yn){
        case 'y':

            printf("1. Login as User\n2. Login as Admin\n3. Exit\n ");
            printf("___________\n");
            printf("\nEnter your choice:");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    login(users, &currentUser);
                    saveAccountData(accounts, numAccounts, "account_data.txt");

                    break;
                case 2:
                    login(users, &currentUser); // Admin login (for simplicity, using the same function)
                    if (currentUser != -1 && users[currentUser].role != 0) {
                        printf("Invalid credentials. Logging out.\n");
                        currentUser = -1;
                    }
                    saveAccountData(accounts, numAccounts, "account_data.txt");

                    break;
                case 3:
                    // Save data to file before exiting
                    saveAccountData(accounts, numAccounts, "account_data.txt");
                    printf("Exiting program.\n");
                    break;
                default:
                    printf("Invalid choice. Try again.\n");
            }
            break;
        
        case 'n':
        // User does not have an account
                printf("\nWould you like to create one (y/n):");
                char response2;
                scanf(" %c", &response2);
                switch (response2)
                {
                case 'n':
                    printf("Okay, Goodbye!");
                    return 0;

                case 'y':
                    createUser(users, &numAccounts);
                    break;
                
                }
                break;

        default:
            printf("Invalid choice. Try again.\n");
            break;
        }

        if (currentUser != -1) {
            // If a user is logged in, show the user-specific menu
            do {
                displayMenu(users[currentUser].role);
                printf("Enter your choice: ");
                scanf("%d", &choice);

                switch (choice) {
                    // User and admin functionalities
                    case 1:
                        if (users[currentUser].role == 0) {
                            deposit(accounts, numAccounts);
                        } else {
                            // Admin-specific functionality (e.g., view customer info)
                            viewBalance(accounts, numAccounts);
                        }
                        break;
                    case 2:
                        if (users[currentUser].role == 0) {
                            withdraw(accounts, numAccounts);
                        } else {
                            // Admin-specific functionality (e.g., add customer)
                            addCustomer(accounts, &numAccounts);
                        }
                        break;
                    case 3:
                        viewBalance(accounts, numAccounts);
                        break;
                    case 4:
                        if (users[currentUser].role == 0) {
                            printf("Invalid choice. Try again.\n");
                        } else {
                            // Admin-specific functionality (e.g., remove customer)
                            removeCustomer(accounts, &numAccounts);
                        }
                        break;
                    case 5:
                        if (users[currentUser].role == 0) {
                            printf("Invalid choice. Try again.\n");
                        } else {
                            // Admin-specific functionality (e.g., sort)
                            sortAccounts(accounts, numAccounts);
                        }
                        break;
                    case 6:
                        // Logout
                        currentUser = -1;
                        printf("Logged out.\n");
                        break;
                    default:
                        printf("Invalid choice. Try again.\n");
                }
            } while (choice != 6);
        }
    } while (choice != 4);

    return 0;
}


// Function definitions

void displayMenu(int role) {
    if (role == 0) {
        // Bank employee menu
        printf("1. Deposit\n2. Withdraw\n3. View Balance\n4. Account Search\n5. Create User\n6. Logout\n");
    } else {
        // Customer menu
        printf("1. Deposit\n2. Withdraw\n3. View Balance\n4. Logout\n");
    }
}


void login(User *users, int *currentUser) {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < MAX_CREDENTIALS; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            *currentUser = i;
            printf("Login successful. Welcome, %s!\n", username);
            return;
        }
    }

    printf("Login failed. Invalid username or password.\n");
}

void createUser(User *users, int *numUsers) {
    if (*numUsers < MAX_CREDENTIALS) {
        printf("Enter first name: ");
        getchar(); // Clear the input buffer
        fgets(users[*numUsers].firstName, MAX_NAME_LENGTH, stdin);
        users[*numUsers].firstName[strcspn(users[*numUsers].firstName, "\n")] = '\0'; // Remove the newline character
        
        printf("Enter last name: ");
        fgets(users[*numUsers].lastName, MAX_NAME_LENGTH, stdin);
        users[*numUsers].lastName[strcspn(users[*numUsers].lastName, "\n")] = '\0'; // Remove the newline character

        printf("Enter email: ");
        fgets(users[*numUsers].email, MAX_NAME_LENGTH, stdin);
        users[*numUsers].email[strcspn(users[*numUsers].email, "\n")] = '\0'; // Remove the newline character

        printf("Enter social security number: ");
        fgets(users[*numUsers].socialSecurityNumber, MAX_NAME_LENGTH, stdin);
        users[*numUsers].socialSecurityNumber[strcspn(users[*numUsers].socialSecurityNumber, "\n")] = '\0'; // Remove the newline character

        printf("Enter address: ");
        fgets(users[*numUsers].address, MAX_NAME_LENGTH, stdin);
        users[*numUsers].address[strcspn(users[*numUsers].address, "\n")] = '\0'; // Remove the newline character

        printf("Enter new username: ");
        scanf("%s", users[*numUsers].username);

        // Check if the username already exists
        for (int i = 0; i < *numUsers; i++) {
            if (strcmp(users[i].username, users[*numUsers].username) == 0) {
                printf("Username already exists. Please choose a different username.\n");
                return;
            }
        }

        printf("Enter new password: ");
        scanf("%s", users[*numUsers].password);

        printf("Enter user role (0 for bank employee, 1 for customer): ");
        scanf("%d", &users[*numUsers].role);

        (*numUsers)++;
        printf("Account created with the following details:\n");
        printf("Name: %s %s\n", users[*numUsers - 1].firstName, users[*numUsers - 1].lastName);
        printf("Address: %s\n", users[*numUsers - 1].address);
        printf("SSN: %s\n", users[*numUsers - 1].socialSecurityNumber);
        printf("Email: %s\n", users[*numUsers - 1].email);
    } else {
        printf("Maximum number of users reached.\n");
    }
}

void deposit(Account *accounts, int numAccounts) {
    // Implement the logic to deposit money
    char accountHolder[MAX_NAME_LENGTH];
    double amount;
    printf("Enter your account holder name: ");
    scanf("%s", accountHolder);

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].accountHolder, accountHolder) == 0) {
            printf("Enter the amount to deposit: ");
            scanf("%lf", &amount);
            accounts[i].balance += amount;
            printf("Deposit successful. New balance: %.2f\n", accounts[i].balance);
            return;
        }
    }

    printf("Account not found.\n");
}

void withdraw(Account *accounts, int numAccounts) {
    // Implement the logic to withdraw money
    char accountHolder[MAX_NAME_LENGTH];
    double amount;
    printf("Enter your account holder name: ");
    scanf("%s", accountHolder);

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].accountHolder, accountHolder) == 0) {
            printf("Enter the amount to withdraw: ");
            scanf("%lf", &amount);
            if (amount <= accounts[i].balance) {
                accounts[i].balance -= amount;
                printf("Withdrawal successful. New balance: %.2f\n", accounts[i].balance);
            } else {
                printf("Insufficient funds.\n");
            }
            return;
        }
    }

    printf("Account not found.\n");
}

void viewBalance(Account *accounts, int numAccounts) {
    // Implement the logic to view account balance
    char accountHolder[MAX_NAME_LENGTH];
    printf("Enter your account holder name: ");
    scanf("%s", accountHolder);

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].accountHolder, accountHolder) == 0) {
            printf("Account Holder: %s, Balance: %.2f\n", accounts[i].accountHolder, accounts[i].balance);
            return;
        }
    }

    printf("Account not found.\n");
}

void accountSearch(Account *accounts, int numAccounts) {
    // Implement the logic for account search
    char accountHolder[MAX_NAME_LENGTH];
    printf("Enter the account holder name you want to search for: ");
    scanf("%s", accountHolder);

    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].accountHolder, accountHolder) == 0) {
            printf("Account found:\n");
            printf("Account Holder: %s, Balance: %.2f\n", accounts[i].accountHolder, accounts[i].balance);
            return;
        }
    }

    printf("Account not found.\n");
}

void saveAccountData(User *users, int numUsers, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numUsers; i++) {
        fprintf(file, "%s %s %s %s %s %s %s %d\n", users[i].firstName, users[i].lastName, users[i].email,
                users[i].socialSecurityNumber, users[i].address, users[i].username, users[i].password, users[i].role);
    }

    fclose(file);
    printf("User data saved to %s.\n", filename);
}

void loadUserData(User *users, int *numUsers, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File not found. Starting with an empty dataset.\n");
        return;
    }

    while (fscanf(file, "%s %s %s %s %s %s %s %d", users[*numUsers].firstName, users[*numUsers].lastName,
                  users[*numUsers].email, users[*numUsers].socialSecurityNumber, users[*numUsers].address,
                  users[*numUsers].username, users[*numUsers].password, &users[*numUsers].role) == 8) {
        (*numUsers)++;
    }

    fclose(file);
    printf("User data loaded from %s.\n", filename);
}


void loadAccountData(Account *accounts, int *numAccounts, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("File not found. Starting with an empty dataset.\n");
        return;
    }

    while (fscanf(file, "%s %lf", accounts[*numAccounts].accountHolder, &accounts[*numAccounts].balance) == 2) {
        (*numAccounts)++;
    }

    fclose(file);
    printf("Account data loaded from %s.\n", filename);
}

void addCustomer(Account *accounts, int *numAccounts) {
    if (*numAccounts < MAX_ACCOUNTS) {
        printf("Enter account holder name: ");
        scanf("%s", accounts[*numAccounts].accountHolder);

        printf("Enter initial balance: ");
        scanf("%lf", &accounts[*numAccounts].balance);

        (*numAccounts)++;
        printf("Customer added successfully.\n");
    } else {
        printf("Maximum number of accounts reached.\n");
    }
}

void removeCustomer(Account *accounts, int *numAccounts) {
    char accountHolder[MAX_NAME_LENGTH];
    printf("Enter the account holder name to remove: ");
    scanf("%s", accountHolder);

    for (int i = 0; i < *numAccounts; i++) {
        if (strcmp(accounts[i].accountHolder, accountHolder) == 0) {
            // Remove customer by shifting elements
            for (int j = i; j < *numAccounts - 1; j++) {
                strcpy(accounts[j].accountHolder, accounts[j + 1].accountHolder);
                accounts[j].balance = accounts[j + 1].balance;
            }

            (*numAccounts)--;
            printf("Customer removed successfully.\n");
            return;
        }
    }

    printf("Customer not found.\n");
}

void sortAccounts(Account *accounts, int numAccounts) {
    // Implement the logic to sort accounts
    // For simplicity, let's sort by account holder name
    for (int i = 0; i < numAccounts - 1; i++) {
        for (int j = 0; j < numAccounts - i - 1; j++) {
            if (strcmp(accounts[j].accountHolder, accounts[j + 1].accountHolder) > 0) {
                // Swap accounts
                Account temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }

    printf("Accounts sorted successfully.\n");
}
