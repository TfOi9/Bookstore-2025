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

    void open_file() {
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) {
            file.open(file_name, std::ios::out | std::ios::binary);
            file.close();
            file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        }
    }

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        open_file();
        if (!auto_clear) {
            return;
        }
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        if (!file.is_open()) {
            open_file();
        }
        if (!file) {
            return;
        }
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        if (!file.is_open()) {
            open_file();
        }
        if (!file) {
            return;
        }
        file.seekp((n - 1) * sizeof(int));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
    }

    int size() {
        int tmp;
        get_info(tmp, 1);
        return tmp;
    }

    void expand(int size) {
        write_info(size, 1);
    }

    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T &t) {
        int pos = size();
        if (!file.is_open()) {
            open_file();
        }
        if (!file) {
            return -1;
        }
        file.seekp(info_offset + pos * sizeofT);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        expand(pos + 1);
        return pos;
    }

    //用t的值更新位置索引index对应的对象，保证调用的index都是由write函数产生
    void update(T &t, const int index) {
        if (!file.is_open()) {
            open_file();
        }
        if (!file) {
            return;
        }
        file.seekp(info_offset + index * sizeofT);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        if (!file.is_open()) {
            open_file();
        }
        if (!file) {
            return;
        }
        file.seekg(info_offset + index * sizeofT);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {
        /* your code here */
    }
};


#endif //BPT_MEMORYRIVER_HPP