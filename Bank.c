#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[50];
    float balance;
} Account;

Account accounts[100];
int account_count = 0;

// Save accounts to file
void save_accounts() {
    FILE *f = fopen("accounts.dat", "wb");
    if (f == NULL) {
        printf("Error saving accounts!\n");
        return;
    }
    fwrite(accounts, sizeof(Account), account_count, f);
    fclose(f);
}

// Load accounts from file
void load_accounts() {
    FILE *f = fopen("accounts.dat", "rb");
    if (f == NULL) {
        return;
    }
    account_count = fread(accounts, sizeof(Account), 100, f);
    fclose(f);
}

// Create a new account
void create_account() {
    Account new_acc;
    new_acc.id = account_count + 1;
    printf("Enter name: ");
    getchar(); // clear newline
    fgets(new_acc.name, 50, stdin);
    new_acc.name[strcspn(new_acc.name, "\n")] = 0;
    printf("Enter initial balance: ");
    scanf_s("%f", &new_acc.balance);
    accounts[account_count++] = new_acc;
    save_accounts();
    printf("Account created successfully! ID: %d\n", new_acc.id);
}

// View a single account
void view_account() {
    int id;
    printf("Enter account ID: ");
    scanf_s("%d", &id);
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].id == id) {
            printf("ID: %d\nName: %s\nBalance: %.2f\n", accounts[i].id, accounts[i].name, accounts[i].balance);
            return;
        }
    }
    printf("Account not found!\n");
}

// Deposit money
void deposit() {
    int id;
    float amount;
    printf("Enter account ID: ");
    scanf_s("%d", &id);
    printf("Enter amount to deposit: ");
   scanf_s("%f", &amount);
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].id == id) {
            accounts[i].balance += amount;
            save_accounts();
            printf("Deposit successful! New Balance: %.2f\n", accounts[i].balance);
            return;
        }
    }
    printf("Account not found!\n");
}

// Withdraw money
void withdraw() {
    int id;
    float amount;
    printf("Enter account ID: ");
    scanf_s("%d", &id);
    printf("Enter amount to withdraw: ");
    scanf_s("%f", &amount);
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].id == id) {
            if (accounts[i].balance >= amount) {
                accounts[i].balance -= amount;
                save_accounts();
                printf("Withdrawal successful! New Balance: %.2f\n", accounts[i].balance);
            } else {
                printf("Insufficient balance!\n");
            }
            return;
        }
    }
    printf("Account not found!\n");
}

// View all accounts
void view_all_accounts() {
    printf("\n=== All Accounts ===\n");
    for (int i = 0; i < account_count; i++) {
        printf("ID: %d | Name: %s | Balance: %.2f\n", accounts[i].id, accounts[i].name, accounts[i].balance);
    }
}

// Delete an account
void delete_account() {
    int id;
    printf("Enter account ID to delete: ");
    scanf_s("%d", &id);
    int found = 0;
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].id == id) {
            for (int j = i; j < account_count - 1; j++) {
                accounts[j] = accounts[j + 1];
            }
            account_count--;
            save_accounts();
            found = 1;
            printf("Account deleted successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Account not found!\n");
    }
}

// Edit account name
void edit_account() {
    int id;
    printf("Enter account ID to edit: ");
   scanf_s("%d", &id);
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].id == id) {
            printf("Enter new name: ");
            getchar(); // clear newline
            fgets(accounts[i].name, 50, stdin);
            accounts[i].name[strcspn(accounts[i].name, "\n")] = 0;
            save_accounts();
            printf("Account name updated successfully!\n");
            return;
        }
    }
    printf("Account not found!\n");
}

// Transfer money between accounts
void transfer_money() {
    int from_id, to_id;
    float amount;
    printf("Sender Account ID: ");
    scanf_s("%d", &from_id);
    printf("Receiver Account ID: ");
    scanf_s("%d", &to_id);
    printf("Amount to transfer: ");
    scanf_s("%f", &amount);

    Account *sender = NULL, *receiver = NULL;
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].id == from_id) sender = &accounts[i];
        if (accounts[i].id == to_id) receiver = &accounts[i];
    }

    if (!sender || !receiver) {
        printf("Invalid account IDs!\n");
        return;
    }

    if (sender->balance < amount) {
        printf("Insufficient balance!\n");
        return;
    }

    sender->balance -= amount;
    receiver->balance += amount;
    save_accounts();
    printf("Transfer successful!\n");
}

// Search accounts by name
void search_account_by_name() {
    char search_name[50];
    printf("Enter name to search: ");
    getchar(); // clear newline
    fgets(search_name, 50, stdin);
    search_name[strcspn(search_name, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < account_count; i++) {
        if (strstr(accounts[i].name, search_name)) {
            printf("ID: %d | Name: %s | Balance: %.2f\n", accounts[i].id, accounts[i].name, accounts[i].balance);
            found = 1;
        }
    }
    if (!found) {
        printf("No account found matching that name.\n");
    }
}

// Sort accounts by balance (high to low)
void sort_accounts_by_balance() {
    for (int i = 0; i < account_count-1; i++) {
        for (int j = i+1; j < account_count; j++) {
            if (accounts[i].balance < accounts[j].balance) {
                Account temp = accounts[i];
                accounts[i] = accounts[j];
                accounts[j] = temp;
            }
        }
    }
    printf("Accounts sorted by balance (High to Low).\n");
    view_all_accounts();
}

// Main menu
int main() {
    int choice;
    load_accounts();

//Condition always true
    while (1) {
        printf("\n=== Bank Management System ===\n"); 
        printf("1. Create Account\n");
        printf("2. View Account\n");
        printf("3. Deposit\n");
        printf("4. Withdraw\n");
        printf("5. View All Accounts\n");
        printf("6. Delete Account\n");
        printf("7. Edit Account\n");
        printf("8. Transfer Money\n");
        printf("9. Search Account by Name\n");
        printf("10. Sort Accounts by Balance\n");
        printf("11. Exit\n");
        printf("Choice: ");
       scanf_s("%d", &choice);

        switch (choice) {
            case 1: create_account(); break;
            case 2: view_account(); break;
            case 3: deposit(); break;
            case 4: withdraw(); break;
            case 5: view_all_accounts(); break;
            case 6: delete_account(); break;
            case 7: edit_account(); break;
            case 8: transfer_money(); break;
            case 9: search_account_by_name(); break;
            case 10: sort_accounts_by_balance(); break;
            case 11: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}
