#include <nan.h>
#include "generic.h"   // NOLINT(build/include)
#include "mpeg.h"   // NOLINT(build/include)


using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;


NAN_MODULE_INIT(InitAll) {
    Set(target, New<String>("readGeneric").ToLocalChecked(), GetFunction(New<FunctionTemplate>(ReadGeneric)).ToLocalChecked());
    Set(target, New<String>("readMPEG").ToLocalChecked(), GetFunction(New<FunctionTemplate>(ReadMPEG)).ToLocalChecked());

}

NODE_MODULE(addon, InitAll)

