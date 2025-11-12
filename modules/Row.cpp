#ifndef ROW_CPP
#define ROW_CPP

#include "LinkedList.cpp"
#include <string>

using namespace std;

// Representa una fila del dataset usando LinkedList (TDA visto en clase)
class Row {
private:
    LinkedList<string> fields;  // Campos de la fila
    
public:
    Row() {}
    
    void addField(const string& field) {
        fields.push_back(field);
    }
    
    bool getField(size_t index, string& out) const {
        return fields.get(index, out);
    }
    
    size_t size() const {
        return fields.size();
    }
    
    template<typename F>
    void forEach(F func) const {
        fields.for_each(func);
    }
    
    const LinkedList<string>& getFields() const {
        return fields;
    }
};

#endif
