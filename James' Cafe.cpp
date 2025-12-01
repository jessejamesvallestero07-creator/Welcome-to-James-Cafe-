#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Item {
    string name;
    double price;
    int stock;
};

int main() {
    // Simple menu
    vector<Item> menu = {
        {"Cappuccino", 140, 20},
        {"Latte", 150, 20},
        {"Muffin", 75, 20},
        {"Burger Meal", 220, 20}
    };

    cout << "=== SIMPLE CAFE ORDERING SYSTEM ===\n";

    string customerName;
    cout << "Enter customer name: ";
    getline(cin, customerName);

    vector<int> boughtQty(menu.size(), 0); // how many pieces the customer buys
    double total = 0;

    while (true) {
        cout << "\n--- MENU ---\n";
        for (int i = 0; i < menu.size(); i++) {
            cout << i + 1 << ") " 
                 << menu[i].name << " - ₱" << menu[i].price
                 << " (" << menu[i].stock << " left)\n";
        }
        cout << "0) Finish ordering\n";

        int choice;
        cout << "Choose an item: ";
        cin >> choice;

        if (choice == 0) break;
        if (choice < 1 || choice > menu.size()) {
            cout << "Invalid choice.\n";
            continue;
        }

        int index = choice - 1;

        int qty;
        cout << "Enter quantity: ";
        cin >> qty;

        if (qty > menu[index].stock) {
            cout << "Not enough stock.\n";
            continue;
        }

        // Update totals
        boughtQty[index] += qty;
        menu[index].stock -= qty;
        total += menu[index].price * qty;

        cout << "Added to order!\n";
    }

    // Print receipt
    cout << "\n=== RECEIPT ===\n";
    cout << "Customer: " << customerName << "\n";

    for (int i = 0; i < menu.size(); i++) {
        if (boughtQty[i] > 0) {
            cout << menu[i].name << " x" << boughtQty[i]
                 << " = ₱" << menu[i].price * boughtQty[i] << "\n";
        }
    }

    cout << "TOTAL: ₱" << total << "\n";

    return 0;
