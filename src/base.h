#ifndef TAGIO_BASE_H
#define TAGIO_BASE_H


#include <node/node.h>
#include "md5.h"
#include "wrapper.h"

namespace TagIO {

    class Base {

    public:
        const char *GetFilePath() { return this->path.c_str(); }

    protected:
        Base(const char *path);
        ~Base();

    private:
        std::string path;

    };
}

#endif //TAGIO_BASE_H
