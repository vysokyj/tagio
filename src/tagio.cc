#include <nan.h>
#include "generic.h"   // NOLINT(build/include)
#include "mpeg.h"   // NOLINT(build/include)
#include "flac.h"   // NOLINT(build/include)


using v8::FunctionTemplate;
using v8::Handle;
using v8::Object;
using v8::String;
using Nan::GetFunction;
using Nan::New;
using Nan::Set;


NAN_MODULE_INIT(InitAll) {
    Set(target, New<String>("readGeneric").ToLocalChecked(), GetFunction(New<FunctionTemplate>(ReadGeneric)).ToLocalChecked());
    Set(target, New<String>("writeGeneric").ToLocalChecked(), GetFunction(New<FunctionTemplate>(WriteGeneric)).ToLocalChecked());
    Set(target, New<String>("readMPEG").ToLocalChecked(), GetFunction(New<FunctionTemplate>(ReadMPEG)).ToLocalChecked());
    Set(target, New<String>("readFLAC").ToLocalChecked(), GetFunction(New<FunctionTemplate>(ReadFLAC)).ToLocalChecked());
}

NODE_MODULE(addon, InitAll)

