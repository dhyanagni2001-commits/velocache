//cache.cpp --> main engine of LRU eviction based cache

#include "../include/cache.hpp"

void Cache::disconnect(Node* n) { //to remove a node and set other nodes as nothing happened (making sure the chain doesn't break)
    if (n->prev != nullptr) {
        n->prev->next = n->next;
    } else {
        head = n->next;
    }
    if (n->next != nullptr) {
        n->next->prev = n->prev;
    } else {
        tail = n->prev;
    }
}

void Cache::attachToHead(Node*& n) { //to move the detached or 'plucked' node to the front making it the first node(recently used)
    n->next = head;
    n->prev = nullptr;
    if (head != nullptr) {
        head->prev = n;
    }

    head = n;
    if (tail == nullptr) {
        tail = n;
    }
}

bool Cache::contains(const std::string& key) const {
    return cacheMap.find(key) != cacheMap.end();
}

std::string Cache::getValue(const std::string& key) { //to fetch the value form the hashmap i.e. 'cacheMap'
    if (cacheMap.find(key) == cacheMap.end()) {
        return "Value NOT found!\n";
    }
    Node* target = cacheMap[key];

    disconnect(target);
    attachToHead(target);
    return target->value; //returns the data which is mapped to the 'target' key
}


void Cache::putValue(const std::string& key, const std::string& value) {
    if (cacheMap.find(key) != cacheMap.end()) { //if node with the same key exists(even if same value mapped to that key --> idempotent write)
        Node* existing = cacheMap[key];
        existing->value = value;

        disconnect(existing); //remove that node since it is the recently accessed one
        attachToHead(existing); //attach to the head
        return;
    }

    if (max_size == 0) { //a zero-capacity cache can't store anything, so there's nothing to evict either
        return;
    }

    Node* newNode = new Node{key, value, nullptr, nullptr}; //for new node creation

    if (cacheMap.size() >= max_size) { //delete from end of the DLL
        Node* victim = tail; //points to the last node in the DLL
        std::cout << " ALERT: Capacity reached. Evicting: " << victim->key << std::endl;
        cacheMap.erase(victim->key); //clear data for the victim key from the hashmap
        disconnect(victim); //remove the node from the DLL
        delete victim; //delete the node
    }
    cacheMap[key] = newNode;
    attachToHead(newNode); //add node at front
}


