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
    o.SetString("fileExtracted", FileExtractedAsString(conf->FileExtracted()));
    o.SetString("fileDirectory", conf->FileDirectory());
    o.SetString("fileUrlPrefix", conf->FileUrlPrefix());
    o.SetBoolean("configurationReadable", conf->ConfigurationReadable());
    o.SetBoolean("audioPropertiesReadable", conf->AudioPropertiesReadable());
    o.SetBoolean("tagReadable", conf->TagReadable());
    o.SetBoolean("apeReadable", conf->APEReadable());
    o.SetBoolean("apeWritable", conf->APEWritable());
    o.SetBoolean("id3v1Readable", conf->ID3v1Readable());
    o.SetBoolean("id3v1Writable", conf->ID3v1Writable());
    o.SetEncoding("id3v1Encoding", conf->ID3v1Encoding());
    o.SetBoolean("id3v2Readable", conf->ID3v2Readable());
    o.SetBoolean("id3v2Writable", conf->ID3v2Writable());
    o.SetUint32("id3v2Version", conf->ID3v2Version());
    o.SetEncoding("id3v2Encoding", conf->ID3v2Encoding());
    o.SetBoolean("id3v2UseFrameEncoding", conf->ID3v2UseFrameEncoding());
    o.SetBoolean("xiphCommentReadable", conf->XIPHCommentReadable());
    o.SetBoolean("xiphCommentWritable", conf->XIPHCommentWritable());
}

void ImportConfiguration(v8::Object *object, Configuration *conf) {
    TagLibWrapper o(object);
    conf->SetFileExtracted(FileExtractedAsCode(o.GetString("fileExtracted")));
    conf->SetFileDirectory(o.GetString("fileDirectory"));
    conf->SetFileUrlPrefix(o.GetString("fileUrlPrefix"));
    conf->SetConfigurationReadable(o.GetBoolean("configurationReadable"));
    conf->SetAudioPropertiesReadable(o.GetBoolean("audioPropertiesReadable"));
    conf->SetTagReadable(o.GetBoolean("tagReadable"));
    conf->SetAPEReadable(o.GetBoolean("apeReadable"));
    conf->SetAPEWritable(o.GetBoolean("apeWritable"));
    conf->SetID3v1Readable(o.GetBoolean("id3v1Readable"));
    conf->SetID3v1Writable(o.GetBoolean("id3v1Writable"));
    conf->SetID3v1Encoding(o.GetEncoding("id3v1Encoding"));
    conf->SetID3v2Readable(o.GetBoolean("id3v2Readable"));
    conf->SetID3v2Writable(o.GetBoolean("id3v2Writable"));
    conf->SetID3v2Encoding(o.GetEncoding("id3v2Encoding"));
    conf->SetID3v2Version(o.GetUint32("id3v2Version"));
    conf->SetID3v2UseFrameEncoding(o.GetBoolean("id3v2UseFrameEncoding"));
    conf->SetXIPHCommentReadable(o.GetBoolean("xiphCommentReadable"));
    conf->SetXIPHCommentWritable(o.GetBoolean("xiphCommentWritable"));
}
