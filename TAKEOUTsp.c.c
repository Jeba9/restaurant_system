#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

struct foodItem {
    char foodName[35], foodDetails[50];
    int foodCode;
    float unitPrice;
    int stock;
};

struct foodItem foods[1000];
int foodCount;

struct foodOrder {
    int foodID, quantity;
    double unitPrice;
};

struct invoice {
    struct foodOrder orders[20];
    double allTotal;
};

struct foodItem foodItemInfo;

FILE *fileToSave, *tempFile;

void showSellReport();
void readInvoice();
double orderForperson(int);
void showMenu();
void takeOrder();
void orderFood(int);
void removeFoodInfo();
void updateFoodItemInfo();
void title();
void menu();
int authentication();
void addFoodItemInfo();
void saveFoodItemInfo(struct foodItem foodItemInfo, FILE *file);
void viewAllFoodItemInformation();

int main() {
    system("cls");
    title();
lebel3:
    if (authentication() == 0) {
        menu();
    } else {
        printf("\n\t\t\t\t::\t  Your password is incorrect. Please Try again.");
        goto lebel3;
    }

    return 0;
}

void title() {
    printf(
        "\n\n\n\n\n\n\n\t\t\t\t::::::::::::::::::::==================::::::::::"
        "::::::::::");
    printf("\n\t\t\t\t::\t\t\t WELCOME TO\t\t\t::");
    printf("\n\t\t\t\t::\t\t       Take Out Fast Food \t\t::");
    printf(
        "\n\t\t\t\t::::::::::::::::::::==================::::::::::::::::::::");
}

void menu() {
    system("cls");
    title();
    printf(
        "\n\t\t\t\t::\t\t\t  1. Create Food Item \n\t\t\t\t::\t\t\t  2. Show "
        "All Food Items \n\t\t\t\t::\t\t\t  3. Update Food Items "
        "\n\t\t\t\t::\t\t\t  4. Delete Food Items \n\t\t\t\t::\t\t\t  5. Place "
        "Order \n\t\t\t\t::\t\t\t  6. Show Invoice \n\t\t\t\t::\t\t\t  7. Show All Invoice\n\t\t\t\t::\t\t\t  8. Show "
        "Sell Report \n\t\t\t\t::\t\t\t  9. Quit\n\t\t\t\t::\t\t\t"
        "\n\t\t\t\t::\t\t\t   Please enter your choice (Between 1-8) : ");
    int choice;
lebel1:
    scanf("%d", &choice);
    if (choice == 1) {
        addFoodItemInfo();
    } else if (choice == 2) {
        viewAllFoodItemInformation();
    } else if (choice == 3) {
        updateFoodItemInfo();
    } else if (choice == 4) {
        removeFoodInfo();
    } else if (choice == 5) {
        system("cls");
        showMenu();
        takeOrder();
    } else if (choice == 6) {
        system("cls");
        readInvoice();
        printf("\n\n\t\t\t\t::\t\t\t Press any key to continue:   ");
        getch();
        menu();
    }else if (choice == 7) {
        system("cls");
        readAllInvoice();
        printf("\n\n\t\t\t\t::\t\t\t Press any key to continue:   ");
        getch();
        menu();
    } else if (choice == 8) {
        system("cls");
        showSellReport();
        printf("\n\n\t\t\t\t::\t\t\t Press any key to continue:   ");
        getch();
        menu();
    } else if (choice == 9) {
        return;
    } else {
        printf(
            "\n\t\t\t\t::\t\t\tInvalid Choice. You should enter a choice "
            "between 1 to 8.\n\t\t\t\t::\t\t\tPlease enter your choice again "
            "(between 1-8) : ");
        goto lebel1;
    }
}

void showSellReport() {
    FILE *orderFile = fopen("orders.txt", "r");

    int totalOrders = 0;
    double totalRevenue = 0;

    struct foodOrder orders;
    while (fread(&orders, sizeof(struct foodOrder), 1, orderFile)) {
        totalOrders++;
        totalRevenue += orders.unitPrice * orders.quantity;
    }

    printf("\t\tTotal orders: %d\n", totalOrders);
    printf("\t\tTotal revenue: %lf\n", totalRevenue);

    fclose(orderFile);
}

