#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <cctype>
using namespace std;

// ---------- Structs ----------
struct Item {
    string name, category;
    double price;
    int qty, sold = 0;
};

struct Order {
    string customer, dineOption;
    vector<pair<Item*, int>> lines;
    unsigned long long receiptNo;

    double total() const {
        double sum = 0;
        for (auto& line : lines)
            sum += line.first->price * line.second;
        return sum;
    }

    void printReceipt() const {
        auto tt = chrono::system_clock::to_time_t(chrono::system_clock::now());
        tm tmv{};
#ifdef _WIN32
        localtime_s(&tmv, &tt);
#else
        localtime_r(&tt, &tmv);
#endif
        char buf[40]; strftime(buf, 40, "%Y-%m-%d %H:%M:%S", &tmv);

        cout << "\n=== James' Café Receipt ===\n";
        cout << "Receipt# " << receiptNo << "   " << buf << "\n";
        cout << "Customer: " << customer << " (" << dineOption << ")\n\n";
        cout << left << setw(25) << "Item" << setw(6) << "Qty" << "Subtotal\n";
        cout << "--------------------------------------\n";
        for (auto& line : lines)
            cout << left << setw(25) << line.first->name
            << setw(6) << line.second
            << "₱ " << fixed << setprecision(2) << line.first->price * line.second << "\n";
        cout << "--------------------------------------\n";
        cout << "TOTAL: ₱ " << fixed << setprecision(2) << total() << "\n\n";
    }
};

// ---------- Helpers ----------
unsigned long long generateReceiptNo() {
    static unsigned long long counter = 0;
    auto ms = chrono::duration_cast<chrono::milliseconds>(
        chrono::system_clock::now().time_since_epoch()).count();
    return (ms % 1000000000ULL) + (++counter);
}

string getString(const string& prompt) {
    string s;
    do {
        cout << prompt;
        getline(cin, s);
        while (!s.empty() && isspace(s.front())) s.erase(s.begin());
        while (!s.empty() && isspace(s.back())) s.pop_back();
    } while (s.empty());
    return s;
}

int getInt(const string& prompt, int minVal, int maxVal) {
    string s; int x;
    while (true) {
        cout << prompt;
        getline(cin, s);
        try { x = stoi(s); if (x >= minVal && x <= maxVal) return x; }
        catch (...) {}
        cout << "Invalid input. Try again.\n";
    }
}

bool yesNo(const string& prompt) {
    string s;
    while (true) {
        cout << prompt;
        getline(cin, s);
        for (auto& c : s) c = tolower(c);
        if (s == "y" || s == "yes") return true;
        if (s == "n" || s == "no") return false;
        cout << "Please answer Y or N.\n";
    }
}

// ---------- Main Program ----------
int main() {
    vector<Item> menu = {
        {"Cappuccino","Beverages",140,20}, {"Latte","Beverages",150,20}, {"Iced Americano","Beverages",120,20},
        {"Milkshake","Beverages",190,20}, {"Muffin","Snacks",75,20}, {"Toast","Snacks",95,20}, {"Donut Holes","Snacks",100,20},
        {"Chicken Wrap","Meals",180,20}, {"Burger Meal","Meals",220,20}, {"Pasta","Meals",275,20},
        {"Cake Slice","Desserts",130,20}, {"Parfait","Desserts",110,20}, {"Tiramisu","Desserts",270,20}
    };
    vector<string> categories = { "Beverages","Snacks","Meals","Desserts" };
    vector<Order> allOrders;
    int served = 0;

    while (true) {
        cout << "\n--- New Customer ---\n";
        Order order;
        order.receiptNo = generateReceiptNo();
        order.customer = getString("Enter customer name: ");
        order.dineOption = yesNo("Eat-In? (Y/N): ") ? "Eat-In" : "Take-Out";

        while (true) {
            cout << "\nCategories:\n";
            for (int i = 0; i < categories.size(); i++) {
                bool soldOut = true;
                for (auto& item : menu) if (item.category == categories[i] && item.qty > 0) soldOut = false;
                cout << i + 1 << ") " << categories[i] << (soldOut ? " [SOLD OUT]" : "") << "\n";
            }
            cout << "0) Finish order\n";

            int catChoice = getInt("Choose category: ", 0, categories.size());
            if (catChoice == 0) break;
            string cat = categories[catChoice - 1];

            vector<Item*> available;
            for (auto& item : menu) if (item.category == cat && item.qty > 0) available.push_back(&item);
            if (available.empty()) { cout << "No items available in this category.\n"; continue; }

            for (int i = 0; i < available.size(); i++)
                cout << i + 1 << ") " << available[i]->name << " ₱" << available[i]->price
                << " (" << available[i]->qty << " left)\n";
            cout << "0) Back\n";

            int itemChoice = getInt("Select item: ", 0, available.size());
            if (itemChoice == 0) continue;
            Item* selected = available[itemChoice - 1];
            int qty = getInt("Enter quantity: ", 1, selected->qty);
            order.lines.push_back({ selected, qty });
            selected->qty -= qty; selected->sold += qty;

            if (!yesNo("Add more items? (Y/N): ")) break;
        }

        if (!order.lines.empty()) { order.printReceipt(); allOrders.push_back(order); served++; }
        else cout << "No items ordered. Cancelling transaction.\n";

        if (!yesNo("Serve next customer? (Y/N): ")) break;
    }

    double revenue = 0; int totalSold = 0;
    for (auto& o : allOrders) revenue += o.total();
    for (auto& item : menu) totalSold += item.sold;

    cout << "\n=== Daily Summary ===\nCustomers served: " << served
        << "\nTotal revenue: ₱" << fixed << setprecision(2) << revenue
        << "\nTotal items sold: " << totalSold << "\n";

    Item* best = nullptr;
    for (auto& item : menu) if (!best || item.sold > best->sold) best = &item;
    if (best && best->sold > 0) cout << "Best seller: " << best->name << " (" << best->sold << " sold)\n";

    cout << "\nRemaining inventory:\n";
    for (auto& item : menu) cout << "- " << item.name << ": " << item.qty << " left\n";
}
