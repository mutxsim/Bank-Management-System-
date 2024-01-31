#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CREDENTIALS 100
#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50
#define MAX_TRANSACTIONS 100

// logi

// Data structures
typedef struct
{
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char email[MAX_NAME_LENGTH];
    char socialSecurityNumber[MAX_NAME_LENGTH];
    char address[MAX_NAME_LENGTH];
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    int role; // 0 for bank employee, 1 for customer
} User;

typedef struct
{
    char accountHolder[MAX_NAME_LENGTH];
    double balance;
    char transactions[MAX_TRANSACTIONS][MAX_NAME_LENGTH]; // Array to store transaction history
    int numTransactions;                                  // Counter to keep track of transactions
} Account;

// Function prototypes
void displayMenu(User *users, int currentUser, Account *accounts, int *numAccounts, int *numUsers);

int login(User *users, int *currentUser, Account *accounts, int *numAccounts);

void createUser(User *users, int *numAccounts, Account *accounts);
void deposit(Account *accounts, int numAccounts, User currentUser);
void withdraw(Account *accounts, int numAccounts, int currentUser, double amount);
void viewBalance(Account *accounts, int numAccounts, int currentUser);
void accountSearch(Account *accounts, int numAccounts);
// Function prototypes for account info
void saveAccountData(Account *accounts, int numAccounts);
void loadAccountData(Account *accounts, int *numAccounts);
// Function prototypes for user data
void saveUserData(User *users, int numUsers);
void loadUserData(User *users, int *numUsers);

// Admin-specific functionalities ()
void viewCustomer(Account *accounts, int numAccounts);
void addCustomer();
void removeCustomer();
void sortAccounts();

int main()
{
    User users[MAX_CREDENTIALS];
    Account accounts[MAX_ACCOUNTS];

    int currentUser = -1;
    int numAccounts = 0;
    int numUsers = 0;

    // Load user data at the start
    loadUserData(users, &numUsers);
    // Load account data at the start
    loadAccountData(accounts, &numAccounts);

    // Main program loop
    int choice;
    char yn;
    do
    {
        printf("\n------------JAK Bank------------\n");
        puts("");
        printf("Do you have an account? (y/n):");
        scanf(" %c", &yn);

        switch (yn)
        {
        case 'y':
            printf("1. Login as User\n2. Login as Admin\n3. Exit\n");
            printf("--------------------------------\n");
            printf("\nEnter your choice:");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                // login(users, &currentUser, &accounts, &numAccounts);
                if (login(users, &currentUser, &accounts, &numAccounts) == 1)
                {
                    printf("Login successful. Welcome, %s!\n", &users[currentUser].username);
                    displayMenu(users, currentUser, accounts, &numAccounts, &numUsers);
                }
                break;

            case 2:
                login(users, &currentUser, &accounts, &numAccounts);
                if (currentUser != -1 && users[currentUser].role != 0)
                {
                    printf("Invalid credentials. Logging out. \n");
                    currentUser = -1;
                }
                displayMenu(users, currentUser, accounts, &numAccounts, &numUsers);
                break;

            case 3:

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
            puts("");

            switch (response2)
            {

            case 'y':
                createUser(users, &numAccounts, accounts);
                break;

            case 'n':
                printf("Okay, Goodbye!");
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
            }
            break;

        default:
            printf("Invalid choice. Try again.\n");
            break;
        }
    } while (1); // Loop until explicitly exited
}

int login(User *users, int *currentUser, Account *accounts, int *numAccounts)
{
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];

    printf("\nEnter username: ");
    scanf("%s", username);

    // Clear input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    printf("Enter password: ");
    scanf("%s", password);

    // Clear input buffer again
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    for (int i = 0; i < MAX_CREDENTIALS; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            *currentUser = i;

            // printf("Login successful. Welcome, %s!\n", username);
            return 1;
        }
    }

    printf("\nLogin failed. Invalid username or password.\n");
    return 0;
}

