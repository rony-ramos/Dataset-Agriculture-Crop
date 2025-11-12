#ifndef CSVREADER_CPP
#define CSVREADER_CPP

#include <fstream>
#include <sstream>
#include "DatasetReader.cpp"

using namespace std;

static vector<string> parseCSVLine(const string& line) {
    vector<string> result;
    stringstream ss(line);
    string item;
    while(getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

class CSVReader : public DatasetReader {
    vector<string> hdr;
    LinkedList<vector<string>> data;
public:
    bool load(const string& path) override {
        ifstream ifs(path);
        if (!ifs.is_open()) return false;
        string line;
        if (getline(ifs, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            hdr = parseCSVLine(line);
        }
        while (getline(ifs, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            data.push_back(parseCSVLine(line));
        }
        return true;
    }
    vector<string> header() const override { return hdr; }
    const LinkedList<vector<string>>& rows() const override { return data; }
};

#endif
