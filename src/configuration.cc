#include "configuration.h"
#include "wrapper.h"

using namespace std;

static int FileExtractedAsCode(TagLib::String string) {
    std::string s = string.to8Bit(true);
    if (s.compare("IS_IGNORED") == 0)
        return FILE_EXTRACTED_IS_IGNORED;
    else if (s.compare("AS_FILENAME") == 0)
        return FILE_EXTRACTED_AS_FILENAME;
    else if (s.compare("AS_ABSOLUTE_URL") == 0)
        return FILE_EXTRACTED_AS_ABSOLUTE_URL;
    else if (s.compare("AS_RELATIVE_URL") == 0)
        return FILE_EXTRACTED_AS_RELATIVE_URL;
    else
        return FILE_EXTRACTED_IS_IGNORED;
}

static TagLib::String FileExtractedAsString(int method) {
    switch(method) {
        case FILE_EXTRACTED_IS_IGNORED:
            return "IS_IGNORED";
        case FILE_EXTRACTED_AS_FILENAME:
            return "AS_FILENAME";
        case FILE_EXTRACTED_AS_ABSOLUTE_URL:
            return "AS_ABSOLUTE_URL";
        case FILE_EXTRACTED_AS_RELATIVE_URL:
            return "AS_RELATIVE_URL";
        default:
            return "IS_IGNORED";
    }
}

void ExportConfiguration(Configuration *conf, v8::Object *object) {
    TagLibWrapper o(object);
    o.SetString("fileExtracted", FileExtractedAsString(conf->fileExtracted()));
    o.SetString("fileDirectory", conf->fileDirectory());
    o.SetString("fileUrlPrefix", conf->fileUrlPrefix());
    o.SetBoolean("apeReadable", conf->APEReadable());
    o.SetBoolean("apeWritable", conf->APEWritable());
    o.SetBoolean("apeExpunged", conf->APEExpunged());
    o.SetBoolean("id3v1Readable", conf->ID3v1Readable());
    o.SetBoolean("id3v1Writable", conf->ID3v1Writable());
    o.SetBoolean("id3v1Expunged", conf->ID3v1Expunged());
    o.SetEncoding("id3v1Encoding", conf->ID3v1Encoding());
    o.SetBoolean("id3v2Readable", conf->ID3v2Readable());
    o.SetBoolean("id3v2Writable", conf->ID3v2Writable());
    o.SetBoolean("id3v2Expunged", conf->ID3v2Expunged());
    o.SetUint32("id3v2Version", conf->ID3v2Version());
    o.SetEncoding("id3v2Encoding", conf->ID3v2Encoding());
    o.SetBoolean("id3v2UseFrameEncoding", conf->ID3v2UseFrameEncoding());
}

void ImportConfiguration(v8::Object *object, Configuration *conf) {
    TagLibWrapper o(object);
    conf->setFileExtracted(FileExtractedAsCode(o.GetString("fileExtracted")));
    conf->setFileDirectory(o.GetString("fileDirectory"));
    conf->setFileUrlPrefix(o.GetString("fileUrlPrefix"));
    conf->setAPEReadable(o.GetBoolean("apeReadable"));
    conf->setAPEWritable(o.GetBoolean("apeWritable"));
    conf->setAPEExpunged(o.GetBoolean("apeExpunged"));
    conf->setID3v1Readable(o.GetBoolean("id3v1Readable"));
    conf->setID3v1Writable(o.GetBoolean("id3v1Writable"));
    conf->setID3v1Expunged(o.GetBoolean("id3v1Expunged"));
    conf->setID3v1Encoding(o.GetEncoding("id3v1Encoding"));
    conf->setID3v2Readable(o.GetBoolean("id3v2Readable"));
    conf->setID3v2Writable(o.GetBoolean("id3v2Writable"));
    conf->setID3v2Expunged(o.GetBoolean("id3v2Expunged"));
    conf->setID3v2Encoding(o.GetEncoding("id3v2Encoding"));
    conf->setID3v2Version(o.GetUint32("id3v2Version"));
    conf->setID3v2UseFrameEncoding(o.GetBoolean("id3v2UseFrameEncoding"));
}
