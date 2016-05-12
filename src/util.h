#ifndef TAGIO_UTIL_H
#define TAGIO_UTIL_H

#include <string>
#include <vector>

namespace TagIO {
    class Util {
    public:
        static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
        static std::vector<std::string> split(const std::string &s, char delim);
    };

}
#endif //TAGIO_UTIL_H