void createUser(User *users, int *numUsers, Account *accounts)
{
    if (*numUsers < MAX_CREDENTIALS)
    {
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
        for (int i = 0; i < *numUsers; i++)
        {
            if (strcmp(users[i].username, users[*numUsers].username) == 0)
            {
                printf("Username already exists. Please choose a different username.\n");
                return;
            }
        }

        printf("Enter new password: ");
        scanf("%s", users[*numUsers].password);

        printf("Enter user role (0 for bank employee, 1 for customer): ");
        int role;
        while (1)
        {
            if (scanf("%d", &role) == 1)
            {
                if (role == 0 || role == 1)
                {
                    break; // Valid input, exit the loop
                }
                else
                {
                    printf("Invalid input. Please enter 0 for bank employee or 1 for customer: ");
                }
            }
            else
            {
                printf("Invalid input. Please enter a number: ");
                while (getchar() != '\n')
                    ; // Clear the invalid input from the buffer
            }
        }

        users[*numUsers].role = role;

        strcpy(accounts[*numUsers].accountHolder, users[*numUsers].username);
        accounts[*numUsers].balance = 0.0;
        accounts[*numUsers].numTransactions = 0;

        (*numUsers)++;
        printf("Account created with the following details:\n");
        printf("Name: %s %s\n", users[*numUsers - 1].firstName, users[*numUsers - 1].lastName);
        printf("Address: %s\n", users[*numUsers - 1].address);
        printf("SSN: %s\n", users[*numUsers - 1].socialSecurityNumber);
        printf("Email: %s\n", users[*numUsers - 1].email);

        saveAccountData(users, *numUsers);
        saveUserData(users, *numUsers);
    }
    else
    {
        printf("Maximum number of users reached.\n");
    }
}

