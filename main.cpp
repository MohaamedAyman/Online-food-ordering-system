#include <iostream>
#include <vector>
#include <string>

using namespace std;

class MenuItem {
public:
    string name;
    double price;
    MenuItem(string n, double p) : name(n), price(p) {}
};

class Customer {
public:
    string name;
    Customer(string n) : name(n) {}
};

class Order {
public:
    Customer* customer;
    vector<MenuItem> items;
    Order* next;

    Order(Customer* c) : customer(c), next(nullptr) {}

    void addItem(MenuItem item) {
        items.push_back(item);
    }

    void printOrder() {
        cout << "Order for " << customer->name << ":\n";
        for (auto& item : items)
            cout << " - " << item.name << " - " << item.price << " EGP" << endl;
    }
};

class StackNode {
public:
    Order* order;
    StackNode* next;

    StackNode(Order* o) : order(o), next(nullptr) {}
};

class OrderStack {
private:
    StackNode* topNode; 

public:
    OrderStack() : topNode(nullptr) {}

    void push(Order* order) {
        StackNode* newNode = new StackNode(order);
        newNode->next = topNode;
        topNode = newNode;
    }

   
    void pop() {
        if (topNode != nullptr) {
            StackNode* temp = topNode;
            topNode = topNode->next;
            delete temp; 
        }
    }

 
    Order* top() {
        if (topNode != nullptr) {
            return topNode->order;
        }
        return nullptr;
    }

  
    bool empty() {
        return topNode == nullptr;
    }
};

class System {
private:
    vector<MenuItem> menu;
    Order* ordersHead;
    OrderStack undoStack; 

public:
    System() : ordersHead(nullptr) {}

    void addMenuItem(string name, double price) {
        menu.push_back(MenuItem(name, price));
    }

    void updatePrice(int index, double newPrice) {
        if (index >= 0 && index < menu.size()) {
            menu[index].price = newPrice;
            cout << menu[index].name << " price updated to " << newPrice << " EGP\n";
        }
    }

    void showMenu() {
        cout << "\n ---{Menu}--- \n";
        for (int i = 0; i < menu.size(); i++)
            cout << i + 1 << ". " << menu[i].name << " - " << menu[i].price << " EGP" << endl;
    }

    MenuItem getMenuItem(int index) {
        return menu[index];
    }

    void addOrder(Order* newOrder) {
        newOrder->next = ordersHead;
        ordersHead = newOrder;
        cout << "Order added for " << newOrder->customer->name << endl;
    }

    void printOrders() {
        cout << "\n--- {Current Orders} ---\n";
        Order* temp = ordersHead;
        while (temp) {
            temp->printOrder();
            temp = temp->next;
        }
    }

    void cancelLastOrder() {
        if (ordersHead == nullptr) {
            cout << "No orders to cancel.\n";
            return;
        }

        Order* orderToCancel = ordersHead; 
        ordersHead = ordersHead->next;   

        undoStack.push(orderToCancel);     
        cout << "Last order canceled.\n";
    }

    void redoLastOrder() {
        if (undoStack.empty()) {
            cout << "No canceled orders to redo.\n";
            return;
        }
        Order* restored = undoStack.top(); 
        undoStack.pop();                  

        restored->next = ordersHead;
        ordersHead = restored;
        cout << "Last canceled order restored.\n";
    }

    int optimalDeliveryTime(int n) {
        if (n <= 0) return 0;
        return 10 + optimalDeliveryTime(n - 1);
    }

    int countOrders() {
        int count = 0;
        Order* temp = ordersHead;
        while (temp) {
            count++;
            temp = temp->next;
        }
        return count;
    }
};

int main() {
    System system;

    system.addMenuItem("Kabab", 100);
    system.addMenuItem("Kofta", 80);
    system.addMenuItem("Salad", 20);

    system.showMenu();

    Customer* c1 = new Customer("Alaa");
    Order* order1 = new Order(c1);
    order1->addItem(system.getMenuItem(0));
    order1->addItem(system.getMenuItem(2));
    system.addOrder(order1);

    Customer* c2 = new Customer("Ayman");
    Order* order2 = new Order(c2);
    order2->addItem(system.getMenuItem(1));
    system.addOrder(order2);

    system.printOrders();
    system.cancelLastOrder();
    system.printOrders();
    system.redoLastOrder();
    system.printOrders();

    int totalTime = system.optimalDeliveryTime(system.countOrders());
    cout << "\nEstimated total delivery time: " << totalTime << " minutes\n";

    return 0;
}
