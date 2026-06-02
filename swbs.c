#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max_user 100
#define max_history 60
#define USER_FILE "users.dat"

int count = 0, flag;
static int bill_index[max_user] = {0};

/* Forward declarations */
void reg();
void login();
void logged(int f);
void deleteAccount();
void bill(int f);
void history(int f);
void query(int f);
void saveUsersToFile();
void loadUsersFromFile();

typedef struct {
    int consume;
    int month;
    int year;
    float bill;
    int overamount;
} maininfo;

typedef struct {
    int id;
    char username[50];
    char password[20];
    char name[50];
    char type[20];
    maininfo bhis[max_history];
} reginfo;
reginfo user[max_user];

/* File I/O Functions */
void saveUsersToFile() {
    FILE *file = fopen(USER_FILE, "wb");
    if (file == NULL) {
        printf("Error: Could not open file for saving!\n");
        return;
    }
    
    fwrite(&count, sizeof(int), 1, file);
    fwrite(bill_index, sizeof(int), max_user, file);
    fwrite(user, sizeof(reginfo), count, file);
    
    fclose(file);
}

void loadUsersFromFile() {
    FILE *file = fopen(USER_FILE, "rb");
    if (file == NULL) {
        printf("No existing user data found. Starting fresh.\n");
        return;
    }
    
    int read_count = fread(&count, sizeof(int), 1, file);
    if (read_count != 1) {
        printf("Error reading user count from file!\n");
        fclose(file);
        return;
    }
    
    fread(bill_index, sizeof(int), max_user, file);
    fread(user, sizeof(reginfo), count, file);
    
    printf("Loaded %d user(s) from file.\n", count);
    fclose(file);
}

void reg() {
    int idc;
    char un[50], pswd[20], nm[50], ty[20];
    label:
    printf("Enter 5 Digit Unique ID: ");
    scanf("%d", &idc);
    getchar();

    for (int i = 0; i < count; i++) {
        if (user[i].id == idc) {
            printf("User already exist!");
            goto label;
        }
    }
    user[count].id = idc;
    printf("Enter Username: ");
    fgets(un, sizeof(un), stdin);
    un[strcspn(un, "\n")] = 0;
    strcpy(user[count].username, un);

    printf("Enter Password: ");
    fgets(pswd, sizeof(pswd), stdin);
    pswd[strcspn(pswd, "\n")] = 0;
    strcpy(user[count].password, pswd);

    printf("Enter Fullname: ");
    fgets(nm, sizeof(nm), stdin);
    nm[strcspn(nm, "\n")] = 0;
    strcpy(user[count].name, nm);

    printf("Enter User Type(residential/commercial): ");
    fgets(ty, sizeof(ty), stdin);
    ty[strcspn(ty, "\n")] = 0;
    strcpy(user[count].type, ty);

    count++;
    saveUsersToFile();

    printf("Registered Successfully!\n");
}

void login() {
    char user_name[50];
    char pass[20];
    int chk = 0;
    int i;

    printf("Enter Username: ");
    fgets(user_name, sizeof(user_name), stdin);
    user_name[strcspn(user_name, "\n")] = 0;

    for (i = 0; i < count; i++) {
        if (strcmp(user_name, user[i].username) == 0) {
            chk = 1;
            flag = i;
            printf("Enter Password: ");
            fgets(pass, sizeof(pass), stdin);
            pass[strcspn(pass, "\n")] = 0;

            if (strcmp(pass, user[i].password) == 0) {
                printf("Login Successfully! as %s\n", user[i].name);
                logged(flag);
                return;
            } else {
                printf("Incorrect password!\n");
                return;
            }
        }
    }

    if (!chk) {
        printf("Register Now!\n");
        printf("Invalid Credential!\n");
    }
}