void displayMenu(User *users, int currentUser, Account *accounts, int *numAccounts, int *numUsers)
{
    int choice;

    switch (users[currentUser].role)
    {
    case 1:
        do
        {
            printf("\nCustomer Menu:\n");
            printf("1. Deposit\n");
            printf("2. Withdraw\n");
            printf("3. View Balance\n");
            printf("4. View Transactions\n");
            printf("0. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                deposit(accounts, *numAccounts, users[currentUser]);
                break;
            case 2:
            {
                double amount;
                printf("Enter the amount to withdraw: ");
                scanf("%lf", &amount);
                withdraw(accounts, *numAccounts, currentUser, amount);
            }
            break;
            case 3:
                viewBalance(accounts, *numAccounts, currentUser);
                break;
            case 4:
            {
                printf("Transaction History:\n");
                for (int i = 0; i < accounts[currentUser].numTransactions; ++i)
                {
                    printf("%d. %s\n", i + 1, accounts[currentUser].transactions[i]);
                }
            }
            break;
            case 0:
                printf("Exiting customer menu.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
            }
        } while (choice != 0);
        break;

    case 0:
        do
        {
            printf("\nAdmin Menu:\n");
            printf("1. View Customer Info\n");
            printf("2. Add Customer\n");
            printf("3. Remove Customer\n");
            printf("4. Sort Accounts\n");
            printf("0. Exit\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                viewCustomer(accounts, *numAccounts); // Implement the view customer info function or its logic
                break;
            case 2:
                addCustomer(users, numUsers, accounts, numAccounts); // Call the addCustomer function
                break;
            case 3:
                removeCustomer(users, numUsers, accounts, numAccounts); // Call the removeCustomer function
                break;
            case 4:
                sortAccounts(accounts, *numAccounts); // Call the sortAccounts function
                break;
            case 0:
                printf("Exiting admin menu.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
            }
        } while (choice != 0);
        break;

    default:
        printf("Invalid role.\n");
        break;
    }
}

void deposit(Account *accounts, int numAccounts, User currentUser)
{
    double amount;
    printf("Enter the amount to deposit: ");
    scanf("%lf", &amount);

    for (int i = 0; i < numAccounts; i++)
    {
        if (strcmp(accounts[i].accountHolder, currentUser.username) == 0)
        {
            accounts[i].balance += amount;
            printf("Deposit successful. New balance: %.2f\n", accounts[i].balance);

            // Format the transaction string and update the history for deposits
            sprintf(accounts[i].transactions[accounts[i].numTransactions], "Deposit: %.2f", amount);
            accounts[i].numTransactions++;
            return;
        }
    }
}

void withdraw(Account *accounts, int numAccounts, int currentUser, double amount)
{
    if (currentUser >= 0 && currentUser < numAccounts)
    {
        if (amount <= accounts[currentUser].balance)
        {
            accounts[currentUser].balance -= amount;
            printf("Withdrawal successful. New balance: %.2f\n", accounts[currentUser].balance);

            // Check if there's space in the transaction history
            if (accounts[currentUser].numTransactions < MAX_TRANSACTIONS)
            {
                // Format the transaction string and update the history for withdrawals
                sprintf(accounts[currentUser].transactions[accounts[currentUser].numTransactions], "Withdrawal: %.2f", amount);
                accounts[currentUser].numTransactions++;
            }
            else
            {
                printf("Transaction history is full. Unable to record withdrawal.\n");
            }
        }
        else
        {
            printf("Insufficient funds.\n");
        }
    }
    else
    {
        printf("Invalid user account.\n");
    }
}

void viewBalance(Account *accounts, int numAccounts, int currentUser)
{
    if (currentUser >= 0 && currentUser < numAccounts)
    {
        printf("Account Holder: %s, \nBalance: %.2f\n", accounts[currentUser].accountHolder, accounts[currentUser].balance);
    }
    else
    {
        printf("Invalid user account.\n");
    }
}

void viewCustomer(Account *accounts, int numAccounts)
{
    char searchUsername[MAX_NAME_LENGTH];
    printf("Enter the username of the customer to view: ");
    scanf("%s", searchUsername);

    int found = 0;

    for (int i = 0; i < numAccounts; i++)
    {
        if (strcmp(accounts[i].accountHolder, searchUsername) == 0)
        {
            found = 1;

            // Display account information
            printf("Account Holder: %s\n", accounts[i].accountHolder);
            printf("Balance: %.2f\n", accounts[i].balance);

            // Display transaction history
            printf("Transaction History:\n");
            for (int j = 0; j < accounts[i].numTransactions; ++j)
            {
                printf("%d. %s\n", j + 1, accounts[i].transactions[j]);
            }

            break; // No need to continue searching
        }
    }

    if (!found)
    {
        printf("Account with username %s not found.\n", searchUsername);
    }
}

// Function to add a new customer
void addCustomer(User *users, int *numUsers, Account *accounts, int *numAccounts)
{
    if (*numUsers < MAX_CREDENTIALS && *numAccounts < MAX_ACCOUNTS)
    {
        // Input user details
        printf("Enter first name: ");
        getchar();
        fgets(users[*numUsers].firstName, MAX_NAME_LENGTH, stdin);
        users[*numUsers].firstName[strcspn(users[*numUsers].firstName, "\n")] = '\0';

        printf("Enter last name: ");
        fgets(users[*numUsers].lastName, MAX_NAME_LENGTH, stdin);
        users[*numUsers].lastName[strcspn(users[*numUsers].lastName, "\n")] = '\0';

        // ... (similarly input other details)

        printf("Enter new username: ");
        scanf("%s", users[*numUsers].username);

        // Check if the username already exists
        for (int i = 0; i < *numUsers; i++)
        {
            if (strcmp(users[i].username, users[*numUsers].username) == 0)
            {
                printf("Username already exists. Please choose a different username.\n");
                return;
            }
        }

        printf("Enter new password: ");
        scanf("%s", users[*numUsers].password);

        printf("Enter user role (0 for bank employee, 1 for customer): ");
        int role;
        while (1)
        {
            if (scanf("%d", &role) == 1)
            {
                if (role == 0 || role == 1)
                {
                    break; // Valid input, exit the loop
                }
                else
                {
                    printf("Invalid input. Please enter 0 for bank employee or 1 for customer: ");
                }
            }
            else
            {
                printf("Invalid input. Please enter a number: ");
                while (getchar() != '\n')
                    ; // Clear the invalid input from the buffer
            }
        }

        users[*numUsers].role = role;

        // Initialize account for the new user
        strcpy(accounts[*numAccounts].accountHolder, users[*numUsers].username);
        accounts[*numAccounts].balance = 0.0;
        accounts[*numAccounts].numTransactions = 0;

        (*numUsers)++;
        (*numAccounts)++;

        printf("Customer added successfully.\n");

        // Save data to files
        // ... (other save data calls)
    }
    else
    {
        printf("Maximum number of users/accounts reached.\n");
    }
}

// Function to remove a customer
void removeCustomer(User *users, int *numUsers, Account *accounts, int *numAccounts)
{
    char usernameToRemove[MAX_NAME_LENGTH];
    printf("Enter the username of the customer to remove: ");
    scanf("%s", usernameToRemove);

    int userIndex = -1;
    int accountIndex = -1;

    // Find the user and corresponding account
    for (int i = 0; i < *numUsers; i++)
    {
        if (strcmp(users[i].username, usernameToRemove) == 0)
        {
            userIndex = i;
            break;
        }
    }

    for (int i = 0; i < *numAccounts; i++)
    {
        if (strcmp(accounts[i].accountHolder, usernameToRemove) == 0)
        {
            accountIndex = i;
            break;
        }
    }

    // If the user and account are found, remove them
    if (userIndex != -1 && accountIndex != -1)
    {
        // Remove user
        for (int i = userIndex; i < *numUsers - 1; i++)
        {
            users[i] = users[i + 1];
        }

        // Remove account
        for (int i = accountIndex; i < *numAccounts - 1; i++)
        {
            accounts[i] = accounts[i + 1];
        }

        (*numUsers)--;
        (*numAccounts)--;

        printf("Customer removed successfully.\n");

        // Save data to files
        saveAccountData(accounts, *numAccounts);
        saveUserData(users, *numUsers);
    }
    else
    {
        printf("Customer not found.\n");
    }
}

// Function to sort accounts (by account holder's username)
void sortAccounts(Account *accounts, int numAccounts)
{
    // Use a simple bubble sort for demonstration purposes
    for (int i = 0; i < numAccounts - 1; i++)
    {
        for (int j = 0; j < numAccounts - i - 1; j++)
        {
            if (strcmp(accounts[j].accountHolder, accounts[j + 1].accountHolder) > 0)
            {
                // Swap accounts
                Account temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }

    printf("Accounts sorted successfully.\n");

    // Save sorted data to file
    saveAccountData(accounts, numAccounts);
}

// Function to save user data to a file
void saveUserData(User *users, int numUsers)
{
    FILE *file = fopen("user.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numUsers; ++i)
    {
        fprintf(file, "Customer %d:\n First Name: %s Last Name: %s \nEmail: %s \nSSN: %s \nAddress: %s \nUsername: %s \nPassword: %s Role: %d\n",
                i + 1, users[i].firstName, users[i].lastName, users[i].email,
                users[i].socialSecurityNumber, users[i].address, users[i].username,
                users[i].password, users[i].role);
    }

    fclose(file);
    printf("User data saved successfully.\n");
}

// Function to load user data from a file
void loadUserData(User *users, int *numUsers)
{
    FILE *file = fopen("user.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file for reading. Assuming first run.\n");
        return;
    }

    int customerNumber;
    while (fscanf(file, "Customer %d:\n First Name: %s Last Name: %s \nEmail: %s \nSSN: %s \nAddress: %s \nUsername: %s \nPassword: %s Role: %d",
                  &customerNumber, users[*numUsers].firstName, users[*numUsers].lastName,
                  users[*numUsers].email, users[*numUsers].socialSecurityNumber,
                  users[*numUsers].address, users[*numUsers].username, users[*numUsers].password,
                  &users[*numUsers].role) == 9)
    {
        (*numUsers)++;
    }

    fclose(file);
    printf("User data loaded successfully.\n");
}

void saveAccountData(Account *accounts, int numAccounts)
{
    FILE *file = fopen("accountinfo.txt", "w");
    if (file == NULL)
    {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numAccounts; ++i)
    {
        fprintf(file, "User: %s %.2f %d\n", accounts[i].accountHolder, accounts[i].balance, accounts[i].numTransactions);

        for (int j = 0; j < accounts[i].numTransactions; ++j)
        {
            fprintf(file, "%s\n", accounts[i].transactions[j]);
        }
    }

    fclose(file);
    printf("Account data saved successfully.\n");
}

void loadAccountData(Account *accounts, int *numAccounts)
{
    FILE *file = fopen("accountinfo.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file for reading. Assuming first run.\n");
        return;
    }

    while (fscanf(file, "User: %s %lf %d", accounts[*numAccounts].accountHolder,
                  &accounts[*numAccounts].balance, &accounts[*numAccounts].numTransactions) == 3)
    {
        for (int i = 0; i < accounts[*numAccounts].numTransactions; ++i)
        {
            fscanf(file, "%s", accounts[*numAccounts].transactions[i]);
        }
        (*numAccounts)++;
    }

    fclose(file);
    printf("Account data loaded successfully.\n");
}