void readInvoice() {
    FILE *invoice = fopen("invoice.txt", "r");
    if (invoice == NULL) {
        printf("\t\tError: could not open file 'invoice.txt'!\n");
    }
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];

    while (fgets(buffer, MAX_LENGTH, invoice)) printf("%s", buffer);

    fclose(invoice);
}

void readAllInvoice() {
    FILE *Allinvoice = fopen("Allinvoice.txt", "r");
    if (Allinvoice == NULL) {
        printf("\t\tError: could not open file 'ALLinvoice.txt'!\n");
    }
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];

    while (fgets(buffer, MAX_LENGTH, Allinvoice)) printf("%s", buffer);

    fclose(Allinvoice);
}

struct foodOrder getOrder() {
    struct foodOrder f;

again:

    printf("\t\tEnter food Code:");
    scanf("%d", &f.foodID);

    int foodIDFound = 0;
    for (int i = 0; i < foodCount; i++) {
        // if no such foodID
        if (foods[i].foodCode == f.foodID) {
            foodIDFound = 1;
            f.unitPrice = foods[i].unitPrice;
            break;
        }
    }

    if (!foodIDFound) {
        printf("\t\tPlease enter a valid food Code.\n");
        goto again;
    }

onceAgain:

    printf("\t\tEnter food quantity:");
    scanf("%d", &f.quantity);

    for (int i = 0; i < foodCount; i++) {
        // if not enough in stock
        if (foods[i].foodCode == f.foodID && foods[i].stock < f.quantity) {
            printf("\t\tPlease enter a valid quantity.\n");
            goto onceAgain;
        }
    }

    return f;
}

void takeOrder() {
    char c;
    double allTotal = 0, total = 0;
again:
    printf("\t\tPlace order now..\n");
    printf("\t\tEnter number of orders:");
    int numOfOrders;
    scanf("%d", &numOfOrders);

    foodCount = 0;

    fileToSave = fopen("fooditeminfo.txt", "r");
    while (fread(&foodItemInfo, sizeof(struct foodItem), 1, fileToSave)) {
        foods[foodCount++] = foodItemInfo;
    }
    fclose(fileToSave);

    struct foodOrder orders[numOfOrders];
    int idx = 0;

    for (int i = 0; i < numOfOrders; i++) {
        // struct foodOrder order = getOrder();
        printf("\n\n\t\tTaking order number %d:\n", i + 1);
        orders[idx++] = getOrder();

        total = orders[idx - 1].unitPrice * orders[idx - 1].quantity;

        allTotal += total;

        // TODO: update the food stock after this order
    }

    // add 15% VAT
    allTotal = allTotal + allTotal * 0.15;

    FILE *orderFile = fopen("orders.txt", "a");

    for (int i = 0; i < idx; i++) {
        fwrite(&orders[i], sizeof(struct foodOrder), 1, orderFile);
    }

    fclose(orderFile);

    system("cls");

    // show invoice of the order
    FILE *invoice = fopen("invoice.txt", "wb");
    FILE *Allinvoice = fopen("Allinvoice.txt", "a");

    printf("\t\t########### Invoice ###########\n");

    for (int i = 0; i < idx; i++) {
        printf("\t\tOrder #%d:\n", i + 1);
        printf("\t\t\tFood Code    : %d\n", orders[i].foodID);
        printf("\t\t\tFood quantity: %d\n", orders[i].quantity);
        printf("\t\t\tTotal        : %.2lf\n",
               orders[i].unitPrice * orders[i].quantity);
        printf("\t\t\t-----------------------------\n");

        fprintf(invoice, "\t\tOrder #%d:\n", i + 1);
        fprintf(invoice, "\t\t\tFood Code    : %d\n", orders[i].foodID);
        fprintf(invoice, "\t\t\tFood quantity: %d\n", orders[i].quantity);
        fprintf(invoice, "\t\t\tTotal        : %.2lf\n",
                orders[i].unitPrice * orders[i].quantity);
        fprintf(invoice, "\t\t\t-----------------------------\n");


        fprintf(Allinvoice, "\t\tOrder #%d:\n", i + 1);
        fprintf(Allinvoice, "\t\t\tFood Code    : %d\n", orders[i].foodID);
        fprintf(Allinvoice, "\t\t\tFood quantity: %d\n", orders[i].quantity);
        fprintf(Allinvoice, "\t\t\tTotal        : %.2lf\n",
                orders[i].unitPrice * orders[i].quantity);
        fprintf(Allinvoice, "\t\t\t-----------------------------\n");
    }

    printf("\t\t\tTotal bill   : %.2lf    (VAT included)\n", allTotal);
    fprintf(invoice, "\t\tTotal bill           : %.2lf    (VAT included)\n", allTotal);
    fprintf(Allinvoice, "\t\tTotal bill           : %.2lf    (VAT included)\n", allTotal);
    fclose(invoice);
    fclose(Allinvoice);

    printf("\t\t########### END ###########\n");

    printf("\n\n\t\t\t\t::\t\t\t Press any key to continue:   ");
    getch();

    menu();
}

