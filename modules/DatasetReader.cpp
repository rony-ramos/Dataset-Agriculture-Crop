#ifndef DATASETREADER_CPP
#define DATASETREADER_CPP

#include <string>
#include <vector>
#include "LinkedList.cpp"

using namespace std;

// Clase abstracta para lectores de dataset
class DatasetReader {
public:
    virtual ~DatasetReader() = default;
    virtual bool load(const string& path) = 0;
    virtual vector<string> header() const = 0;
    virtual const LinkedList<vector<string>>& rows() const = 0;
};

#endif
