#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Product {
    int id;
    char name[50];
    float price;
    int quantity;
    char size[5];
};

struct Order {
    int productId;
    int quantity;
    char size[5];
};

void addProduct(struct Product store[], int *numProducts) {
    struct Product product;
    product.id = *numProducts + 1;

    printf("Enter product name: ");
    scanf("%s", product.name);

    printf("Enter product price: ");
    scanf("%f", &product.price);

    printf("Enter product quantity: ");
    scanf("%d", &product.quantity);

    printf("Enter product size (M/L/XL) or 'N' if it has no size: ");
    scanf("%s", product.size);

    store[*numProducts] = product;
    (*numProducts)++;
    printf("Product added successfully.\n");
}

void displayProducts(struct Product store[], int numProducts) {
    if (numProducts == 0) {
        printf("No products in the store.\n");
        return;
    }

    printf("Product ID  Name                 Price   Quantity  Size\n");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < numProducts; i++) {
        printf("%-11d %-20s %.2f    %-9d %s\n",
               store[i].id, store[i].name, store[i].price, store[i].quantity, store[i].size);
    }
}

void deleteProduct(struct Product store[], int *numProducts, int productId) {
    for (int i = 0; i < *numProducts; i++) {
        if (store[i].id == productId) {
            for (int j = i; j < *numProducts - 1; j++) {
                store[j] = store[j + 1];
            }
            (*numProducts)--;
            printf("Product with ID %d has been deleted.\n", productId);
            return;
        }
    }
    printf("Product with ID %d not found.\n", productId);
}
void placeOrder(struct Product store[], int numProducts, struct Order orders[], int *numOrders) {
    printf("Place Order\n");

    int productId;
    printf("Enter the Product ID to order: ");
    scanf("%d", &productId);

    if (productId < 1 || productId > numProducts) {
        printf("Invalid Product ID.\n");
        return;
    }

    int quantity;
    printf("Enter the quantity: ");
    scanf("%d", &quantity);

    if (quantity <= 0) {
        printf("Quantity must be greater than 0.\n");
        return;
    }

    if (strcmp(store[productId - 1].size, "N") != 0) {
        char size[5];
        printf("Enter the size (M/L/XL): ");
        scanf("%s", size);

        if (strcmp(store[productId - 1].size, size) != 0) {
            printf("Invalid size for this product.\n");
            return;
        }
    }

    if (store[productId - 1].quantity < quantity) {
        printf("Insufficient quantity in stock.\n");
        return;
    }

    store[productId - 1].quantity -= quantity;

    orders[*numOrders].productId = productId;
    strcpy(orders[*numOrders].size, store[productId - 1].size);
    orders[*numOrders].quantity = quantity;
    (*numOrders)++;

    char confirm;
    printf("To confirm your order, press 'y' or 'n': ");
    scanf(" %c", &confirm);

    if (confirm == 'y' || confirm == 'Y') {
        printf("Order placed successfully.\n");
    } else {
        printf("Order not confirmed. Stock has been reverted.\n");
        store[productId - 1].quantity += quantity;
        (*numOrders)--;
    }
}

void displayOrders(struct Order orders[], int numOrders, struct Product store[], int numProducts) {
    if (numOrders == 0) {
        printf("No orders placed yet.\n");
        return;
    }

    printf("Order ID\tProduct Name\tQuantity\tSize\n");
    for (int i = 0; i < numOrders; i++) {
        int productId = orders[i].productId;
        printf("%d\t\t%s\t\t%d\t\t%s\n", i + 1, store[productId - 1].name, orders[i].quantity, orders[i].size);
    }
}

void saveProductData(struct Product store[], int numProducts) {
    FILE *file = fopen("products.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < numProducts; i++) {
        fprintf(file, "%d %s %.2f %d %s\n", store[i].id, store[i].name, store[i].price, store[i].quantity, store[i].size);
    }

    fclose(file);
}

void loadProductData(struct Product store[], int *numProducts) {
    FILE *file = fopen("products.txt", "r");
    if (file == NULL) {
        printf("No existing product data.\n");
        return;
    }

    *numProducts = 0;
    while (fscanf(file, "%d %s %f %d %s\n", &store[*numProducts].id, store[*numProducts].name,
                  &store[*numProducts].price, &store[*numProducts].quantity, store[*numProducts].size) != EOF) {
        (*numProducts)++;
    }

    fclose(file);
}

int main() {
    struct Product store[100];
    struct Order orders[100];
    int numProducts = 0;
    int numOrders = 0;
    int isBuyerMode = 0;

    loadProductData(store, &numProducts);

    int choice;
    do {
        printf("\nStore Management System\n");
        printf("1. Switch to Buyer Mode\n");
        printf("2. Switch to Seller Mode\n");
        printf("3. Add Product\n");
        printf("4. Display Products\n");
        printf("5. Delete Product\n");
        printf("6. Place Order (Buyer Mode)\n");
        printf("7. View Orders (Seller Mode)\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                isBuyerMode = 1;
                printf("Switched to Buyer Mode.\n");
                break;
            case 2:
                isBuyerMode = 0;
                printf("Switched to Seller Mode.\n");
                break;
            case 3:
                if (isBuyerMode) {
                    printf("You are in Buyer Mode. Switch to Seller Mode to add products.\n");
                } else {
                    addProduct(store, &numProducts);
                }
                break;
            case 4:
                displayProducts(store, numProducts);
                break;
            case 5:
                if (isBuyerMode) {
                    printf("You are in Buyer Mode. Switch to Seller Mode to delete products.\n");
                } else {
                    printf("Enter the Product ID to delete: ");
                    int productId;
                    scanf("%d", &productId);
                    deleteProduct(store, &numProducts, productId);
                }
                break;
            case 6:
                if (isBuyerMode) {
                    placeOrder(store, numProducts, orders, &numOrders);
                } else {
                    printf("You are in Seller Mode. Switch to Buyer Mode to place orders.\n");
                }
                break;
            case 7:
                if (!isBuyerMode) {
                    displayOrders(orders, numOrders, store, numProducts);
                } else {
                    printf("You are in Buyer Mode. Switch to Seller Mode to view orders.\n");
                }
                break;
            case 8:
                saveProductData(store, numProducts);
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 8);

    return 0;
}