void showMenu() {
    printf("\n\n\t\t\t\t::\t\t\t******All Food Item Information******\n");
    printf(
        "\n\t\t\t\t::\t\t\tFood Name\tFood Code\tDetails\t\tStock\t\tUnit "
        "Price");
    fileToSave = fopen("fooditeminfo.txt", "r");
    while (fread(&foodItemInfo, sizeof(struct foodItem), 1, fileToSave)) {
        printf("\n\t\t\t\t::\t\t\t%s\t\t%d\t\t%s\t\t%d\t\t\%.2f\n",
               foodItemInfo.foodName, foodItemInfo.foodCode,
               foodItemInfo.foodDetails, foodItemInfo.stock,
               foodItemInfo.unitPrice);
    }
    fclose(fileToSave);
}

int authentication() {
    char fixPassword[15] = "1";
    char userPassword[15];
    printf("\n\t\t\t\t::\t\t\t  Enter Password :");
    fflush(stdin);
    gets(userPassword);
    return strcmp(userPassword, fixPassword);
}

void addFoodItemInfo() {
    system("cls");
    title();
    printf(
        "\n\t\t\t\t::\t\t\t***Please fill up all necessary information.***\n");
    printf("\n\t\t\t\t::\t\t\t***Enter the right information as asked***");
    printf("\n\n\t\t\t\t::\t\t\tFood Name : ");
    fflush(stdin);
    gets(foodItemInfo.foodName);
    printf("\n\t\t\t\t::\t\t\tFood Code : ");
    fflush(stdin);
    scanf("%d", &foodItemInfo.foodCode);
    printf("\n\t\t\t\t::\t\t\tFood Details : ");
    fflush(stdin);
    gets(foodItemInfo.foodDetails);
    printf("\n\t\t\t\t::\t\t\tFood Quantity/Stock : ");
    fflush(stdin);
    scanf("%d", &foodItemInfo.stock);
    printf("\n\t\t\t\t::\t\t\tUnite Price : ");
    fflush(stdin);
    scanf("%f", &foodItemInfo.unitPrice);

    saveFoodItemInfo(foodItemInfo, fileToSave);
}

//                 *********++++++++++++++++++++++++++++++++++*******

void saveFoodItemInfo( struct foodItem foodItemInfo, FILE *file) {
    file = fopen("fooditeminfo.txt", "a");
    fwrite(&foodItemInfo, sizeof(struct foodItem), 1, file);
    if (fwrite != 0) {
        printf("\n\t\t\t\t::\t\t\tInformation saved successfully.");
    } else {
        printf("\n\t\t\t\t::\t\t\tFailed to save the Information");
    }
    fclose(file);
    printf(
        "\n\t\t\t\t::\t\t\tDo you want to add another food info? \nPress Y for "
        "yes and N for no:  ");
    char choice;
    fflush(stdin);
    scanf("%c", &choice);
    if (choice == 'Y' || choice == 'y') {
        addFoodItemInfo();
    } else {
        menu();
    }
}

