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
    int head_index_ = -1;
    int total_blocks_ = 0;
    int total_size_ = 0;

    bool read_block(Block& block, int index) {
        if (index < 0) {
            return 0;
        }
        block_file_.read(block, index);
        return 1;
    }

    void write_block(Block& block, int index = -1) {
        if (index < 0) {
            total_blocks_++;
            block_file_.write(block);
        }
        else {
            block_file_.update(block, index);
        }
    }

    Block create_block(const KeyType& key, const ValueType& value) {
        Block new_block;
        new_block.data_[0] = std::make_pair(key, value);
        new_block.size_ = 1;
        new_block.index_ = total_blocks_;
        total_blocks_++;
        total_size_++;
        new_block.min_key_ = key;
        new_block.max_key_ = key;
        return new_block;
    }

    int find_block(const KeyType& key) {
        if (total_blocks_ <= 1) {
            std::cerr << "no blocks yet" << std::endl;
            return -1;
        }
        Block cur_block, next_block;
        read_block(cur_block, head_index_);
        read_block(next_block, cur_block.next_block_);
        while (next_block.next_block_ != -1) {
            Block temp_block;
            read_block(temp_block, next_block.next_block_);
            if (key < temp_block.min_key_) {
                return next_block.index_;
            }
            next_block = temp_block;
        }
        return next_block.index_;
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
            block.data_[i + 1] = block.data_[i];
        }
        block.data_[pos] = std::make_pair(key, value);
        block.size_++;
        total_size_++;
        // for (int i = 0; i < block.size_; i++) {
        //     std::cerr << block.data_[i].first << "/" << block.data_[i].second << std::endl;
        // }
        write_block(block, block.index_);
        return 1;
    }

    void block_split(Block& block) {
        std::cerr << "spliting block " << block.index_ << std::endl;
        int mid = block.size_ / 2;
        std::cerr << "spliting from pos " << mid << std::endl;
        Block new_block;
        new_block.size_ = block.size_ - mid;
        new_block.index_ = total_blocks_;
        total_blocks_++;
        for (int i = mid; i < block.size_; i++) {
            new_block.data_[i - mid] = block.data_[i];
        }
        block.size_ = mid;
        block.max_key_ = block.data_[mid - 1].first;
        new_block.min_key_ = new_block.data_[0].first;
        new_block.max_key_ = new_block.data_[new_block.size_ - 1].first;
        new_block.next_block_ = block.next_block_;
        block.next_block_ = new_block.index_;
        new_block.prev_block_ = block.index_;
        write_block(block, block.index_);
        write_block(new_block, new_block.index_);
    }

    int block_lower_bound(Block& block, const KeyType& key) const {
        int l = 0, r = block.size_ - 1, mid = 0;
        while (l <= r) {
            mid = (l + r) / 2;
            if (block.data_[mid].first < key) {
                l = mid + 1;
            }
            else {
                r = mid - 1;
            }
        }
        return l;
    }

    int block_upper_bound(Block& block, const KeyType& key) const {
        int l = 0, r = block.size_ - 1, mid = 0;
        while (l <= r) {
            mid = (l + r) / 2;
            if (key < block.data_[mid].first) {
                r = mid - 1;
            }
            else {
                l = mid + 1;
            }
        }
        return l;
    }

    void block_delete(Block& block) {
        if (block.prev_block_ != -1) {
            Block prev_block;
            read_block(prev_block, block.prev_block_);
            prev_block.next_block_ = block.next_block_;
            write_block(prev_block, block.prev_block_);
        }
        if (block.next_block_ != -1) {
            Block next_block;
            read_block(next_block, block.next_block_);
            next_block.prev_block_ = block.prev_block_;
            write_block(next_block, block.next_block_);
        }
        total_blocks_--;
    }

    void block_erase(Block& block, const KeyType& key) {
        int l = block_lower_bound(block, key);
        int r = block_upper_bound(block, key);
        std::cerr << "lbound: " << l << ", rbound: " << r << std::endl;
        int del_size = r - l;
        if (!del_size) {
            return;
        }
        if (del_size == block.size_) {
            total_size_ -= del_size;
            block_delete(block);
            return;
        }
        for (int i = r; i < block.size_; i++) {
            block.data_[i - del_size] = block.data_[i];
        }
        block.size_ -= del_size;
        total_size_ -= del_size;
        block.min_key_ = block.data_[0].first;
        block.max_key_ = block.data_[block.size_ - 1].first;
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
            os << block.data_[i].first << "->" << block.data_[i].second << " ";
        }
        os << std::endl;
    }

public:
    UnrolledLinkedList() {
        block_file_.initialise("block_file");
        Block head;
        head_index_ = 0;
        head.index_ = 0;
        write_block(head, 0);
        total_blocks_++;
    }

    void insert(const KeyType& key, const ValueType& value) {
        int index = find_block(key);
        if (index == -1) {
            Block new_block = create_block(key, value);
            Block head_block;
            read_block(head_block, 0);
            head_block.next_block_ = new_block.index_;
            new_block.prev_block_ = head_block.index_;
            write_block(head_block, 0);
            write_block(new_block, new_block.index_);
        }
        else {
            Block block;
            read_block(block, index);
            if (!block_insert(block, key, value)) {
                block_split(block);
                if (block.max_key_ < key) {
                    Block next_block;
                    read_block(next_block, block.next_block_);
                    block_insert(next_block, key, value);
                }
                else {
                    block_insert(block, key, value);
                }
            }
        }
    }

    void erase(const KeyType& key) {
        int index = find_block(key);
        if (index == -1) {
            return;
        }
        Block block;
        read_block(block, index);
        block_erase(block, key);
    }

    void find(const KeyType& key) {
        int index = find_block(key);
        if (index == -1) {
            return;
        }
        Block block;
        read_block(block, index);
        block_find(block, key, std::cout);
    }

    void print(std::ostream& os) {
        os << "Total blocks: " << total_blocks_ << std::endl;
        os << "Total size: " << total_size_ << std::endl;
        os << "Head index: " << head_index_ << std::endl;
        if (head_index_ == -1) {
            return;
        }
        Block cur_block;
        read_block(cur_block, head_index_);
        while (1) {
            block_print(cur_block, os);
            if (cur_block.next_block_ == -1) {
                break;
            }
            int id = cur_block.next_block_;
            std::cerr << "reading block " << id << std::endl;
            bool f = read_block(cur_block, id);
            std::cerr << "valid " << f << std::endl;
            if (!f) break;
        }
    }
};

#endif