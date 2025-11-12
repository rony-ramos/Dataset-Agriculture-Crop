#ifndef CSVREADER_CPP
#define CSVREADER_CPP

#include <fstream>
#include <sstream>
#include "DatasetReader.cpp"

using namespace std;

class CSVReader : public DatasetReader {
private:
    void parseLine(const string& line, Row& row) {
        stringstream ss(line);
        string field;
        while (getline(ss, field, ',')) {
            // Trim espacios al inicio y final
            size_t start = field.find_first_not_of(" \t\r\n");
            size_t end = field.find_last_not_of(" \t\r\n");
            if (start != string::npos) {
                field = field.substr(start, end - start + 1);
            } else {
                field = "";
            }
            row.addField(field);
        }
    }
    
public:
    bool load(const string& path) override {
        ifstream file(path);
        if (!file.is_open()) return false;
        
        string line;
        
        // Leer cabecera
        if (getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            stringstream ss(line);
            string colName;
            while (getline(ss, colName, ',')) {
                // Trim
                size_t start = colName.find_first_not_of(" \t\r\n");
                size_t end = colName.find_last_not_of(" \t\r\n");
                if (start != string::npos) {
                    colName = colName.substr(start, end - start + 1);
                }
                hdr.push_back(colName);
            }
        }
        
        // Leer datos
        while (getline(file, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) continue;
            
            Row row;
            parseLine(line, row);
            if (row.size() > 0) {
                data.push_back(row);
            }
        }
        
        file.close();
        return true;
    }
};

#endif
