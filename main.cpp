#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cinttypes>

#define CACHE_SETS 16
#define CACHE_WAY 4
#define MEM_SIZE 512 * 1024

class pLRU {
   private:
    class pLRULine {
       public:
        uint64_t tag;
        bool flag = false;

        pLRULine(uint64_t tag, bool flag_) : tag(tag), flag(flag_) {}

        pLRULine() : tag(-1), flag(false) {}

        ~pLRULine() = default;
    };

    std::vector<std::vector<pLRULine>> cache =
        std::vector<std::vector<pLRULine>>(CACHE_SETS);

   public:
    int hits = 0;
    int access = 0;

    pLRU() {
        for (int i = 0; i < CACHE_SETS; ++i)
            cache[i] = std::vector<pLRULine>(CACHE_WAY);
    };

    void WriteToPLRU(int index, int tag) {
        access++;
        for (int i = 0; i < CACHE_WAY; ++i) {
            if (cache[index][i].tag == tag) {
                cache[index][i].flag = true;
                ++hits;
                bool dot = false;
                for (int j = 0; j < CACHE_WAY; ++j) {
                    if (!cache[index][j].flag) {
                        dot = true;
                        break;
                    }
                }
                if (!dot) {
                    for (int j = 0; j < CACHE_WAY; ++j) {
                        if (i != j) cache[index][j].flag = false;
                    }
                }
                return;
            }
        }

        int temp = Find(index);

        cache[index][temp].tag = tag;
        cache[index][temp].flag = true;
        bool dot = false;
        for (int j = 0; j < CACHE_WAY; j++) {
            if (!cache[index][j].flag) {
                dot = true;
                break;
            }
        }
        if (!dot) {
            for (int j = 0; j < CACHE_WAY; j++) {
                if (temp != j) cache[index][j].flag = false;
            }
        }
    }

    int Find(int index) {
        int temp = 0;
        for (int i = 0; i < CACHE_WAY; i++) {
            if (!cache[index][i].flag) temp = i;
        }
        return temp;
    }

    float Result() const { return 100 * static_cast<float>(hits) / access; }
};

class LRU {
   private:
    class LRULine {
       public:
        uint64_t tag = -1;
        int time = 0;

        LRULine(uint64_t tag, int time_) : tag(tag), time(time_) {}

        LRULine() : tag(-1), time(0) {}

        ~LRULine() = default;
    };

    std::vector<std::vector<LRULine>> cache =
        std::vector<std::vector<LRULine>>(CACHE_SETS);

   public:
    int hits = 0;
    int access = 0;

    LRU() {
        for (int i = 0; i < CACHE_SETS; ++i)
            cache[i] = std::vector<LRULine>(CACHE_WAY);
    }

    void WriteToLRU(int index, int tag, int time_) {
        access++;
        for (int i = 0; i < CACHE_WAY; ++i) {
            if (cache[index][i].tag == tag) {
                cache[index][i].time = time_;
                ++hits;
                return;
            }
        }
        int temp = Find(index);
        cache[index][temp].tag = tag;
        cache[index][temp].time = time_;
    }

    int Find(int index) {
        int temp = 0;
        for (int i = 0; i < CACHE_WAY; ++i) {
            if (cache[index][i].time < cache[index][temp].time)
                temp = i;
        }
        return temp;
    }

    float Result() const { return 100 * static_cast<float>(hits) / access; }
};

class Arguments {
   public:
    int recplacement_;
    std::string asm_;
    std::string bin_;

    void Parse(int argc, char** argv) {
        std::vector<std::string> array;
        for (int i = 0; i < argc; i++) {
            array.push_back(argv[i]);
        }
        for (int i = 0; i < array.size(); ++i) {
            if (array[i] == "--asm") {
                asm_ = array[i + 1];
            } else if (array[i] == "--bin") {
                bin_ = array[i + 1];
                std::cerr << "Compiling asm code is not supported" << std::endl;
                exit(0);
            } else if (array[i] == "--replacement") {
                recplacement_ = std::stoi(array[i + 1]);
            }
        }
    }
};

std::string LoadAndStoreArgs(std::string& rs1, std::string& rs2) {
    if (rs2 != "") {
        return rs2;
    } else {
        return rs1.substr(2, rs1.size() - 3);
    }
}

