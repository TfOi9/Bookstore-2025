#ifndef HASH_MAP_HPP
#define HASH_MAP_HPP

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include "memory_river.hpp"

template<typename ValueType, int BlockCapacity = 200>
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

    uint64_t hash(const char* str, int size) {
        uint64_t val = 0ull;
        for (int i = 0; i < size; i++) {
            val = val * 131ull + uint64_t(str[i]);
        }
        return val;
    }

    MemoryRiver<Bucket> bucket_file_;
    MemoryRiver<Block> block_file_;

    int new_block() {
        Block block;
        return block_file_.write(block);
    }

};

#endif