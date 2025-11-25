#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <algorithm>
#include "memory_river.hpp"

template<typename ValueType, int BlockCapacity = 200, int HashSize = 10007>
class HashMap {
private:
    struct Bucket {
        int head_;
        int tail_;
        Bucket() : head_(0), tail_(0) {}
    };
    struct KeyPair {
        char str_[64];
        ValueType val_;
        KeyPair() : val_() {
            memset(str_, 0, sizeof(str_));
        }
    };
    struct BlockHead {
        int size_;
        int next_;
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
        return int(val) % HashSize;
    }

    MemoryRiver<Bucket> bucket_file_;
    MemoryRiver<Block> block_file_;

    int new_block() {
        Block block;
        return block_file_.write(block);
    }

public:
    HashMap() {
        bool f = bucket_file_.initialise("bucket");
        block_file_.initialise("block");
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
            for (int i = 0; i < block.head_.size_; i++) {
                if (strcmp(block.data_[i].str_, str) == 0 && block.data_[i].val_ == val) {
                    return;
                }
            }
            pos = block.head_.next_;
        }
        if (!bucket.tail_) {
            // empty bucket, create a new block
            int new_pos = new_block();
            Block block;
            block.head_.size_ = 1;
            block.head_.next_ = 0;
            strncpy(block.data_[0].str_, str, 64);
            block.data_[0].val_ = val;
            bucket.head_ = new_pos;
            bucket.tail_ = new_pos;
            bucket_file_.update(bucket, hash_val);
            block_file_.update(block, new_pos);
        }
        else {
            Block tail_block;
            block_file_.read(tail_block, bucket.tail_);
            if (tail_block.head_.size_ < BlockCapacity) {
                // tail has space, put data into tail block
                strncpy(block.data_[tail_block.head_.size_].str_, str, 64);
                block.data_[tail_block.head_.size_].val_ = val;
                tail_block.head_.size_++;
                block_file_.update(tail_block, bucket.tail_);
            }
            else {
                // tail is full, create a new block
                int new_pos = new_block();
                Block block;
                block.head_.size_ = 1;
                block.head_.next_ = 0;
                strncpy(block.data_[0].str_, str, 64);
                block.data_[0].val_ = val;
                tail_block.head_.next_ = new_pos;
                block_file_.update(tail_block, bucket.tail_);
                block_file_.update(block, new_pos);
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
                        bucket_file_.update(bucket, hash_val);
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
        bucket_file_.read(bucket, hash_val);
        Block block;
        int pos = bucket.head_;
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