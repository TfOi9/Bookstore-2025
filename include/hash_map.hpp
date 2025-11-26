#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include "memory_river.hpp"

template<typename ValueType, int BlockCapacity = 200, int HashSize = 199999, int StringSize = 64>
class HashMap {
    typedef long long ll;
private:
    struct Bucket {
        ll head_;
        ll tail_;
        Bucket() : head_(0), tail_(0) {}
    };
    struct KeyPair {
        char str_[StringSize];
        ValueType val_;
        KeyPair() : val_() {
            memset(str_, 0, sizeof(str_));
        }
    };
    struct BlockHead {
        int size_;
        ll next_;
        BlockHead() : size_(0), next_(0) {}
    };
    struct Block {
        BlockHead head_;
        KeyPair data_[BlockCapacity];
        Block() : head_() {
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

    ll new_block() {
        Block block;
        ll pos =  block_file_.write(block);
        // std::cerr << "new block numbered " << pos << std::endl;
        return pos;
    }

public:
    HashMap(const std::string& file_name = "hash") : file_name_(file_name) {
        bool f = bucket_file_.initialise(file_name_ + "_bucket");
        block_file_.initialise(file_name_ + "block");
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
        bucket_file_.read(bucket, hash_val * sizeof(Bucket));
        Block block;
        ll pos = bucket.head_;
        while (pos) {
            block_file_.read(block, pos);
            for (int i = 0; i < block.head_.size_; i++) {
                if (strcmp(block.data_[i].str_, str) == 0 && block.data_[i].val_ == val) {
                    return;
                }
            }
            pos = block.head_.next_;
        }
        if (!bucket.tail_) {
            // empty bucket, create a new block
            ll new_pos = new_block();
            Block new_block;
            new_block.head_.size_ = 1;
            new_block.head_.next_ = 0;
            strncpy(new_block.data_[0].str_, str, 64);
            new_block.data_[0].val_ = val;
            bucket.head_ = new_pos;
            bucket.tail_ = new_pos;
            bucket_file_.update(bucket, hash_val * sizeof(Bucket));
            block_file_.update(new_block, new_pos);
        }
        else {
            Block tail_block;
            block_file_.read(tail_block, bucket.tail_);
            if (tail_block.head_.size_ < BlockCapacity) {
                // tail has space, put data into tail block
                strncpy(tail_block.data_[tail_block.head_.size_].str_, str, 64);
                tail_block.data_[tail_block.head_.size_].val_ = val;
                tail_block.head_.size_++;
                block_file_.update(tail_block, bucket.tail_);
            }
            else {
                // tail is full, create a new block
                ll new_pos = new_block();
                Block new_block;
                new_block.head_.size_ = 1;
                new_block.head_.next_ = 0;
                strncpy(new_block.data_[0].str_, str, 64);
                new_block.data_[0].val_ = val;
                tail_block.head_.next_ = new_pos;
                block_file_.update(tail_block, bucket.tail_);
                block_file_.update(new_block, new_pos);
                bucket.tail_ = new_pos;
                bucket_file_.update(bucket, hash_val * sizeof(Bucket));
            }
        }
    }

    void erase(const char* str, int size, const ValueType& val) {
        int hash_val = hash(str, size);
        Bucket bucket;
        bucket_file_.read(bucket, hash_val * sizeof(Bucket));
        Block block;
        ll pos = bucket.head_, last = -1;
        while (pos) {
            block_file_.read(block, pos);
            ll found_pos = -1;
            for (int i = 0; i < block.head_.size_; i++) {
                if (strcmp(block.data_[i].str_, str) == 0 && block.data_[i].val_ == val) {
                    found_pos = i;
                    break;
                }
            }
            if (found_pos != -1) {
                // found keypair in block
                if (block.head_.size_ == 1) {
                    // only one pair in block, delete it
                    if (last == -1) {
                        // first block in bucket
                        bucket.head_ = block.head_.next_;
                        if (bucket.head_ == 0) {
                            // bucket now empty
                            bucket.tail_ = 0;
                        }
                        bucket_file_.update(bucket, hash_val * sizeof(Bucket));
                    }
                    else {
                        // block in the middle
                        Block last_block;
                        block_file_.read(last_block, last);
                        last_block.head_.next_ = block.head_.next_;
                        block_file_.update(last_block, last);
                        if (block.head_.next_ == 0) {
                            // last block
                            bucket.tail_ = last;
                            bucket_file_.update(bucket, hash_val * sizeof(Bucket));
                        }
                    }
                    return;
                }
                for (int i = found_pos; i < block.head_.size_ - 1; i++) {
                    block.data_[i] = block.data_[i + 1];
                }
                block.head_.size_--;
                block_file_.update(block, pos);
                return;
            }
            last = pos;
            pos = block.head_.next_;
        }
    }

    std::vector<ValueType> find(const char* str, int size) {
        int hash_val = hash(str, size);
        Bucket bucket;
        bucket_file_.read(bucket, hash_val * sizeof(Bucket));
        Block block;
        ll pos = bucket.head_;
        std::vector<ValueType> vec;
        while (pos) {
            block_file_.read(block, pos);
            for (int i = 0; i < block.head_.size_; i++) {
                if (strcmp(block.data_[i].str_, str) == 0) {
                    vec.push_back(block.data_[i].val_);
                }
            }
            pos = block.head_.next_;
        }
        std::sort(vec.begin(), vec.end());
        return vec;
    } 

};

#endif