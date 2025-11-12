#ifndef DATASETREADER_CPP
#define DATASETREADER_CPP

#include <string>
#include "LinkedList.cpp"
#include "Row.cpp"

using namespace std;

// Clase abstracta para lectores de dataset (TDA)
class DatasetReader {
protected:
    LinkedList<string> hdr;      // Cabeceras
    LinkedList<Row> data;         // Filas de datos
    
public:
    virtual ~DatasetReader() = default;
    virtual bool load(const string& path) = 0;
    
    const LinkedList<string>& header() const { return hdr; }
    const LinkedList<Row>& rows() const { return data; }
    
    size_t numColumns() const { return hdr.size(); }
    size_t numRows() const { return data.size(); }
};

#endif