int RegisterToIdx(const std::string& reg) {
    if (reg == "zero") {
        return 0;
    } else if (reg == "ra") {
        return 1;
    } else if (reg == "sp") {
        return 2;
    } else if (reg == "gp") {
        return 3;
    } else if (reg == "tp") {
        return 4;
    } else if (reg == "t0") {
        return 5;
    } else if (reg == "t1") {
        return 6;
    } else if (reg == "t2") {
        return 7;
    } else if (reg == "s0" || reg == "fp") {
        return 8;
    } else if (reg == "s1") {
        return 9;
    } else if (reg == "a0") {
        return 10;
    } else if (reg == "a1") {
        return 11;
    } else if (reg == "a2") {
        return 12;
    } else if (reg == "a3") {
        return 13;
    } else if (reg == "a4") {
        return 14;
    } else if (reg == "a5") {
        return 15;
    } else if (reg == "a6") {
        return 16;
    } else if (reg == "a7") {
        return 17;
    } else if (reg == "s2") {
        return 18;
    } else if (reg == "s3") {
        return 19;
    } else if (reg == "s4") {
        return 20;
    } else if (reg == "s5") {
        return 21;
    } else if (reg == "s6") {
        return 22;
    } else if (reg == "s7") {
        return 23;
    } else if (reg == "s8") {
        return 24;
    } else if (reg == "s9") {
        return 25;
    } else if (reg == "s10") {
        return 26;
    } else if (reg == "s11") {
        return 27;
    } else if (reg == "t3") {
        return 28;
    } else if (reg == "t4") {
        return 29;
    } else if (reg == "t5") {
        return 30;
    } else if (reg == "t6") {
        return 31;
    }

    return -1;
}

int32_t HexToDec(const std::string& hex) {
    if (hex.find("(") != std::string::npos) {
        return HexToDec(hex.substr(0, 1));
    }
    if (hex.size() < 2) {
        return std::stoi(hex);
    }
    if (hex[0] != '0' || hex[1] != 'x') {
        return std::stoi(hex);
    }
    int32_t result = 0;
    std::map<char, int> hex_map = {
        {'0', 0b0000}, {'1', 0b0001}, {'2', 0b0010}, {'3', 0b0011},
        {'4', 0b0100}, {'5', 0b0101}, {'6', 0b0110}, {'7', 0b0111},
        {'8', 0b1000}, {'9', 0b1001}, {'A', 0b1010}, {'B', 0b1011},
        {'C', 0b1100}, {'D', 0b1101}, {'E', 0b1110}, {'F', 0b1111},
        {'a', 0b1010}, {'b', 0b1011}, {'c', 0b1100}, {'d', 0b1101},
        {'e', 0b1110}, {'f', 0b1111}};
    for (char i : hex.substr(2)) {
        result <<= 4;
        result |= hex_map[i];
    }
    return result;
}

