#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <set>
#include <map>

class TestDataGenerator {
private:
    std::mt19937 gen;
    std::uniform_int_distribution<int> value_dist;
    std::uniform_int_distribution<int> op_dist;
    
    const std::vector<std::string> base_keys = {
        "CppPrimer", "Dune", "FlowersForAlgernon", "Java", "Python",
        "Algorithm", "DataStructure", "OperatingSystem", "ComputerNetwork",
        "Database", "Compiler", "ComputerArchitecture"
    };
    
public:
    TestDataGenerator(int seed = 42) : gen(seed), 
                                      value_dist(0, 10000),
                                      op_dist(0, 2) {}
    
    // 生成随机key
    std::string generate_key(int max_length = 64) {
        std::string key = base_keys[gen() % base_keys.size()];
        // 添加随机后缀增加多样性
        if (gen() % 3 == 0) {
            key += "_" + std::to_string(gen() % 100);
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
        os << "12\n";
        // 测试空查找
        os << "find nonexistent_key\n";
        
        // 测试插入后立即删除
        os << "insert temp_key 100\n";
        os << "delete temp_key 100\n";
        os << "find temp_key\n";
        
        // 测试删除不存在的条目
        os << "delete imaginary_key 999\n";
        
        // 测试重复值（题目说可能重复）
        os << "insert repeated_key 50\n";
        os << "insert repeated_key 50\n";  // 重复插入
        os << "find repeated_key\n";
        
        // 测试大量相同key不同value
        for (int i = 1; i <= 5; i++) {
            os << "insert multi_key " << i * 10 << "\n";
        }
        os << "find multi_key\n";
    }
    
    // 生成块分裂测试
    void generate_split_test(std::ostream& os, int block_capacity = 200) {
        int n = block_capacity + 10;  // 确保会分裂
        os << n + 2 << "\n";  // +2 for the find operations
        
        std::string split_key = "split_test_key";
        for (int i = 0; i < n; i++) {
            os << "insert " << split_key << " " << i << "\n";
        }
        
        os << "find " << split_key << "\n";
        
        // 测试分裂后的删除
        os << "delete " << split_key << " " << (n/2) << "\n";
        os << "find " << split_key << "\n";
    }
    
    // 生成随机压力测试
    void generate_random_test(std::ostream& os, int n = 1000) {
        os << n << "\n";
        
        std::map<std::string, std::set<int>> data;  // 用于跟踪当前数据状态
        
        for (int i = 0; i < n; i++) {
            int op_type = op_dist(gen);
            std::string key = generate_key();
            int value = value_dist(gen);
            
            switch(op_type) {
                case 0:  // insert
                    os << "insert " << key << " " << value << "\n";
                    data[key].insert(value);
                    break;
                    
                case 1:  // delete
                    // 有一定概率删除存在的数据，有一定概率删除不存在的数据
                    if (!data[key].empty() && gen() % 3 != 0) {
                        // 删除存在的数据
                        auto it = data[key].begin();
                        std::advance(it, gen() % data[key].size());
                        value = *it;
                        data[key].erase(it);
                    } else {
                        // 删除可能不存在的数据
                        value = value_dist(gen);
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
        std::cout << "生成综合测试..." << std::endl;
        
        // 第一部分：基础功能
        generate_basic_test(os);
        
        // 第二部分：边界情况
        generate_edge_cases(os);
        
        // 第三部分：块分裂测试
        generate_split_test(os, 200);
        
        // 第四部分：随机测试
        generate_random_test(os, 500);
    }
};

int main() {
    TestDataGenerator generator;
    
    std::ofstream test4("test_random.txt");
    generator.generate_random_test(test4, 10000);

    return 0;
}