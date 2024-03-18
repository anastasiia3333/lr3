#include <iostream>
#include <list>
#include <random>
using namespace std;

template<typename T>
T getRandElem(const list<T>& lst);

int main() {
    list<int> my_List;
	srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < 10; i++) {
        int random_value = rand() % 1000; 
        my_List.push_back(random_value);
    }
	
	for (auto i = my_List.begin(); i != my_List.end(); ++i) {
		cout << *i << " ";
	}
	cout << endl;
    cout <<"Рандомний елемент: "<< getRandElem(my_List) << endl;
    return 0;
}

template<typename T>
T getRandElem(const list<T>& lst) {
    if (lst.empty()) {
        cerr << "Список порожній!" << endl;
        exit(1);
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, lst.size() - 1);
    auto it = lst.begin();
    advance(it, dis(gen));
    return *it;
}