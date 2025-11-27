#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint>
#include <functional>
#include <fstream>
#include <array>
#include <algorithm>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
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

template<typename CharType, int StringSize>
class FixedString {
private:
    CharType str_[StringSize];
public:
    FixedString() {
        memset(str_, 0, sizeof(str_));
    }
    FixedString(const char* str) {
        memset(str_, 0, sizeof(str_));
        const char* pos = str;
        int index = 0;
        while(*pos) {
            str_[index] = *pos;
            pos++;
            index++;
            if (index == StringSize) {
                return;
            }
        }
    }
    FixedString(const char* str, int size_) {
        memset(str_, 0, sizeof(str_));
        for (int i = 0; i < size_ && i < StringSize; i++) {
            str_[i] = str[i];
        }
    }
    FixedString(const std::string& str) {
        memset(str_, 0, sizeof(str_));
        for (int i = 0; i < str.size() && i < StringSize; i++) {
            str_[i] = str[i];
        }
    }
    bool operator==(const FixedString& oth) const {
        for (int i = 0; i < StringSize; i++) {
            if (str_[i] != oth.str_[i]) {
                return 0;
            }
        }
        return 1;
    }
    CharType operator[](int index) const {
        if (index < 0 || index >= StringSize) {
            return CharType();
        }
        return str_[index];
    }
    CharType& operator[](int index) {
        static CharType dummy = CharType();
        if (index < 0 || index >= StringSize) {
            return dummy;
        }
        return str_[index];
    }
    FixedString& operator=(const char* str) {
        memset(str_, 0, sizeof(str_));
        const char* pos = str;
        int index = 0;
        while(*pos) {
            str_[index] = *pos;
            pos++;
            index++;
            if (index == StringSize) {
                return *this;
            }
        }
        return *this;
    }
    FixedString& operator=(const std::string& str) {
        memset(str_, 0, sizeof(str_));
        for (int i = 0; i < str.size() && i < StringSize; i++) {
            str_[i] = str[i];
        }
        return *this;
    }
    FixedString& operator=(const FixedString& oth) {
        memcpy(str_, oth.str_, StringSize * sizeof(CharType));
        return *this;
    }
    CharType* serialize() const {
        CharType* str = new CharType[StringSize]();
        memcpy(str, str_, StringSize * sizeof(CharType));
        return str;
    }
    friend std::istream& operator>>(std::istream& is, FixedString& str) {
        std::string s;
        is >> s;
        str = s;
        return is;
    }
    friend std::ostream& operator<<(std::ostream& os, const FixedString& str) {
        for (int i = 0; i < StringSize; i++) {
            if (str.str_[i]) {
                os << str.str_[i];
            }
            else {
                break;
            }
        }
        return os;
    }
};

template<typename HashType>
struct MemoryHash {
    size_t operator()(const HashType& obj) const {
        return std::hash<std::string>{}(std::string(reinterpret_cast<const char *>(&obj), sizeof(HashType)));
    }
};

template<typename KeyType, typename ValueType, typename Hasher = MemoryHash<KeyType>, int BlockCapacity = 200, int HashSize = 199999>
class HashMap {
private:
    struct Bucket {
        int head_;
        int tail_;
        Bucket() : head_(0), tail_(0) {}
    };
    struct KeyPair {
        KeyType key_;
        ValueType val_;
        KeyPair() : key_(), val_() {}
    };
    struct Block {
        KeyPair data_[BlockCapacity];
        int size_;
        int next_;
        Block() : size_(0), next_(0) {}
    };

    int hash(const KeyType& key) {
        return hasher_(key) % HashSize;
    }

    MemoryRiver<Bucket> bucket_file_;
    MemoryRiver<Block> block_file_;

    std::string file_name_;

    Hasher hasher_;

    int new_block() {
        Block block;
        int pos =  block_file_.write(block);
        // std::cerr << "new block numbered " << pos << std::endl;
        return pos;
    }

public:
    HashMap(const std::string& file_name = "hash") : file_name_(file_name), hasher_() {
        bool f = bucket_file_.initialise(file_name_ + "_bucket");
        block_file_.initialise(file_name_ + "_block");
        if (!f) {
            Bucket bucket;
            for (int i = 0; i < HashSize; i++) {
                bucket_file_.write(bucket);
            }
        }
    }

    void insert(const KeyType& key, const ValueType& val) {
        int hash_val = hash(key);
        // std::cerr << "hash value " << hash_val << std::endl;
        Bucket bucket;
        bucket_file_.read(bucket, hash_val);
        Block block;
        int pos = bucket.head_;
        while (pos) {
            block_file_.read(block, pos);
            for (int i = 0; i < block.size_; i++) {
                if (block.data_[i].key_ == key && block.data_[i].val_ == val) {
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
            new_block.data_[0].key_ = key;
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
                tail_block.data_[tail_block.size_].key_ = key;
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
                new_block.data_[0].key_ = key;
                new_block.data_[0].val_ = val;
                tail_block.next_ = new_pos;
                block_file_.update(tail_block, bucket.tail_);
                block_file_.update(new_block, new_pos);
                bucket.tail_ = new_pos;
                bucket_file_.update(bucket, hash_val);
            }
        }
    }

    void erase(const KeyType& key, const ValueType& val) {
        int hash_val = hash(key);
        Bucket bucket;
        bucket_file_.read(bucket, hash_val);
        Block block;
        int pos = bucket.head_, last = -1;
        while (pos) {
            block_file_.read(block, pos);
            int found_pos = -1;
            for (int i = 0; i < block.size_; i++) {
                if (block.data_[i].key_ == key && block.data_[i].val_ == val) {
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

    std::vector<ValueType> find(const KeyType& key) {
        int hash_val = hash(key);
        Bucket bucket;
        bucket_file_.read(bucket, hash_val);
        Block block;
        int pos = bucket.head_;
        std::vector<ValueType> vec;
        while (pos) {
            block_file_.read(block, pos);
            for (int i = 0; i < block.size_; i++) {
                if (block.data_[i].key_ == key) {
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

void read_arr(array<char, 64>& arr) {
    string s;
    cin >> s;
    for (int i = 0; i < 64; i++) {
        if (i < s.size()) {
            arr[i] = s[i];
        }
        else {
            arr[i] = 0;
        }
    }
}

int main() {
    int t;
    cin >> t;
    HashMap<array<char, 64>, int, MemoryHash<array<char, 64> >, 200, 1007> hshmp;
    while (t--) {
        string op, str;
        array<char, 64> s;
        int x;
        cin >> op;
        if (op == "insert") {
            read_arr(s);
            cin >> x;
            hshmp.insert(s, x);
        }
        else if (op == "delete") {
            read_arr(s);
            cin >> x;
            hshmp.erase(s, x);
        }
        else if (op == "find") {
            read_arr(s);
            std::vector<int> vec = hshmp.find(s);
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
    return 0;
}