auto RunCommand(std::vector<std::string>& instructions) {
    std::vector<uint32_t> registers(32, 0);
    std::vector<uint8_t> memory(MEM_SIZE, 0);
    LRU lru;
    pLRU plru;
    int run_time = 0;
    for (int pc = 0; pc < instructions.size(); pc++) {
        std::istringstream iss(instructions[pc]);
        std::string opcode, rd, rs1, rs2;
        iss >> opcode;
        iss >> rd;
        iss >> rs1;
        iss >> rs2;
        if (rd[rd.size() - 1] == ',') {
            rd = rd.substr(0, rd.size() - 1);
        }
        if (rs1[rs1.size() - 1] == ',') {
            rs1 = rs1.substr(0, rs1.size() - 1);
        }
        if (opcode == "add") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] + registers[RegisterToIdx(rs2)];
        } else if (opcode == "sub") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] - registers[RegisterToIdx(rs2)];
        } else if (opcode == "xor") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] ^ registers[RegisterToIdx(rs2)];
        } else if (opcode == "or") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] | registers[RegisterToIdx(rs2)];
        } else if (opcode == "and") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] & registers[RegisterToIdx(rs2)];
        } else if (opcode == "sll") {
            registers[RegisterToIdx(rd)] = registers[RegisterToIdx(rs1)]
                                           << registers[RegisterToIdx(rs2)];
        } else if (opcode == "srl") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] >> registers[RegisterToIdx(rs2)];
        } else if (opcode == "sra") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] >> registers[RegisterToIdx(rs2)];
        } else if (opcode == "slt") {
            registers[RegisterToIdx(rd)] =
                (registers[RegisterToIdx(rs1)] < registers[RegisterToIdx(rs2)])
                    ? 1
                    : 0;
        } else if (opcode == "sltu") {
            registers[RegisterToIdx(rd)] =
                (registers[RegisterToIdx(rs1)] < registers[RegisterToIdx(rs2)])
                    ? 1
                    : 0;
        } else if (opcode == "addi") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] + HexToDec(rs2);
        } else if (opcode == "xori") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] ^ HexToDec(rs2);
        } else if (opcode == "ori") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] | HexToDec(rs2);
        } else if (opcode == "andi") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] & HexToDec(rs2);
        } else if (opcode == "slli") {
            registers[RegisterToIdx(rd)] = registers[RegisterToIdx(rs1)]
                                           << HexToDec(rs2);
        } else if (opcode == "srli") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] >> HexToDec(rs2);
        } else if (opcode == "srai") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] >> HexToDec(rs2);
        } else if (opcode == "slti") {
            registers[RegisterToIdx(rd)] =
                (registers[RegisterToIdx(rs1)] < HexToDec(rs2)) ? 1 : 0;
        } else if (opcode == "sltiu") {
            registers[RegisterToIdx(rd)] =
                (registers[RegisterToIdx(rs1)] < HexToDec(rs2)) ? 1 : 0;
        } else if (opcode == "lb") {
            run_time += 1;
            std::string arg = LoadAndStoreArgs(rs1, rs2);
            registers[RegisterToIdx(rd)] =
                memory[registers[RegisterToIdx(arg)] + HexToDec(rs1)];
            int index = (registers[RegisterToIdx(arg)] >> 5) & 15;
            int tag = (registers[RegisterToIdx(arg)] >> 9) & 1023;
            lru.WriteToLRU(index, tag, run_time);
            plru.WriteToPLRU(index, tag);
        } else if (opcode == "lh") {
            run_time += 1;
            std::string arg = LoadAndStoreArgs(rs1, rs2);
            registers[RegisterToIdx(rd)] =
                memory[registers[RegisterToIdx(arg)] + HexToDec(rs1)];
            registers[RegisterToIdx(rd)] <<= 8;
            registers[RegisterToIdx(rd)] |=
                memory[registers[RegisterToIdx(arg)] + HexToDec(rs1) + 1];
            int index = (registers[RegisterToIdx(arg)] >> 5) & 15;
            int tag = (registers[RegisterToIdx(arg)] >> 9) & 1023;
            lru.WriteToLRU(index, tag, run_time);
            plru.WriteToPLRU(index, tag);
        } else if (opcode == "lw") {
            run_time += 1;
            std::string arg = LoadAndStoreArgs(rs1, rs2);
            registers[RegisterToIdx(rd)] =
                memory[registers[RegisterToIdx(arg)] + HexToDec(rs1)];
            registers[RegisterToIdx(rd)] <<= 8;
            registers[RegisterToIdx(rd)] |=
                memory[registers[RegisterToIdx(arg)] + HexToDec(rs1) + 1];
            registers[RegisterToIdx(rd)] <<= 8;
            registers[RegisterToIdx(rd)] |=
                memory[registers[RegisterToIdx(arg)] + HexToDec(rs1) + 2];
            registers[RegisterToIdx(rd)] <<= 8;
            registers[RegisterToIdx(rd)] |=
                memory[registers[RegisterToIdx(arg)] + HexToDec(rs1) + 3];
            int index = (registers[RegisterToIdx(arg)] >> 5) & 15;
            int tag = (registers[RegisterToIdx(arg)] >> 9) & 1023;
            lru.WriteToLRU(index, tag, run_time);
            plru.WriteToPLRU(index, tag);
        } else if (opcode == "lbu") {
            run_time += 1;
            std::string arg = LoadAndStoreArgs(rs1, rs2);
            registers[RegisterToIdx(rd)] =
                memory[registers[RegisterToIdx(arg)] + HexToDec(rs1)];
            int index = (registers[RegisterToIdx(arg)] >> 5) & 15;
            int tag = (registers[RegisterToIdx(arg)] >> 9) & 1023;
            lru.WriteToLRU(index, tag, run_time);
            plru.WriteToPLRU(index, tag);
        } else if (opcode == "lhu") {
            run_time += 1;
            std::string arg = LoadAndStoreArgs(rs1, rs2);
            registers[RegisterToIdx(rd)] =
                memory[registers[RegisterToIdx(arg)] + HexToDec(rs1)];
            registers[RegisterToIdx(rd)] <<= 8;
            registers[RegisterToIdx(rd)] |=
                memory[registers[RegisterToIdx(arg)] + HexToDec(rs1) + 1];
            int index = (registers[RegisterToIdx(arg)] >> 5) & 15;
            int tag = (registers[RegisterToIdx(arg)] >> 9) & 1023;
            lru.WriteToLRU(index, tag, run_time);
            plru.WriteToPLRU(index, tag);
        } else if (opcode == "sb") {
            run_time += 1;
            std::string arg = LoadAndStoreArgs(rs1, rs2);
            memory[registers[RegisterToIdx(arg)] + HexToDec(rs1)] =
                registers[RegisterToIdx(rs2)];
            int index = (registers[RegisterToIdx(arg)] >> 5) & 15;
            int tag = (registers[RegisterToIdx(arg)] >> 9) & 1023;
            lru.WriteToLRU(index, tag, run_time);
            plru.WriteToPLRU(index, tag);
        } else if (opcode == "sh") {
            run_time += 1;
            std::string arg = LoadAndStoreArgs(rs1, rs2);
            memory[registers[RegisterToIdx(arg)] + HexToDec(rs1) + 1] =
                registers[RegisterToIdx(rs2)] & 255;
            memory[registers[RegisterToIdx(arg)] + HexToDec(rs1)] =
                (registers[RegisterToIdx(rs2)] >> 8) & 255;
            int index = (registers[RegisterToIdx(arg)] >> 5) & 15;
            int tag = (registers[RegisterToIdx(arg)] >> 9) & 1023;
            lru.WriteToLRU(index, tag, run_time);
            plru.WriteToPLRU(index, tag);
        } else if (opcode == "sw") {
            run_time += 1;
            std::string arg = LoadAndStoreArgs(rs1, rs2);
            memory[registers[RegisterToIdx(arg)] + HexToDec(rs1) + 3] =
                registers[RegisterToIdx(rs2)] & 255;
            memory[registers[RegisterToIdx(arg)] + HexToDec(rs1) + 2] =
                (registers[RegisterToIdx(rs2)] >> 8) & 255;
            memory[registers[RegisterToIdx(arg)] + HexToDec(rs1) + 1] =
                (registers[RegisterToIdx(rs2)] >> 16) & 255;
            memory[registers[RegisterToIdx(arg)] + HexToDec(rs1)] =
                (registers[RegisterToIdx(rs2)] >> 24) & 255;
            int index = (registers[RegisterToIdx(arg)] >> 5) & 15;
            int tag = (registers[RegisterToIdx(arg)] >> 9) & 1023;
            lru.WriteToLRU(index, tag, run_time);
            plru.WriteToPLRU(index, tag);
        } else if (opcode == "beq") {
            if (registers[RegisterToIdx(rd)] == registers[RegisterToIdx(rs1)]) {
                pc += HexToDec(rs2) / 4 + 1;
            }
        } else if (opcode == "bne") {
            if (registers[RegisterToIdx(rd)] != registers[RegisterToIdx(rs1)]) {
                pc += HexToDec(rs2) / 4 - 1;
            }
        } else if (opcode == "blt") {
            if (registers[RegisterToIdx(rd)] < registers[RegisterToIdx(rs1)]) {
                pc += HexToDec(rs2) / 4 - 1;
            }
        } else if (opcode == "bge") {
            if (registers[RegisterToIdx(rd)] >= registers[RegisterToIdx(rs1)]) {
                pc += HexToDec(rs2) / 4 - 1;
            }
        } else if (opcode == "bltu") {
            if (registers[RegisterToIdx(rd)] < registers[RegisterToIdx(rs1)]) {
                pc += HexToDec(rs2) / 4 - 1;
            }
        } else if (opcode == "bgeu") {
            if (registers[RegisterToIdx(rd)] >= registers[RegisterToIdx(rs1)]) {
                pc += HexToDec(rs2) / 4 - 1;
            }
        } else if (opcode == "jalr") {
            break;
        } else if (opcode == "jal") {
            pc += HexToDec(rs1) / 4 - 1;
        } else if (opcode == "mul") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] * registers[RegisterToIdx(rs2)];
        } else if (opcode == "mulh") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] * registers[RegisterToIdx(rs2)];
        } else if (opcode == "mulhsu") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] * registers[RegisterToIdx(rs2)];
        } else if (opcode == "mulhu") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] * registers[RegisterToIdx(rs2)];
        } else if (opcode == "div") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] / registers[RegisterToIdx(rs2)];
        } else if (opcode == "divu") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] / registers[RegisterToIdx(rs2)];
        } else if (opcode == "rem") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] % registers[RegisterToIdx(rs2)];
        } else if (opcode == "remu") {
            registers[RegisterToIdx(rd)] =
                registers[RegisterToIdx(rs1)] % registers[RegisterToIdx(rs2)];
        } else if (opcode == "lui") {
            registers[RegisterToIdx(rd)] = HexToDec(rs1) << 12;
        }
        registers[0] = 0;
    }
    return std::make_pair(lru.Result(), plru.Result());
}

int main(int argc, char** argv) {
    Arguments args;
    args.Parse(argc, argv);

    std::ifstream infile(args.asm_);
    std::string line;
    std::vector<std::string> instructions;

    while (std::getline(infile, line)) instructions.push_back(line);

    auto result = RunCommand(instructions);

    if (args.recplacement_ == 0) {
        printf("LRU\thit rate: %3.4f%%\npLRU\thit rate: %3.4f%%\n", result.first, result.second);
    } else if (args.recplacement_ == 1) {
        printf("LRU\thit rate: %3.4f%%\n", result.first);
    } else if (args.recplacement_ == 2) {
        printf("pLRU\thit rate: %3.4f%%\n", result.second);
    }

    return 0;
}