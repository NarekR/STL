#include <iostream>
#include <string>
#include <vector>

#define CAPACITY 50000 // Size of the HashTable.

class HashTable;

class LinkedList {
public:
    struct Ht_item {
        std::string key;
        std::string value;
    };

    LinkedList* next = nullptr;
    Ht_item item;

    LinkedList() : next(nullptr) {}
    LinkedList(const std::string& key, const std::string& value) {
        item.key = key;
        item.value = value;
        next = nullptr;
    }
};

class HashTable {
public:
    std::vector<LinkedList*> overflow_buckets;
    std::vector<LinkedList::Ht_item*> items;
    int size;
    int count;

    HashTable(int table_size) {
        size = table_size;
        count = 0;
        items.resize(size, nullptr);
        overflow_buckets.resize(size, nullptr);
    }

    ~HashTable() {
        for (int i = 0; i < size; i++) {
            if (items[i]) {
                delete items[i];
            }

            LinkedList* head = overflow_buckets[i];
            while (head) {
                LinkedList* temp = head;
                head = head->next;
                delete temp;
            }
        }
    }

    unsigned long hash_function(const std::string& str) {
        unsigned long hash = 5381;
        for (char c : str) {
            hash = ((hash << 5) + hash) + c;  // hash * 33 + c
        }
        return hash % size;
    }

    void handle_collision(int index, const std::string& key, const std::string& value) {
        LinkedList* head = overflow_buckets[index];
        if (head == nullptr) {
            overflow_buckets[index] = new LinkedList(key, value);
            return;
        }

        while (head->next) {
            head = head->next;
        }
        head->next = new LinkedList(key, value);
    }

    void ht_insert(const std::string& key, const std::string& value) {
        int index = hash_function(key);

        if (items[index] == nullptr) {
            if (count == size) {
                std::cout << "Insert Error: Hash Table is full\n";
                return;
            }

            items[index] = new LinkedList::Ht_item{ key, value };
            count++;
        }
        else {
            if (items[index]->key == key) {
                items[index]->value = value;
            }
            else {
                handle_collision(index, key, value);
            }
        }
    }

    std::string ht_search(const std::string& key) {
        int index = hash_function(key);
        LinkedList::Ht_item* item = items[index];
        LinkedList* head = overflow_buckets[index];

        if (item != nullptr) {
            if (item->key == key) {
                return item->value;
            }

            while (head) {
                if (head->item.key == key) {
                    return head->item.value;
                }
                head = head->next;
            }
        }

        return "";
    }

    void ht_delete(const std::string& key) {
        int index = hash_function(key);
        LinkedList::Ht_item* item = items[index];
        LinkedList* head = overflow_buckets[index];

        if (item == nullptr) return;

        if (head == nullptr && item->key == key) {
            items[index] = nullptr;
            delete item;
            count--;
        }
        else if (head != nullptr) {
            if (item->key == key) {
                delete item;
                LinkedList* node = head;
                head = head->next;
                items[index] = new LinkedList::Ht_item{ node->item.key, node->item.value };
                delete node;
                overflow_buckets[index] = head;
            }
            else {
                LinkedList* curr = head;
                LinkedList* prev = nullptr;

                while (curr) {
                    if (curr->item.key == key) {
                        if (prev == nullptr) {
                            overflow_buckets[index] = curr->next;
                        }
                        else {
                            prev->next = curr->next;
                        }
                        delete curr;
                        return;
                    }
                    prev = curr;
                    curr = curr->next;
                }
            }
        }
    }

    void print_table() {
        std::cout << "\n-------------------Hash Table-------------------\n";

        for (int i = 0; i < size; i++) {
            if (items[i]) {
                std::cout << "Index:" << i << ", Key:" << items[i]->key << ", Value:" << items[i]->value << "\n";
            }

            LinkedList* head = overflow_buckets[i];
            while (head) {
                std::cout << "    [Collision] Key:" << head->item.key << ", Value:" << head->item.value << "\n";
                head = head->next;
            }
        }

        std::cout << "-----------------------------------------\n\n";
    }
};

int main() {
    HashTable ht(CAPACITY);

    ht.ht_insert("apple", "A fruit that's red or green.");
    ht.ht_insert("banana", "A yellow fruit that's long and curved.");
    ht.ht_insert("cat", "A small domesticated mammal.");
    ht.ht_insert("dog", "A domesticated carnivorous mammal.");
    ht.ht_insert("elephant", "A large herbivorous mammal with tusks.");

    std::cout << "Search Results:\n";
    std::cout << "Key:apple, Value: " << ht.ht_search("apple") << "\n";
    std::cout << "Key:banana, Value: " << ht.ht_search("banana") << "\n";
    std::cout << "Key:cat, Value: " << ht.ht_search("cat") << "\n";
    std::cout << "Key:dog, Value: " << ht.ht_search("dog") << "\n";
    std::cout << "Key:elephant, Value: " << ht.ht_search("elephant") << "\n";
    std::cout << "Key:lion, Value: " << ht.ht_search("lion") << "\n";  

    ht.print_table();

    
    ht.ht_delete("apple");  
    ht.ht_delete("dog");    
    ht.ht_delete("lion");  

    ht.print_table();

    return 0;
}

