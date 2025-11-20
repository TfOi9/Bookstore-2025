#ifndef UNROLLED_LINKEDLIST_HPP
#define UNROLLED_LINKEDLIST_HPP

#include <iostream>
#include <vector>
#include <utility>
#include "memory_river.hpp"

template <typename KeyType, typename ValueType, int BlockCapacity = 100>
class UnrolledLinkedList {
private:
    struct Block {
        std::pair<KeyType, ValueType> data_[BlockCapacity];
        KeyType min_key_;
        KeyType max_key_;
        int size_ = 0;
        int index_ = -1;
        int next_block_ = -1;
        int prev_block_ = -1;
    };

    MemoryRiver<Block> block_file_;
    int head_index_ = 0;
    int total_blocks_ = 0;
    int total_size_ = 0;

    bool read_block(Block& block, int index) {
        if (index < 0 || index >= total_blocks_) {
            return 0;
        }
        block_file_.read(block, index);
        return 1;
    }

    bool write_block(Block& block, int index = -1) {
        if (index < 0) {
            total_blocks_++;
            block_file_.write(block);
        }
        else {
            block_file_.update(block, index);
        }
    }

    void find_block(const KeyType& key) const {
        
    }

    bool block_insert(Block& block, const KeyType& key, const ValueType& value) {
        if (block.size_ == BlockCapacity) {
            return 0;
        }
        int pos = 0;
        std::pair<KeyType, ValueType> p = std::make_pair(key, value);
        while (pos < block.size_ && block.data_[pos] < p) {
            pos++;
        }
        if (pos == 0) {
            block.min_key_ = key;
        }
        else if (pos == block.size_) {
            block.max_key_ = key;
        }
        for (int i = block.size_ - 1; i >= pos; i--) {
            block.data_[pos + 1] = block.data_[pos];
        }
        block.data_[pos] = value;
        block.size_++;
        total_size_++;
        write_block(block, block.index_);
        return 1;
    }

    void block_split(Block& block) {
        int mid = block.size_ / 2;
        Block new_block;
        new_block.size_ = block.size_ - mid;
        new_block.index_ = total_blocks_;
        total_blocks_++;
        for (int i = mid; i < block.size_; i++) {
            new_block[i - mid] = block[i];
        }
        block.size_ = mid;
        block.max_key_ = block.data_[mid - 1].first;
        new_block.min_key_ = new_block.data[0].first;
        new_block.max_key_ = new_block.data[new_block.size_ - 1].first;
        new_block.next_block_ = block.next_block_;
        block.next_block_ = new_block.index_;
        new_block.prev_block_ = block.index_;
        write_block(block, block.index_);
        write_block(new_block, new_block.index_);
    }

    int block_lower_bound(Block& block, const KeyType& key) const {
        int l = 0, r = block.size_ - 1, mid = 0, ret = 0;
        while (l <= r) {
            mid = (l + r) / 2;
            if (block.data_[mid].first < key) {
                ret = mid;
                l = mid + 1;
            }
            else {
                r = mid - 1;
            }
        }
        return ret;
    }

    int block_upper_bound(Block& block, const KeyType& key) const {
        int l = 0, r = block.size_ - 1, mid = 0, ret = block.size_;
        while (l <= r) {
            mid = (l + r) / 2;
            if (key < block.data_[mid].first) {
                ret = r;
                r = mid - 1;
            }
            else {
                l = mid + 1;
            }
        }
        return ret;
    }

    void block_delete(Block& block) {
        Block prev_block, next_block;
        int prev_index = -1, next_index = -1;
        if (block.prev_block_ != -1) {
            read_block(prev_block, block.prev_block_);
            prev_index = prev_block.index_;
        }
        if (block.next_block_ != -1) {
            read_block(next_block, block.next_block_);
            next_index = next_block.index_;
        }
        if (prev_index != -1) {
            prev_block.next_block_ = next_index;
            write_block(prev_block, prev_index);
        }
        if (next_index != -1) {
            next_block.prev_block_ = prev_index;
            write_block(next_block, next_index);
        }
        total_blocks_--;
    }

    void block_erase(Block& block, const KeyType& key) {
        int l = block_lower_bound(block, key);
        int r = block_upper_bound(block, key);
        int del_size = r - l;
        if (!del_size) {
            return;
        }
        if (del_size == block.size_) {
            block_delete(block);
            return;
        }
        for (int i = r; i < block.size_; i++) {
            block.data_[i - del_size] = block.data_[i];
        }
        block.size_ -= del_size;
        total_size_ -= del_size;
        block.min_key_ = block.data_[0];
        block.max_key_ = block.data_[block.size_ - 1];
        write_block(block, block.index_);
    }

    void block_find(Block& block, const KeyType& key, std::ostream& os) const {
        int l = block_lower_bound(block, key);
        int r = block_upper_bound(block, key);
        for (int i = l; i < r; i++) {
            os << block.data_[i].second << " ";
        }
        os << std::endl;
    }

    void block_print(Block& block, std::ostream& os) const {
        os << "Block index: " << block.index_ << std::endl;
        os << "Block size: " << block.size_ << std::endl;
        os << "Next block: Block " << block.next_block_ << std::endl;
        os << "Previous block: Block " << block.prev_block_ << std::endl;
        os << "Minimum key: " << block.min_key_ << std::endl;
        os << "Maximum key: " << block.max_key_ << std::endl;
        os << "Block elements: " << std::endl;
        for (int i = 0; i < block.size_; i++) {
            os << block.data_[i] << " ";
        }
        os << std::endl;
    }

public:
    void insert(const KeyType& key, const ValueType& value) {

    }
};

#endif