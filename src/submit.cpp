#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include <fstream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    /* your code here */
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    int info_offset = info_len * sizeof(int);

    int size_;
public:
    MemoryRiver() : size_(1) {}

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    ~MemoryRiver() {
        write_info(size_, 1);
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
        bool f = open_file();
        if (f) {
            get_info(size_, 1);
        }
        return f;
    }

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

    int write(T &t) {
        int siz = size_;
        size_++;
        file.seekp(info_offset + siz * sizeofT);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        return siz;
    }

    void update(T &t, const int pos) {
        file.seekp(info_offset + pos * sizeofT);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
    }

    void read(T &t, const int pos) {
        file.seekg(info_offset + pos * sizeofT);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
    }
};

template<typename ValueType, int BlockCapacity = 200, int HashSize = 1999, int StringSize = 64>
class HashMap {
private:
    struct Bucket {
        int head_;
        int tail_;
        Bucket() : head_(0), tail_(0) {}
    };
    struct KeyPair {
        char str_[StringSize];
        ValueType val_;
        KeyPair() : val_() {
            memset(str_, 0, sizeof(str_));
        }
    };
    struct Block {
        KeyPair data_[BlockCapacity];
        int size_;
        int next_;
        Block() : size_(0), next_(0) {
            memset(data_, 0, sizeof(data_));
        }
    };

    int hash(const char* str, int size) {
        uint64_t val = 0ull;
        for (int i = 0; i < size; i++) {
            val = val * 131ull + uint64_t(str[i]);
        }
        return int(val % uint64_t(2147483647)) % HashSize;
    }

    MemoryRiver<Bucket> bucket_file_;
    MemoryRiver<Block> block_file_;

    std::string file_name_;

    int new_block() {
        Block block;
        int pos =  block_file_.write(block);
        // std::cerr << "new block numbered " << pos << std::endl;
        return pos;
    }

public:
    HashMap(const std::string& file_name = "hash") : file_name_(file_name) {
        bool f = bucket_file_.initialise(file_name_ + "_bucket");
        block_file_.initialise(file_name_ + "_block");
        if (!f) {
            Bucket bucket;
            for (int i = 0; i < HashSize; i++) {
                bucket_file_.write(bucket);
            }
        }
    }

    void insert(const char* str, int size, const ValueType& val) {
        int hash_val = hash(str, size);
        Bucket bucket;
        bucket_file_.read(bucket, hash_val);
        Block block;
        int pos = bucket.head_;
        while (pos) {
            block_file_.read(block, pos);
            for (int i = 0; i < block.size_; i++) {
                if (strcmp(block.data_[i].str_, str) == 0 && block.data_[i].val_ == val) {
                    return;
                }
            }
            pos = block.next_;
        }
        if (!bucket.tail_) {
            // empty bucket, create a new block
            int new_pos = new_block();
            Block new_block;
            new_block.size_ = 1;
            new_block.next_ = 0;
            strncpy(new_block.data_[0].str_, str, 64);
            new_block.data_[0].val_ = val;
            bucket.head_ = new_pos;
            bucket.tail_ = new_pos;
            bucket_file_.update(bucket, hash_val);
            block_file_.update(new_block, new_pos);
        }
        else {
            Block tail_block;
            block_file_.read(tail_block, bucket.tail_);
            if (tail_block.size_ < BlockCapacity) {
                // tail has space, put data into tail block
                strncpy(tail_block.data_[tail_block.size_].str_, str, 64);
                tail_block.data_[tail_block.size_].val_ = val;
                tail_block.size_++;
                block_file_.update(tail_block, bucket.tail_);
            }
            else {
                // tail is full, create a new block
                int new_pos = new_block();
                Block new_block;
                new_block.size_ = 1;
                new_block.next_ = 0;
                strncpy(new_block.data_[0].str_, str, 64);
                new_block.data_[0].val_ = val;
                tail_block.next_ = new_pos;
                block_file_.update(tail_block, bucket.tail_);
                block_file_.update(new_block, new_pos);
                bucket.tail_ = new_pos;
                bucket_file_.update(bucket, hash_val);
            }
        }
    }

    void erase(const char* str, int size, const ValueType& val) {
        int hash_val = hash(str, size);
        Bucket bucket;
        bucket_file_.read(bucket, hash_val);
        Block block;
        int pos = bucket.head_, last = -1;
        while (pos) {
            block_file_.read(block, pos);
            int found_pos = -1;
            for (int i = 0; i < block.size_; i++) {
                if (strcmp(block.data_[i].str_, str) == 0 && block.data_[i].val_ == val) {
                    found_pos = i;
                    break;
                }
            }
            if (found_pos != -1) {
                // found keypair in block
                if (block.size_ == 1) {
                    // only one pair in block, delete it
                    if (last == -1) {
                        // first block in bucket
                        bucket.head_ = block.next_;
                        if (bucket.head_ == 0) {
                            // bucket now empty
                            bucket.tail_ = 0;
                        }
                        bucket_file_.update(bucket, hash_val);
                    }
                    else {
                        // block in the middle
                        Block last_block;
                        block_file_.read(last_block, last);
                        last_block.next_ = block.next_;
                        block_file_.update(last_block, last);
                        if (block.next_ == 0) {
                            // last block
                            bucket.tail_ = last;
                            bucket_file_.update(bucket, hash_val);
                        }
                    }
                    return;
                }
                for (int i = found_pos; i < block.size_ - 1; i++) {
                    block.data_[i] = block.data_[i + 1];
                }
                block.size_--;
                block_file_.update(block, pos);
                return;
            }
            last = pos;
            pos = block.next_;
        }
    }

    std::vector<ValueType> find(const char* str, int size) {
        int hash_val = hash(str, size);
        Bucket bucket;
        bucket_file_.read(bucket, hash_val);
        Block block;
        int pos = bucket.head_;
        std::vector<ValueType> vec;
        while (pos) {
            block_file_.read(block, pos);
            for (int i = 0; i < block.size_; i++) {
                if (strcmp(block.data_[i].str_, str) == 0) {
                    vec.push_back(block.data_[i].val_);
                }
            }
            pos = block.next_;
        }
        std::sort(vec.begin(), vec.end());
        return vec;
    } 

};

using namespace std;

int main() {
    int t;
    cin >> t;
    HashMap<int, 200, 1007> hshmp;
    while (t--) {
        string op, s;
        int x;
        cin >> op;
        if (op == "insert") {
            cin >> s >> x;
            hshmp.insert(s.c_str(), s.size(), x);
        }
        else if (op == "delete") {
            cin >> s >> x;
            hshmp.erase(s.c_str(), s.size(), x);
        }
        else if (op == "find") {
            cin >> s;
            std::vector<int> vec = hshmp.find(s.c_str(), s.size());
            if (!vec.size()) {
                cout << "null" << endl;
                continue;
            }
            for (int i = 0; i < vec.size(); i++) {
                cout << vec[i] << " ";
            }
            cout << endl;
        }
    }
}