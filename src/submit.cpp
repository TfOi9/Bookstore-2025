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

template<typename BucketType, typename BlockType, int info_len = 2, int HashSize = 1007>
class HashMemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofBucket = sizeof(BucketType);
    int sizeofBlock = sizeof(BlockType);
    int info_offset = info_len * sizeof(int);
    int bucket_offset = info_offset + HashSize * sizeofBucket;
    int size_;
public:
    HashMemoryRiver() : size_(1) {}

    HashMemoryRiver(const string& file_name) : file_name(file_name) {}

    ~HashMemoryRiver() {
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

    void update_bucket(BucketType &t, const int pos) {
        file.seekp(info_offset + pos * sizeofBucket);
        file.write(reinterpret_cast<char *>(&t), sizeofBucket);
    }

    void read_bucket(BucketType &t, const int pos) {
        file.seekg(info_offset + pos * sizeofBucket);
        file.read(reinterpret_cast<char *>(&t), sizeofBucket);
    }

    int write_block(BlockType &t) {
        int siz = size_;
        size_++;
        file.seekp(bucket_offset + siz * sizeofBlock);
        file.write(reinterpret_cast<char *>(&t), sizeofBlock);
        return siz;
    }

    void update_block(BlockType &t, const int pos) {
        file.seekp(bucket_offset + pos * sizeofBlock);
        file.write(reinterpret_cast<char *>(&t), sizeofBlock);
    }

    void read_block(BlockType &t, const int pos) {
        file.seekg(bucket_offset + pos * sizeofBlock);
        file.read(reinterpret_cast<char *>(&t), sizeofBlock);
    }
};

template<typename HashType>
struct MemoryHash {
    size_t operator()(const HashType& obj) const {
        return std::hash<std::string>{}(std::string(reinterpret_cast<const char *>(&obj), sizeof(HashType)));
    }
};

template<typename KeyType, typename ValueType, typename Hasher = MemoryHash<KeyType>, int BlockCapacity = 200, int HashSize = 1007>
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

    HashMemoryRiver<Bucket, Block, 2, HashSize> file_;

    std::string file_name_;

    Hasher hasher_;

    int new_block() {
        Block block;
        int pos =  file_.write_block(block);
        // std::cerr << "new block numbered " << pos << std::endl;
        return pos;
    }

public:
    HashMap(const std::string& file_name = "hash") : file_name_(file_name), hasher_() {
        bool f = file_.initialise(file_name_);
        if (!f) {
            Bucket bucket;
            for (int i = 0; i < HashSize; i++) {
                file_.update_bucket(bucket, i);
            }
        }
    }

    ~HashMap() = default;

    void insert(const KeyType& key, const ValueType& val) {
        int hash_val = hash(key);
        // std::cerr << "hash value " << hash_val << std::endl;
        Bucket bucket;
        file_.read_bucket(bucket, hash_val);
        Block block;
        int pos = bucket.head_;
        while (pos) {
            file_.read_block(block, pos);
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
            file_.update_bucket(bucket, hash_val);
            file_.update_block(new_block, new_pos);
        }
        else {
            Block tail_block;
            file_.read_block(tail_block, bucket.tail_);
            if (tail_block.size_ < BlockCapacity) {
                // tail has space, put data into tail block
                tail_block.data_[tail_block.size_].key_ = key;
                tail_block.data_[tail_block.size_].val_ = val;
                tail_block.size_++;
                file_.update_block(tail_block, bucket.tail_);
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
                file_.update_block(tail_block, bucket.tail_);
                file_.update_block(new_block, new_pos);
                bucket.tail_ = new_pos;
                file_.update_bucket(bucket, hash_val);
            }
        }
    }

    void erase(const KeyType& key, const ValueType& val) {
        int hash_val = hash(key);
        Bucket bucket;
        file_.read_bucket(bucket, hash_val);
        Block block;
        int pos = bucket.head_, last = -1;
        while (pos) {
            file_.read_block(block, pos);
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
                        file_.update_bucket(bucket, hash_val);
                    }
                    else {
                        // block in the middle
                        Block last_block;
                        file_.read_block(last_block, last);
                        last_block.next_ = block.next_;
                        file_.update_block(last_block, last);
                        if (block.next_ == 0) {
                            // last block
                            bucket.tail_ = last;
                            file_.update_bucket(bucket, hash_val);
                        }
                    }
                    return;
                }
                for (int i = found_pos; i < block.size_ - 1; i++) {
                    block.data_[i] = block.data_[i + 1];
                }
                block.size_--;
                file_.update_block(block, pos);
                return;
            }
            last = pos;
            pos = block.next_;
        }
    }

    std::vector<ValueType> find(const KeyType& key) {
        int hash_val = hash(key);
        Bucket bucket;
        file_.read_bucket(bucket, hash_val);
        Block block;
        int pos = bucket.head_;
        std::vector<ValueType> vec;
        while (pos) {
            file_.read_block(block, pos);
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