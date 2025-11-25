#ifndef MEMORY_RIVER_HPP
#define MEMORY_RIVER_HPP

#include <iostream>
#include <fstream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2, bool auto_clear = 0>
class MemoryRiver {
private:
    /* your code here */
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    int info_offset = info_len * sizeof(int);
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    ~MemoryRiver() {
        if (file.is_open()) {
            file.close();
        }
    }

    bool open_file() {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) {
            file.open(file_name, std::ios::out | std::ios::binary);
            file.close();
            file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
            return 0;
        }
        return 1;
    }

    bool initialise(string FN = "") {
        if (FN != "") file_name = FN;
        return open_file();
    }

    int write(T &t) {
        file.seekp(0, std::ios::end);
        int pos = file.tellp();
        file.write(reinterpret_cast<const char*>(&t), sizeof(T));
        return pos + 1;
    }

    void update(T &t, const int pos) {
        file.seekp(pos - 1);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
    }

    void read(T &t, const int pos) {
        file.seekg(pos - 1);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
    }
};


#endif