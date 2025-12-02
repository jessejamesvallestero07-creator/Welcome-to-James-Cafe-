{
    vector<Item> menu = {
        {"Cappuccino", 140, 20},
        {"Latte", 150, 20},
        {"Muffin", 75, 20},
        {"Burger Meal", 220, 20}
    };

    cout << "=== JAMES CAFE ORDERING APP ===\n";

    string customerName;
    cout << "Enter customer name: ";
    getline(cin, customerName);

    vector<int> boughtQty(menu.size(), 0);
    double total = 0;

    while (true) {
        cout << "\n--- MENU ---\n";
        for (int i = 0; i < menu.size(); i++)
            cout << i + 1 << ") " << menu[i].name << " - ₱" << menu[i].price << "\n";
        cout << "0) Finish ordering\n";

        int choice;
        cout << "Choose an item: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear(); cin.ignore(1000, '\n');
            cout << "Invalid input.\n"; continue;
        }
        if (choice == 0) break;
        if (choice < 1 || choice > menu.size()) {
            cout << "Invalid choice.\n"; continue;
        }

        int index = choice - 1, qty;
        cout << "Enter quantity: ";
        cin >> qty;

        if (cin.fail()) {
            cin.clear(); cin.ignore(1000, '\n');
            cout << "Invalid input.\n"; continue;
        }
        if (qty <= 0) { cout << "Quantity must be positive.\n"; continue; }
        if (qty > menu[index].stock) { cout << "Not enough stock.\n"; continue; }

        boughtQty[index] += qty;
        menu[index].stock -= qty;
        total += menu[index].price * qty;

        cout << "Item added!\n";
    }

    cout << "\n=== RECEIPT ===\n";
    cout << "Customer: " << customerName << "\n\n";

    for (int i = 0; i < menu.size(); i++)
        if (boughtQty[i] > 0)
            cout << menu[i].name << " x" << boughtQty[i]
                 << " = ₱" << menu[i].price * boughtQty[i] << "\n";

    cout << "\nTOTAL: ₱" << total << "\n\n";
    cout << "--- REMAINING STOCKS ---\n";

    for (int i = 0; i < menu.size(); i++)
        cout << menu[i].name << ": " << menu[i].stock << " left\n";
}
