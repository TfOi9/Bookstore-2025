#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include <map>
#include <sstream>

class TestDataGenerator {
private:
    std::mt19937 gen;
    std::uniform_int_distribution<int> value_dist;
    std::uniform_int_distribution<int> op_dist;
    
    const std::vector<std::string> base_keys = {
        "CppPrimer", "Dune", "FlowersForAlgernon", "Java", "Python",
        "Algorithm", "DataStructure", "OperatingSystem", "ComputerNetwork",
        "Database", "Compiler", "ComputerArchitecture", "Math", "Physics",
        "Chemistry", "Biology", "History", "Geography", "Art", "Music",
        "Economics", "Philosophy", "Psychology", "Sociology", "Engineering"
    };
    
    std::vector<std::string> special_keys;
    
public:
    TestDataGenerator(int seed = 42) : gen(seed), 
                                      value_dist(0, 100000),
                                      op_dist(0, 2) {
        // 生成特殊键名，用于边界测试
        generate_special_keys();
    }
    
private:
    void generate_special_keys() {
        // 生成长键名
        special_keys.push_back(std::string(64, 'A')); // 最大长度键
        special_keys.push_back(std::string(64, 'Z'));
        special_keys.push_back(std::string(64, 'a'));
        special_keys.push_back(std::string(64, 'z'));
        special_keys.push_back(std::string(64, '0'));
        
        // 生成特殊字符键名
        special_keys.push_back("key_with_underscore");
        special_keys.push_back("key-with-dash");
        special_keys.push_back("key.with.dots");
        special_keys.push_back("key with spaces");
        special_keys.push_back("key@with#special$chars%");
    }
    
    std::string get_random_special_key() {
        return special_keys[gen() % special_keys.size()];
    }
    
public:
    // 生成随机key
    std::string generate_key(int max_length = 64) {
        // 5%的概率使用特殊键
        if (gen() % 20 == 0) {
            return get_random_special_key();
        }
        
        std::string key = base_keys[gen() % base_keys.size()];
        
        // 添加随机后缀增加多样性
        int suffix_type = gen() % 4;
        switch(suffix_type) {
            case 0:
                key += "_" + std::to_string(gen() % 1000);
                break;
            case 1:
                key += "_v" + std::to_string(gen() % 100);
                break;
            case 2:
                key += "_edition_" + std::to_string(gen() % 50);
                break;
            case 3:
                // 不加后缀
                break;
        }
        
        // 确保不超过最大长度
        if (key.length() > max_length) {
            key = key.substr(0, max_length);
        }
        
        return key;
    }
    
    // 生成基础功能测试
    void generate_basic_test(std::ostream& os) {
        os << "8\n";
        os << "insert FlowersForAlgernon 1966\n";
        os << "insert CppPrimer 2012\n";
        os << "insert Dune 2021\n";
        os << "insert CppPrimer 2001\n";
        os << "find CppPrimer\n";
        os << "find Java\n";
        os << "delete Dune 2021\n";
        os << "find Dune\n";
    }
    
    // 生成边界情况测试
    void generate_edge_cases(std::ostream& os) {
        os << "25\n";
        
        // 测试空查找
        os << "find nonexistent_key\n";
        
        // 测试插入后立即删除
        os << "insert temp_key 100\n";
        os << "delete temp_key 100\n";
        os << "find temp_key\n";
        
        // 测试删除不存在的条目
        os << "delete imaginary_key 999\n";
        os << "find imaginary_key\n";
        
        // 测试重复值
        os << "insert repeated_key 50\n";
        os << "insert repeated_key 50\n";  // 重复插入相同值
        os << "find repeated_key\n";
        
        // 测试大量相同key不同value
        for (int i = 1; i <= 5; i++) {
            os << "insert multi_key " << i * 10 << "\n";
        }
        os << "find multi_key\n";
        
        // 测试边界值
        os << "insert boundary_key 0\n";
        os << "insert boundary_key 100000\n";
        os << "find boundary_key\n";
        
        // 测试特殊键名
        os << "insert " << special_keys[0] << " 123\n";
        os << "find " << special_keys[0] << "\n";
        
        // 测试删除后再插入
        os << "insert revive_key 100\n";
        os << "delete revive_key 100\n";
        os << "insert revive_key 200\n";
        os << "find revive_key\n";
    }
    
    // 生成块分裂测试
    void generate_split_test(std::ostream& os, int block_capacity = 200) {
        int n = block_capacity * 3;  // 确保多次分裂
        os << n + 5 << "\n";
        
        std::string split_key = "split_test_key";
        
        // 插入大量数据，确保分裂
        for (int i = 0; i < n; i++) {
            os << "insert " << split_key << " " << i << "\n";
        }
        
        // 测试查找
        os << "find " << split_key << "\n";
        
        // 测试分裂后的删除（从中间、开头、结尾删除）
        os << "delete " << split_key << " " << (n/2) << "\n";
        os << "delete " << split_key << " 0\n";
        os << "delete " << split_key << " " << (n-1) << "\n";
        os << "find " << split_key << "\n";
    }
    
