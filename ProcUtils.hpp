#include <vector>
#include "ProcNetTcpRecord.hpp"


class ProcUtils
{
public:

    static ProcUtils* get();

    ProcNetTcpRecord* getTcpRecordBySocket(const char *id) const;
    
private:
    static ProcUtils* instance;
    
    ProcUtils();

    std::vector<ProcNetTcpRecord*> tcp;
};