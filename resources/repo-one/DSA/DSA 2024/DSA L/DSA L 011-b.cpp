#include <iostream>

class Hashtable 
{
public:
    explicit Hashtable(int size, std::string addressing_type = "closed") : size(size), addressing_type(addressing_type) 
    {
        table = new std::pair<int, int>[size];  
        for (int i = 0; i < size; ++i) 
        {
            table[i].first = -1; 
        }
    }

    ~Hashtable() 
    {
        delete[] table;
    }

private:
    int hash_function(int key) 
    {
        return key % size;
    }

    int size;
    std::string addressing_type;
    std::pair<int, int>* table; 

public:
    void insert(int key, int value) 
    {
        if (addressing_type == "closed") 
        {
            insert_closed(key, value);
        } else if (addressing_type == "open") 
        {
            insert_open(key, value);
        } else {
            std::cerr << "Invalid addressing type: " << addressing_type << std::endl;
        }
    }

    void insert_closed(int key, int value) 
    {
        int hash_index = hash_function(key);
        int probe_index = hash_index;

        while (table[probe_index].first != -1) 
        {
            if (table[probe_index].first == key) 
            {
                table[probe_index].second = value;
                return;
            }
            probe_index = (probe_index + 1) % size;
        }

        table[probe_index].first = key;
        table[probe_index].second = value;
    }

    void insert_open(int key, int value) 
    {
        int hash_index = hash_function(key);
        int probe_index = hash_index;

        while (table[probe_index].first != -1 && table[probe_index].first != key) 
        {
            probe_index = (probe_index + 1) % size;
        }

        if (table[probe_index].first == -1) 
        {
            table[probe_index].first = key;
            table[probe_index].second = value;
        } else {
            std::cerr << "Hashtable overflow" << std::endl;
        }
    }

    int search(int key) {
        if (addressing_type == "closed") 
        {
            return search_closed(key);
        } else if (addressing_type == "open") 
        {
            return search_open(key);
        } else {
            std::cerr << "Invalid addressing type: " << addressing_type << std::endl;
            return -1; 
        }
    }

    int search_closed(int key) 
    {
        int hash_index = hash_function(key);
        int probe_index = hash_index;

        while (table[probe_index].first != -1) 
        {
            if (table[probe_index].first == key) 
            {
                return table[probe_index].second;
            }
            probe_index = (probe_index + 1) % size;
        }

        return -1;  

    }

    int search_open(int key) {
        int hash_index = hash_function(key);
        int probe_index = hash_index;

        while (table[probe_index].first != -1) 
        {
            if (table[probe_index].first == key) 
            {
                return table[probe_index].second;
            }
            probe_index = (probe_index + 1) % size;
        }

        return -1; 
    }

    void delete_closed(int key) 
    {
        int hash_index = hash_function(key);
        int probe_index = hash_index;

    bool found = false;
        int empty_slot = -1;
        while (table[probe_index].first != -1) 
        {
            if (table[probe_index].first == key) 
            {
            found = true;
            break;
            } else if (table[probe_index].first == -1 && empty_slot == -1) 
            {
            empty_slot = probe_index;
            }
            probe_index = (probe_index + 1) % size;
        }

        if (!found) {
            return;
        }

        if (empty_slot != -1) 
        {
            std::swap(table[probe_index], table[empty_slot]);
        }

        table[probe_index].first = -1;
        table[probe_index].second = -1;

        probe_index = (probe_index + 1) % size;
        while (table[probe_index].first != -1) 
        {
            int current_key = table[probe_index].first;
            int current_value = table[probe_index].second;

            table[probe_index].first = -1;
            table[probe_index].second = -1;

            insert_closed(current_key, current_value);

            probe_index = (probe_index + 1) % size;
        }
        return;
    }

    void delete_open(int key) 
    {
        int hash_index = hash_function(key);
        int probe_index = hash_index;

        while (table[probe_index].first != -1) 
        {
            if (table[probe_index].first == key) 
            {
            table[probe_index].first = -1;
            table[probe_index].second = -1;
            return;
            }
            probe_index = (probe_open(probe_index, size));
        }
    }

int probe_open(int index, int size) {
  return (index + 1) % size;
}


};

int main()
{
    return 0;
}