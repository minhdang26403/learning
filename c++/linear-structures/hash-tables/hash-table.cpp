#include <iostream>
#include <string>
#include <unordered_map>

template <typename type>
struct Cell {
  type key;
  type value;
  Cell *link;
};

template <typename type>
class HashTable {
 private:
  static const int INITIAL_BUCKET_COUNT = 15;
  Cell<type> **buckets;
  int nBuckets;

  // Hash function
  std::hash<type> hasher;

  Cell<type> *findCell(int bucket, const type &key) {
    Cell<type> *cp = buckets[bucket];
    while ((cp != nullptr) && (cp->key != key)) {
      cp = cp->link;
    }
    return cp;
  }
  // Don't allow copy hash table
  HashTable(const HashTable &src){};
  HashTable &operator=(const HashTable &src) { return *this; };

 public:
  HashTable() {
    nBuckets = INITIAL_BUCKET_COUNT;
    buckets = new Cell<type> *[nBuckets];
    for (int i = 0; i < nBuckets; i++) {
      buckets[i] = nullptr;
    }
  }

  ~HashTable() { clear(); }

  type get(const type &key) {
    int bucket = hasher(key) % nBuckets;
    Cell<type> *cp = findCell(bucket, key);
    return (cp == nullptr) ? "The key doesn't exist" : cp->value;
  }

  void put(const type &key, const type &value) {
    int bucket = hasher(key) % nBuckets;
    Cell<type> *cp = findCell(bucket, key);
    if (cp == nullptr) {
      cp = new Cell<type>;
      cp->key = key;
      cp->link = buckets[bucket];
      buckets[bucket] = cp;
    }
    cp->value = value;
  }

  bool containsKey(const type &key) {
    int bucket = hasher(key) % nBuckets;
    Cell<type> *cp = findCell(bucket, key);
    return (cp != nullptr) ? true : false;
  }

  void remove(const type &key) {
    int bucket = hasher(key) % nBuckets;
    if (buckets[bucket]->key == key) {
      buckets[bucket] = nullptr;
    } else {
      Cell<type> *cur = new Cell<type>;
      cur = buckets[bucket];
      Cell<type> *prev = new Cell<type>;
      while ((cur != nullptr) && (cur->key != key)) {
        prev = cur;
        cur = cur->link;
      }
      if (cur == nullptr) {
        std::cout << "There is no such key!" << std::endl;
      } else {
        Cell<type> *temp = prev->link;
        prev->link = cur->link;
        delete temp;
      }
    }
  }

  void clear() {
    for (int i = 0; i < nBuckets; i++) {
      Cell<type> *cp = buckets[i];
      while (cp != nullptr) {
        Cell<type> *oldCell = cp;
        cp = cp->link;
        delete oldCell;
      }
    }
  }
};

int main() {
  HashTable<std::string> table1;
  table1.put("OH", "Ohio");
  table1.put("MI", "Michigan");
  table1.put("CA", "California");

  std::cout << table1.get("OH") << std::endl;
  table1.remove("MI");

  std::cout << table1.get("MI") << std::endl;

  table1.remove("OH");
  std::cout << table1.containsKey("OH") << std::endl;
}