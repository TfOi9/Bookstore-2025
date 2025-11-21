#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <utility>

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

template <typename KeyType, typename ValueType, int BlockCapacity = 100>
class UnrolledLinkedList {
private:
    struct Block {
        std::pair<KeyType, ValueType> data_[BlockCapacity];
        std::pair<KeyType, ValueType> min_key_;
        std::pair<KeyType, ValueType> max_key_;
        int size_ = 0;
        int index_ = -1;
        int next_block_ = -1;
        int prev_block_ = -1;
    };

    MemoryRiver<Block> block_file_;
    int head_index_ = -1;
    int total_blocks_ = 0;
    int total_size_ = 0;
    int spare_index = 0;

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
            spare_index++;
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
        new_block.index_ = spare_index;
        total_blocks_++;
        total_size_++;
        spare_index++;
        new_block.min_key_ = std::make_pair(key, value);
        new_block.max_key_ = std::make_pair(key, value);
        return new_block;
    }

    int find_block(const KeyType& key, const ValueType& value) {
        if (total_blocks_ <= 1) {
            // std::cerr << "no blocks yet" << std::endl;
            return -1;
        }
        auto p = std::make_pair(key, value);
        Block cur_block, next_block;
        read_block(cur_block, head_index_);
        read_block(next_block, cur_block.next_block_);
        while (next_block.next_block_ != -1) {
            Block temp_block;
            read_block(temp_block, next_block.next_block_);
            if (p < temp_block.min_key_) {
                return next_block.index_;
            }
            next_block = temp_block;
        }
        return next_block.index_;
    }

    bool block_contains(Block& block, const KeyType& key, const ValueType& value) {
        int l = block_lower_bound(block, key, value);
        int r = block_upper_bound(block, key, value);
        if (l == r) {
            return 0;
        }
        return 1;
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
            block.min_key_ = p;
        }
        else if (pos == block.size_) {
            block.max_key_ = p;
        }
        for (int i = block.size_ - 1; i >= pos; i--) {
            block.data_[i + 1] = block.data_[i];
        }
        block.data_[pos] = p;
        block.size_++;
        total_size_++;
        // for (int i = 0; i < block.size_; i++) {
        //     std::cerr << block.data_[i].first << "/" << block.data_[i].second << std::endl;
        // }
        write_block(block, block.index_);
        return 1;
    }

    void block_split(Block& block) {
        // std::cerr << "spliting block " << block.index_ << std::endl;
        int mid = block.size_ / 2;
        // std::cerr << "spliting from pos " << mid << std::endl;
        Block new_block;
        new_block.size_ = block.size_ - mid;
        total_blocks_++;
        spare_index++;
        new_block.index_ = spare_index;
        for (int i = mid; i < block.size_; i++) {
            new_block.data_[i - mid] = block.data_[i];
        }
        block.size_ = mid;
        block.max_key_ = block.data_[mid - 1];
        new_block.min_key_ = new_block.data_[0];
        new_block.max_key_ = new_block.data_[new_block.size_ - 1];
        new_block.next_block_ = block.next_block_;
        block.next_block_ = new_block.index_;
        new_block.prev_block_ = block.index_;
        write_block(block, block.index_);
        write_block(new_block, new_block.index_);
    }

    int block_lower_bound(Block& block, const KeyType& key, const ValueType& value) const {
        int l = 0, r = block.size_ - 1, mid = 0;
        auto p = std::make_pair(key, value);
        while (l <= r) {
            mid = (l + r) / 2;
            if (block.data_[mid] < p) {
                l = mid + 1;
            }
            else {
                r = mid - 1;
            }
        }
        return l;
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

    int block_upper_bound(Block& block, const KeyType& key, const ValueType& value) const {
        int l = 0, r = block.size_ - 1, mid = 0;
        auto p = std::make_pair(key, value);
        while (l <= r) {
            mid = (l + r) / 2;
            if (p < block.data_[mid]) {
                r = mid - 1;
            }
            else {
                l = mid + 1;
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

    void block_erase(Block& block, const KeyType& key, const ValueType& value) {
        int l = block_lower_bound(block, key, value);
        int r = block_upper_bound(block, key, value);
        // std::cerr << "lbound: " << l << ", rbound: " << r << std::endl;
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
        block.min_key_ = block.data_[0];
        block.max_key_ = block.data_[block.size_ - 1];
        write_block(block, block.index_);
    }

    bool block_find(Block& block, const KeyType& key, std::ostream& os) const {
        int l = block_lower_bound(block, key);
        int r = block_upper_bound(block, key);
        // std::cerr << l << " " << r << std::endl;
        if (l == r) {
            return 0;
        }
        for (int i = l; i < r; i++) {
            os << block.data_[i].second << " ";
        }
        return 1;
    }

    void block_print(Block& block, std::ostream& os) const {
        os << "Block index: " << block.index_ << std::endl;
        os << "Block size: " << block.size_ << std::endl;
        os << "Next block: Block " << block.next_block_ << std::endl;
        os << "Previous block: Block " << block.prev_block_ << std::endl;
        os << "Minimum key: " << block.min_key_.first << std::endl;
        os << "Maximum key: " << block.max_key_.first << std::endl;
        os << "Block elements: " << std::endl;
        for (int i = 0; i < block.size_; i++) {
            os << block.data_[i].first << "->" << block.data_[i].second << " ";
        }
        os << std::endl;
    }

public:
    UnrolledLinkedList() {
        std::ifstream fin("list_file");
        block_file_.initialise("block_file");
        if (fin) {
            fin >> head_index_ >> total_blocks_ >> total_size_ >> spare_index;
            return;
        }
        Block head;
        head_index_ = 0;
        head.index_ = 0;
        write_block(head, 0);
        total_blocks_++;
        spare_index++;
    }

    ~UnrolledLinkedList() {
        std::ofstream fout("list_file");
        if (!fout) {
            return;
        }
        fout << head_index_ << std::endl;
        fout << total_blocks_ << " " << total_size_ << std::endl;
        fout << spare_index << std::endl;
    }

    void insert(const KeyType& key, const ValueType& value) {
        int index = find_block(key, value);
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
            if (block_contains(block, key, value)) {
                return;
            }
            if (!block_insert(block, key, value)) {
                block_split(block);
                if (block.max_key_ < std::make_pair(key, value)) {
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

    void erase(const KeyType& key, const ValueType& value) {
        Block head_block;
        read_block(head_block, head_index_);
        if (head_block.next_block_ == -1) {
            return;
        }
        Block cur_block;
        read_block(cur_block, head_block.next_block_);
        while (1) {
            if (!(key < cur_block.min_key_.first || cur_block.max_key_.first < key)) {
                block_erase(cur_block, key, value);
                write_block(cur_block, cur_block.index_);
            }
            if (cur_block.next_block_ == -1) {
                break;
            }
            read_block(cur_block, cur_block.next_block_);
        }
    }

    void find(const KeyType& key) {
        // std::cerr << "NOW FINDING KEY " << key << std::endl;
        Block head_block;
        read_block(head_block, head_index_);
        if (head_block.next_block_ == -1) {
            std::cout << "null" << std::endl;
            return;
        }
        // std::cerr << head_block.index_ << std::endl;
        Block cur_block;
        read_block(cur_block, head_block.next_block_);
        bool found = 0;
        while (1) {
            // std::cerr << cur_block.index_ << " " << found << " " << cur_block.min_key_.first << " " << cur_block.max_key_.first << std::endl;
            if (!(key < cur_block.min_key_.first || cur_block.max_key_.first < key)) {
                found |= block_find(cur_block, key, std::cout);
            }
            if (cur_block.next_block_ == -1) {
                break;
            }
            read_block(cur_block, cur_block.next_block_);
        }
        if (!found) {
            std::cout << "null" << std::endl;
        }
        else {
            std::cout << std::endl;
        }
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
            // std::cerr << "reading block " << id << std::endl;
            bool f = read_block(cur_block, id);
            // std::cerr << "valid " << f << std::endl;
            if (!f) break;
        }
    }
};
struct StringKey {
    char ch[68];
    StringKey() {
        memset(ch, 0, sizeof(ch));
    }
};
bool operator<(const StringKey& a, const StringKey& b) {
    return strcmp(a.ch, b.ch) < 0;
}
std::ostream& operator<<(std::ostream& os, StringKey str) {
    os << str.ch;
    return os;
}
std::istream& operator>>(std::istream& is, StringKey& str) {
    is >> str.ch;
    return is;
}
int main() {
    UnrolledLinkedList<StringKey, int, 200> mp;
    int t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        string op;
        std::cin >> op;
        if (op == "insert") {
            StringKey key;
            int val;
            std::cin >> key >> val;
            // std::cerr << i << " " << op << " " << key << " " << val << std::endl;
            mp.insert(key, val);
        }
        else if (op == "delete") {
            StringKey key;
            int val;
            std::cin >> key >> val;
            // std::cerr << i << " " << op << " " << key << " " << val << std::endl;
            mp.erase(key, val);
        }
        else if (op == "find") {
            StringKey key;
            std::cin >> key;
            // std::cerr << i << " " << op << " " << key << std::endl;
            mp.find(key);
        }
        // mp.print(std::cerr);
    }
    return 0;
}