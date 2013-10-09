#include <vector>
#include <string>

class Util{
public:
    static std::vector<std::string> split(std::string, std::string);
    static std::string trim(const std::string& str);
    static std::string itos(int i);
};
