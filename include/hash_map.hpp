#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <cstring>
#include <cstdint>
#include <functional>
#include <algorithm>
#include "memory_river.hpp"

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
    HashMap(const std::string& file_name = "hash.dat") : file_name_(file_name), hasher_() {
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

    void erase(const KeyType& key) {
        int hash_val = hash(key);
        Bucket bucket;
        file_.read_bucket(bucket, hash_val);
        Block block;
        int pos = bucket.head_, last = -1;
        while (pos) {
            file_.read_block(block, pos);
            int found_pos = -1;
            for (int i = 0; i < block.size_; i++) {
                if (block.data_[i].key_ == key) {
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

    int count(const KeyType& key) {
        int hash_val = hash(key);
        Bucket bucket;
        file_.read_bucket(bucket, hash_val);
        Block block;
        int pos = bucket.head_;
        int cnt = 0;
        while (pos) {
            file_.read_block(block, pos);
            for (int i = 0; i < block.size_; i++) {
                if (block.data_[i].key_ == key) {
                    cnt++;
                }
            }
            pos = block.next_;
        }
        return cnt;
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
        // std::sort(vec.begin(), vec.end());
        return vec;
    }
};

#endif