    // 生成混合操作测试
    void generate_mixed_operations_test(std::ostream& os, int n = 2000) {
        os << n << "\n";
        
        std::map<std::string, std::set<int>> data;
        std::vector<std::string> frequently_used_keys;
        
        // 预生成一些频繁使用的键
        for (int i = 0; i < 10; i++) {
            frequently_used_keys.push_back("frequent_key_" + std::to_string(i));
        }
        
        for (int i = 0; i < n; i++) {
            std::string key;
            int value = value_dist(gen);
            
            // 70%的操作集中在频繁使用的键上，30%使用随机键
            if (gen() % 10 < 7 && !frequently_used_keys.empty()) {
                key = frequently_used_keys[gen() % frequently_used_keys.size()];
            } else {
                key = generate_key();
            }
            
            int op_type = op_dist(gen);
            
            // 调整操作概率：40%插入，30%删除，30%查找
            if (gen() % 10 < 4) {
                op_type = 0; // insert
            } else if (gen() % 10 < 7) {
                op_type = 1; // delete
            } else {
                op_type = 2; // find
            }
            
            switch(op_type) {
                case 0:  // insert
                    os << "insert " << key << " " << value << "\n";
                    data[key].insert(value);
                    break;
                    
                case 1:  // delete
                    if (!data[key].empty() && gen() % 3 != 0) {
                        // 删除存在的数据
                        auto it = data[key].begin();
                        std::advance(it, gen() % data[key].size());
                        value = *it;
                        data[key].erase(it);
                    }
                    os << "delete " << key << " " << value << "\n";
                    break;
                    
                case 2:  // find
                    os << "find " << key << "\n";
                    break;
            }
            
            // 偶尔进行批量操作
            if (i % 100 == 99) {
                std::string batch_key = "batch_key_" + std::to_string(i / 100);
                for (int j = 0; j < 5; j++) {
                    os << "insert " << batch_key << " " << (i * 10 + j) << "\n";
                    data[batch_key].insert(i * 10 + j);
                }
                os << "find " << batch_key << "\n";
                i += 6; // 调整计数器
            }
        }
    }
    
    // 生成极端压力测试
    void generate_stress_test(std::ostream& os, int n = 10000) {
        os << n << "\n";
        
        // 使用少量键进行大量操作，测试性能
        std::vector<std::string> stress_keys;
        for (int i = 0; i < 50; i++) {
            stress_keys.push_back("stress_key_" + std::to_string(i));
        }
        
        std::map<std::string, std::set<int>> data;
        
        for (int i = 0; i < n; i++) {
            std::string key = stress_keys[gen() % stress_keys.size()];
            int value = gen() % 1000;  // 小范围值，增加冲突
            
            int op_type;
            if (i < n * 0.6) {
                op_type = 0; // 前60%主要是插入
            } else if (i < n * 0.8) {
                op_type = 1; // 接下来20%删除
            } else {
                op_type = 2; // 最后20%查找
            }
            
            switch(op_type) {
                case 0:  // insert
                    os << "insert " << key << " " << value << "\n";
                    data[key].insert(value);
                    break;
                    
                case 1:  // delete
                    if (!data[key].empty()) {
                        auto it = data[key].begin();
                        std::advance(it, gen() % data[key].size());
                        value = *it;
                        data[key].erase(it);
                    }
                    os << "delete " << key << " " << value << "\n";
                    break;
                    
                case 2:  // find
                    os << "find " << key << "\n";
                    break;
            }
        }
    }
    
    // 生成随机压力测试
    void generate_random_test(std::ostream& os, int n = 1000) {
        os << n << "\n";
        
        std::map<std::string, std::set<int>> data;
        
        for (int i = 0; i < n; i++) {
            int op_type = op_dist(gen);
            std::string key = generate_key();
            int value = value_dist(gen);
            
            // 调整操作分布
            if (gen() % 10 < 4) op_type = 0; // 40%插入
            else if (gen() % 10 < 7) op_type = 1; // 30%删除
            else op_type = 2; // 30%查找
            
            switch(op_type) {
                case 0:  // insert
                    os << "insert " << key << " " << value << "\n";
                    data[key].insert(value);
                    break;
                    
                case 1:  // delete
                    if (!data[key].empty() && gen() % 3 != 0) {
                        auto it = data[key].begin();
                        std::advance(it, gen() % data[key].size());
                        value = *it;
                        data[key].erase(it);
                    }
                    os << "delete " << key << " " << value << "\n";
                    break;
                    
                case 2:  // find
                    os << "find " << key << "\n";
                    break;
            }
        }
    }
    
    // 生成综合测试
    void generate_comprehensive_test(std::ostream& os) {
        std::vector<std::function<void(std::ostream&)>> test_generators = {
            [this](std::ostream& os) { this->generate_basic_test(os); },
            [this](std::ostream& os) { this->generate_edge_cases(os); },
            [this](std::ostream& os) { this->generate_split_test(os, 100); },
            [this](std::ostream& os) { this->generate_mixed_operations_test(os, 1000); },
            [this](std::ostream& os) { this->generate_stress_test(os, 2000); }
        };
        
        for (const auto& generator : test_generators) {
            generator(os);
        }
    }
};

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

void validate_test_file(const std::string& filename, const std::string& output_file) {
    std::ifstream file(filename);
    std::ofstream outfile(output_file);
    std::string line;
    Validator validator;
    
    int n;
    std::getline(file, line);
    n = std::stoi(line);
    
    std::cerr << "验证文件: " << filename << " (共 " << n << " 条指令)" << std::endl;
    
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
        validator.process_operation(op, key, value, outfile);
    }
}

int main() {
    TestDataGenerator generator;

    std::string path = "../test/test", ans_path;

    int cnt;
    std::cout << "Please enter testcase number: ";
    std::cin >> cnt;

    if (cnt < 0) {
        std::cout << "Generate ans only." << std::endl;
    }

    int x;
    std::cout << "Please enter testcase size: ";
    std::cin >> x;

    path += std::to_string(std::abs(cnt));
    ans_path = path + ".ans";
    path += ".in";
    
    if (cnt >= 0) {
        std::ofstream test4(path);
        generator.generate_random_test(test4, x);
    }
    validate_test_file(path, ans_path);

    return 0;
}