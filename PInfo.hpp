#include <string>
#include <vector>

class PInfo
{
    int pid;
    PInfo* parent;
public:
    PInfo(int _pid);
    bool init(std::vector<std::string>& connections);

private:
    bool checkNetInfo(std::vector<std::string>& connections);
};

