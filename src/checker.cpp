#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <sstream>

class Validator {
private:
    std::map<std::string, std::set<int>> data;
    
public:
    void process_operation(const std::string& op, const std::string& key, int value, std::ostream& output) {
        if (op == "insert") {
            data[key].insert(value);
        } else if (op == "delete") {
            if (data.find(key) != data.end()) {
                data[key].erase(value);
                if (data[key].empty()) {
                    data.erase(key);
                }
            }
        } else if (op == "find") {
            if (data.find(key) == data.end() || data[key].empty()) {
                output << "null\n";
            } else {
                for (auto it = data[key].begin(); it != data[key].end(); ++it) {
                    if (it != data[key].begin()) output << " ";
                    output << *it;
                }
                output << "\n";
            }
        }
    }
};

void validate_test_file(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    Validator validator;
    
    int n;
    std::getline(file, line);
    n = std::stoi(line);
    
    // std::cout << "验证文件: " << filename << " (共 " << n << " 条指令)" << std::endl;
    
    for (int i = 0; i < n; i++) {
        std::getline(file, line);
        std::istringstream iss(line);
        std::string op, key;
        int value = 0;
        
        iss >> op >> key;
        if (op != "find") {
            iss >> value;
        }
        
        // 这里可以输出每一步操作进行调试
        // std::cout << "执行: " << op << " " << key << " " << value << std::endl;
        validator.process_operation(op, key, value, std::cout);
    }
}

int main() {
    validate_test_file("test_random.txt");
    return 0;
}