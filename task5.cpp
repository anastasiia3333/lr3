#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace std;

template<typename T>
class List {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* head;
    int size;
public:
    List() : head(nullptr), size(0) {}

    ~List() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    T get_random_element() const {
        if (size == 0) {
            cerr << "Список порожній!" << endl;
            exit(1);
        }
        srand(time(nullptr));
        int index = rand() % size;
        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    void print() const {
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};

int main() {
    List<int> my_List;
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < 10; i++) {
        int random_value = rand() % 1000;
        my_List.push_back(random_value);
    }

    my_List.print();
    cout << "Рандомний елемент: " << my_List.get_random_element() << endl;
    return 0;
}