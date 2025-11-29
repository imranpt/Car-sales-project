#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to represent a car
typedef struct {
    char model[50];
    int year;
    float price;
    int remaining;
} Car;

// Structure to represent sales data
typedef struct {
    char customerName[50];
    int customerAge;
    char carModel[50];
    int quantity;
    float totalPrice;
    float discount;
    char purchaseDate[20];
} Sale;

// Structure to represent customer feedback
typedef struct {
    char carModel[50];
    int rating; // Out of 5
    char comment[200];
} Feedback;

#define MAX_CARS 10
#define MAX_SALES 100
#define MAX_FEEDBACKS 100

Car cars[MAX_CARS];
Sale sales[MAX_SALES];
Feedback feedbacks[MAX_FEEDBACKS];
int totalSales = 0;
int totalFeedbacks = 0;

// Function to validate if input is a valid positive integer
int validateAgeInput(int *age) {
    char input[10];
    int valid = 0;

    while (!valid) {
        printf("Enter Customer Age: ");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = 0;

        if (sscanf(input, "%d", age) == 1 && *age > 0) {
            valid = 1;
        } else {
            printf("Invalid input! Please enter a valid positive age.\n");
        }
    }
    return *age;
}

// Function to get current date
void getCurrentDate(char *date) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date, "%d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

// View cars in stock
void viewCarsStock() {
    for (int i = 0; i < MAX_CARS - 1; i++) {
        for (int j = i + 1; j < MAX_CARS; j++) {
            if (cars[i].year < cars[j].year) {
                Car temp = cars[i];
                cars[i] = cars[j];
                cars[j] = temp;
            }
        }
    }

    printf("\nAvailable Cars:\n");
    printf("Model | Year | Price | Remaining\n");

    for (int i = 0; i < MAX_CARS; i++) {
        if (cars[i].remaining > 0) {
            printf("%s | %d | %.2f | %d\n", cars[i].model, cars[i].year,
                   cars[i].price, cars[i].remaining);
        }
    }
}
// customer feedback
void collectFeedback() {
    char carModel[50];
    int rating;
    char comment[200];

    printf("\nEnter the car model for feedback: ");
    fgets(carModel, sizeof(carModel), stdin);
    carModel[strcspn(carModel, "\n")] = 0;

    printf("Enter your rating (1-5): ");
    scanf("%d", &rating);
    while (getchar() != '\n');

    if (rating < 1 || rating > 5) {
        printf("Invalid rating! Please enter a number between 1 and 5.\n");
        return;
    }

    printf("Enter your comment: ");
    fgets(comment, sizeof(comment), stdin);
    comment[strcspn(comment, "\n")] = 0;

    strcpy(feedbacks[totalFeedbacks].carModel, carModel);
    feedbacks[totalFeedbacks].rating = rating;
    strcpy(feedbacks[totalFeedbacks].comment, comment);
    totalFeedbacks++;

    printf("Thank you for your feedback!\n");
}

// buy cars
void buyCars() {
    char customerName[50];
    int customerAge;
    int carChoice, quantity;
    float discount = 0;
    char purchaseDate[20];

    printf("\nEnter Customer Name: ");
    fgets(customerName, sizeof(customerName), stdin);
    customerName[strcspn(customerName, "\n")] = 0;

    validateAgeInput(&customerAge);

    if (customerAge < 18) {
        printf("\n Sorry, you must be at least 18 years old to buy a car.\n");
        return;
    }

    printf("\nAvailable Cars:\n");
    for (int i = 0; i < MAX_CARS; i++) {
        if (cars[i].remaining > 0) {
            printf("%d. %s - %d | %.2f | %d remaining\n",
                   i + 1, cars[i].model, cars[i].year,
                   cars[i].price, cars[i].remaining);
        }
    }

    printf("\nEnter car number to buy: ");
    scanf("%d", &carChoice);
    while (getchar() != '\n');

    if (carChoice < 1 || carChoice > MAX_CARS || cars[carChoice - 1].remaining <= 0) {
        printf("Invalid car choice! Returning to menu...\n");
        return;
    }

    printf("Enter number of cars to buy: ");
    scanf("%d", &quantity);
    while (getchar() != '\n');

    if (quantity <= 0 || quantity > cars[carChoice - 1].remaining) {
        printf("Invalid quantity! We have %d cars left.\n",
               cars[carChoice - 1].remaining);
        return;
    }

    float totalPrice = cars[carChoice - 1].price * quantity;

    if (quantity > 2) {
        discount = totalPrice * 0.1;
        totalPrice -= discount;
        printf("Discount applied: %.2f\n", discount);
    }

    cars[carChoice - 1].remaining -= quantity;

    getCurrentDate(purchaseDate);
    strcpy(sales[totalSales].customerName, customerName);
    sales[totalSales].customerAge = customerAge;
    strcpy(sales[totalSales].carModel, cars[carChoice - 1].model);
    sales[totalSales].quantity = quantity;
    sales[totalSales].totalPrice = totalPrice;
    sales[totalSales].discount = discount;
    strcpy(sales[totalSales].purchaseDate, purchaseDate);
    totalSales++;

    printf("\nPurchase details:\n");
    printf("Customer Name: %s\n", customerName);
    printf("Customer Age: %d\n", customerAge);
    printf("Car: %s, %d\n", cars[carChoice - 1].model, cars[carChoice - 1].year);
    printf("Quantity: %d\n", quantity);
    printf("Total Price: %.2f\n", totalPrice);

    char feedbackChoice;

    printf("\nWould you like to leave feedback? (y/n): ");
    scanf("%c", &feedbackChoice);
    while (getchar() != '\n');

    if (feedbackChoice == 'y' || feedbackChoice == 'Y') {
        collectFeedback();
    }
}

