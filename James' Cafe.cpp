#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name, category;
    double price;
    int qty, sold;
};

struct OrderLine {
    int itemIndex;
    int quantity;
};

struct Order {
    string customer, dineOption;
    vector<OrderLine> lines;
    double total(const vector<Item>& m) const {
        double t = 0;
        for (auto& L : lines) t += m[L.itemIndex].price * L.quantity;
        return t;
    }
};

int inputInt(string p) { int x; cout << p; cin >> x; cin.ignore(); return x; }
string inputStr(string p) { string s; cout << p; getline(cin, s); return s; }
bool yes(string p) { string s; cout << p; cin >> s; cin.ignore(); return (s=="Y"||s=="y"); }

int main() {
    vector<Item> menu = {
        {"Cappuccino","Beverages",140,20,0},{"Latte","Beverages",150,20,0},
        {"Iced Americano","Beverages",120,20,0},{"Milkshake","Beverages",190,20,0},
        {"Muffin","Snacks",75,20,0},{"Toast","Snacks",95,20,0},
        {"Donut Holes","Snacks",100,20,0},{"Chicken Wrap","Meals",180,20,0},
        {"Burger Meal","Meals",220,20,0},{"Pasta","Meals",275,20,0},
        {"Cake Slice","Desserts",130,20,0},{"Parfait","Desserts",110,20,0},
        {"Tiramisu","Desserts",270,20,0}
    };

    vector<string> cats = {"Beverages","Snacks","Meals","Desserts"};
    vector<Order> all;
    int served = 0;

    while (true) {
        cout << "\n--- NEW CUSTOMER ---\n";
        Order ord;
        ord.customer = inputStr("Customer name: ");
        ord.dineOption = yes("Eat-In? (Y/N): ") ? "Eat-In" : "Take-Out";

        while (true) {
            cout << "\nCategories:\n";
            for (int i=0;i<cats.size();i++) cout << i+1 << ") " << cats[i] << endl;
            cout << "0) Finish order\n";

            int c = inputInt("Choose category: ");
            if (c==0) break;
            string cat = cats[c-1];

            vector<int> avail;
            for (int i=0;i<menu.size();i++)
                if (menu[i].category==cat && menu[i].qty>0) avail.push_back(i);

            if (avail.empty()) { cout<<"No items available.\n"; continue; }

            cout << "\nItems:\n";
            for (int i=0;i<avail.size();i++) {
                int id=avail[i];
                cout<<i+1<<") "<<menu[id].name<<" ₱"<<menu[id].price
                    <<" ("<<menu[id].qty<<" left)\n";
            }
            cout << "0) Back\n";

            int it = inputInt("Choose item: ");
            if (it==0) continue;
            int id = avail[it-1];

            int q = inputInt("Quantity: ");
            if (q > menu[id].qty) { cout<<"Not enough stock.\n"; continue; }

            ord.lines.push_back({id,q});
            menu[id].qty -= q;
            menu[id].sold += q;

            if (!yes("Add more items? (Y/N): ")) break;
        }

        if (!ord.lines.empty()) {
            all.push_back(ord);
            served++;
            cout << "\n--- RECEIPT ---\n";
            cout << "Customer: "<<ord.customer<<" ("<<ord.dineOption<<")\n";
            for (auto& L : ord.lines)
                cout << menu[L.itemIndex].name << " x" << L.quantity
                     << " = ₱" << menu[L.itemIndex].price * L.quantity << endl;
            cout << "TOTAL: ₱" << ord.total(menu) << "\n";
        }

        if (!yes("Serve next customer? (Y/N): ")) break;
    }

    double revenue = 0; int sold = 0;
    for (auto& o : all) revenue += o.total(menu);
    for (auto& i : menu) sold += i.sold;

    cout << "\n=== DAILY SUMMARY ===\n";
    cout << "Customers served: " << served << endl;
    cout << "Total revenue: ₱" << revenue << endl;
    cout << "Total items sold: " << sold << endl;

    int best = 0;
    for (int i=1;i<menu.size();i++)
        if (menu[i].sold > menu[best].sold) best = i;

    cout << "Best seller: " << menu[best].name << " ("<<menu[best].sold<<" sold)\n";

    cout << "\nRemaining inventory:\n";
    for (auto& i : menu) cout<<i.name<<": "<<i.qty<<" left\n";
}