void viewAllFoodItemInformation() {
    system("cls");
    title();
    printf("\n\n\t\t\t\t::\t\t\t******All Food Item Information******\n");
    printf(
        "\n\t\t\t\t::\t\t\tFood Name\tFood Code\tDetails\t\tStock\t\tUnit "
        "Price");
    fileToSave = fopen("fooditeminfo.txt", "r");
    while (fread(&foodItemInfo, sizeof(struct foodItem), 1, fileToSave)) {
        printf("\n\t\t\t\t::\t\t\t%s\t\t%d\t\t%s\t\t%d\t\t\%.2f\n",
               foodItemInfo.foodName, foodItemInfo.foodCode,
               foodItemInfo.foodDetails, foodItemInfo.stock,
               foodItemInfo.unitPrice);
    }
    fclose(fileToSave);
    printf("\n\n\t\t\t\t::\t\t\t Press any key to continue:   ");
    getch();
    menu();
}
/// update food info
void updateFoodItemInfo() {
    system("cls");
    title();
    printf(
        "\n\nA. Update General Info\nB. Update Price & Quantity Info\n\nPLease "
        "enter your choice :");
    char choice;
    fflush(stdin);
lebel4:
    scanf("%c", &choice);
    choice = toupper(choice);
    printf("\nPLease enter name of the food : ");
    char search_name[30];
    fflush(stdin);
    gets(search_name);
    // Define the selection
    int found = 0, found1 = 0;
    switch (choice) {
        case 'A':
            fileToSave = fopen("fooditeminfo.txt", "r+");
            found = 0;
            while (
                fread(&foodItemInfo, sizeof(struct foodItem), 1, fileToSave)) {
                if (strcmpi(foodItemInfo.foodName, search_name) == 0) {
                    found = 1;
                    printf("\nEnter New Name: ");
                    fflush(stdin);
                    gets(foodItemInfo.foodName);
                    printf("Enter New COde: ");
                    fflush(stdin);
                    // gets(foodItemInfo.foodCode);
                    scanf("%d", &foodItemInfo.foodCode);
                    printf("Enter New details: ");
                    fflush(stdin);
                    gets(foodItemInfo.foodDetails);
                    fseek(fileToSave, -sizeof(foodItemInfo), SEEK_CUR);
                    fwrite(&foodItemInfo, sizeof(struct foodItem), 1,
                           fileToSave);
                    fclose(fileToSave);
                }
            }
            if (found == 1) {
                printf("Food information updated");
            } else {
                printf("Food not found in file");
            }
            break;
        case 'B':
            fileToSave = fopen("fooditeminfo.txt", "r+");
            found1 = 0;
            while (
                fread(&foodItemInfo, sizeof(struct foodItem), 1, fileToSave)) {
                if (strcmpi(foodItemInfo.foodName, search_name) == 0) {
                    found1 = 1;
                    printf("\n\n\tUpdated Price : ");
                    fflush(stdin);
                    scanf("%f", &foodItemInfo.unitPrice);
                    printf("\n\n\tUpdated Quantity : ");
                    fflush(stdin);
                    scanf("%d", &foodItemInfo.stock);
                    fseek(fileToSave, -sizeof(foodItemInfo), SEEK_CUR);
                    fwrite(&foodItemInfo, sizeof(struct foodItem), 1,
                           fileToSave);
                    fclose(fileToSave);
                }
            }
            if (found1 == 1) {
                printf("Food information updated successfully");
            } else {
                printf("Food not found in file");
            }

            break;

        default:
            printf("\nInput is wrong.!\nTry again!!");
    }
    printf("\n\n\t\t\t\t::\t\t\t Press any key to continue:   ");
    getch();
    menu();
}

/// delete food info
void removeFoodInfo() {
    char searchName[24];
    system("cls");
    title();  // call title function

    printf(
        "\n\n\t\t\t================= Delete/Remove food Information "
        "=================");

    fileToSave = fopen("fooditeminfo.txt", "r");  // open the original file
    tempFile = fopen("temporaryfoodinfo.txt", "w");

    printf("\n\n\t\t\tEnter the food name you want to delete: ");
    fflush(stdin);
    gets(searchName);
    while (fread(&foodItemInfo, sizeof(struct foodItem), 1, fileToSave)) {
        if (strcmp(foodItemInfo.foodName, searchName) != 0) {
            fwrite(&foodItemInfo, sizeof(struct foodItem), 1, tempFile);
        }
    }
    fclose(fileToSave);
    fclose(tempFile);
    remove("fooditeminfo.txt");

    if (rename("temporaryfoodinfo.txt", "fooditeminfo.txt")) {
    }
    printf(
        "\n\n\t\t======================\tRecode Delete "
        "Successful\t======================");
    printf("\n\n\t\t\t\t::\t\t\t Press any key to continue:   ");
    getch();
    menu();
}