void viewSalesData() {
    if (totalSales == 0) {
        printf("\nNo sales data available.\n");
        return;
    }

    for (int i = 0; i < totalSales - 1; i++) {
        for (int j = i + 1; j < totalSales; j++) {
            if (sales[i].totalPrice < sales[j].totalPrice) {
                Sale temp = sales[i];
                sales[i] = sales[j];
                sales[j] = temp;
            }
        }
    }

    printf("\n--- Sales Data (Sorted by Total Price) ---\n");
    printf("Customer Name | Car Model | Qty | Total Price | Discount | Date\n");

    for (int i = 0; i < totalSales; i++) {
        printf("%s | %s | %d | %.2f | %.2f | %s\n",
               sales[i].customerName,
               sales[i].carModel,
               sales[i].quantity,
               sales[i].totalPrice,
               sales[i].discount,
               sales[i].purchaseDate);
    }
}

// File read
void readSalesDataFromFile() {
    FILE *file = fopen("sales_data.txt", "r");
    if (!file) return;

    while (fscanf(file, "%s %d %s %d %f %f %s\n",
                  sales[totalSales].customerName,
                  &sales[totalSales].customerAge,
                  sales[totalSales].carModel,
                  &sales[totalSales].quantity,
                  &sales[totalSales].totalPrice,
                  &sales[totalSales].discount,
                  sales[totalSales].purchaseDate) == 7) {
        totalSales++;
    }

    fclose(file);
}

// File write
void writeSalesDataToFile() {
    FILE *file = fopen("sales_data.txt", "w");
    if (!file) {
        printf("Error opening sales data file.\n");
        return;
    }

    for (int i = 0; i < totalSales; i++) {
        fprintf(file, "%s %d %s %d %.2f %.2f %s\n",
                sales[i].customerName,
                sales[i].customerAge,
                sales[i].carModel,
                sales[i].quantity,
                sales[i].totalPrice,
                sales[i].discount,
                sales[i].purchaseDate);
    }

    fclose(file);
}

int main() {
    strcpy(cars[0].model, "Toyota Corolla");
    cars[0].year = 2020;
    cars[0].price = 20000.0;
    cars[0].remaining = 10;

    strcpy(cars[1].model, "Honda Civic");
    cars[1].year = 2021;
    cars[1].price = 25000.0;
    cars[1].remaining = 5;

    strcpy(cars[2].model, "BMW 3 Series");
    cars[2].year = 2019;
    cars[2].price = 35000.0;
    cars[2].remaining = 3;

    strcpy(cars[3].model, "Mercedes Benz A-Class");
    cars[3].year = 2022;
    cars[3].price = 40000.0;
    cars[3].remaining = 7;

    readSalesDataFromFile();

    int choice;
    while (1) {
        printf("\n--- Car Sales System ---\n");
        printf("1. View Cars Stock\n");
        printf("2. Buy Cars\n");
        printf("3. View Sales Data\n");
        printf("4. Customer Feedback\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        while (getchar() != '\n');

        switch (choice) {
            case 1: viewCarsStock(); break;
            case 2: buyCars(); break;
            case 3: viewSalesData(); break;
            case 4: collectFeedback(); break;
            case 5:
                writeSalesDataToFile();
                printf("Exiting... Goodbye!\n");
                return 0;
            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return 0;
}