void logged(int f) {
    int j;
    while (1) {
        printf("Welcome to Water Billing System!\n");
        printf("1. Generate Bill\n2. History\n3. Query\n4. Logout\n");
        printf("Choose: ");
        scanf("%d", &j);
        getchar();

        switch (j) {
            case 1: {
                bill(f);
                break;
            }
            case 2: {
                history(f);
                break;
            }
            case 3: {
                query(f);
                break;
            }
            case 4: {
                return;
            }
            default: {
                printf("Invalid option!\n");
                break;
            }
        }
    }
}

void deleteAccount() {
    int id;
    printf("Enter ID to delete: ");
    scanf("%d", &id);
    getchar();
    int cnt = count;

    int found = 0;

    for (int j = 0; j < cnt; j++) {
        if (user[j].id == id) {
            found = 1;
            if (j < cnt - 1) {
                for (int k = j; k < cnt - 1; k++) {
                    user[k] = user[k + 1];
                }
            }
            count--;
            saveUsersToFile();
            printf("Account deleted successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("Account with ID %d not found!\n", id);
    }
}

void bill(int f) {
    int i = bill_index[f];

    if (i >= max_history) {
        printf("Maximum number of bills reached for this user.\n");
        return;
    }

    printf("Enter month(1-12): ");
    scanf("%d", &user[f].bhis[i].month);
    getchar();
    printf("Enter year: ");
    scanf("%d", &user[f].bhis[i].year);
    getchar();
    printf("Enter Water Consumption(liter): ");
    scanf("%d", &user[f].bhis[i].consume);
    getchar();

    if (strcmp(user[f].type, "residential") == 0) {
        if (user[f].bhis[i].consume <= 5000) {
            user[f].bhis[i].bill = user[f].bhis[i].consume * 0.7f;
        } else {
            user[f].bhis[i].bill = user[f].bhis[i].consume * 1.2f;
        }
    } else if (strcmp(user[f].type, "commercial") == 0) {
        if (user[f].bhis[i].consume <= 12000) {
            user[f].bhis[i].bill = user[f].bhis[i].consume * 0.7f;
        } else {
            user[f].bhis[i].bill = user[f].bhis[i].consume * 1.2f;
        }
    }

    printf("Bill generated successfully!\n");
    printf("Month: %d, Year: %d, Consumption: %d, Bill: %.2f\n", user[f].bhis[i].month, user[f].bhis[i].year, user[f].bhis[i].consume, user[f].bhis[i].bill);

    bill_index[f]++;
}

void history(int f) {
    printf("Water Consumption History:\n");
    printf("Month\tYear\tConsumption\tBill\n");
    for (int i = 0; i < bill_index[f]; i++) {
        printf("%d\t%d\t%d\t%.2f\n", user[f].bhis[i].month, user[f].bhis[i].year, user[f].bhis[i].consume, user[f].bhis[i].bill);
    }
}

void query(int f) {
    int i, j;
    printf("---QUERY BILL---\n");
    printf("Enter Month(1-12): ");
    scanf("%d", &i);
    getchar();
    printf("Enter Year: ");
    scanf("%d", &j);
    getchar();

    printf("Month\tYear\tWater Uses\tType\tBill\n");
    for (int k = 0; k < bill_index[f]; k++) {
        if (i == user[f].bhis[k].month && j == user[f].bhis[k].year) {
            printf("%d\t%d\t%d\t%s\t%.2f\n", user[f].bhis[k].month, user[f].bhis[k].year, user[f].bhis[k].consume, user[f].type, user[f].bhis[k].bill);
            return;
        }
    }
    printf("No matching bill found for the given month and year.\n");
}

int main() {
    loadUsersFromFile();
    
    while (1) {
        int i;
        printf("\n--- Smart Water Billing System ---\n");
        printf("1. Register\n2. Login\n3. Delete Account\n4. Exit\n");
        printf("Choose: ");
        scanf("%d", &i);
        getchar();

        switch (i) {
            case 1: {
                reg();
                break;
            }
            case 2: {
                login();
                break;
            }
            case 3: {
                deleteAccount();
                break;
            }
            case 4: {
                exit(0);
                break;
            }
            default: {
                printf("Invalid option!\n");
                break;
            }
        }
    }
    return 0;
}