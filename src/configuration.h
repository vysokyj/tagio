#ifndef TAGIO_CONFIGURATION_H
#define TAGIO_CONFIGURATION_H

#include <node.h>
#include <string>
#include <taglib/tstring.h>


const int FILE_EXTRACTED_IS_IGNORED = 1;     // Ignore attached files
const int FILE_EXTRACTED_AS_FILENAME = 2;     // JSON contains just the filename -> somefile.ext
const int FILE_EXTRACTED_AS_ABSOLUTE_URL = 3; // JSON contains compete file URL -> file://somepath/somefile.ext
const int FILE_EXTRACTED_AS_RELATIVE_URL = 4; // JSON contains file URL with given prefix -> /somepath/somefile.ext

class Configuration {
public:

    Configuration() {}
    ~Configuration() {}
    
    int fileExtracted() { return m_fileExtracted; }
    void setFileExtracted(int as) { m_fileExtracted = as; }
    
    TagLib::String fileDirectory() { return m_fileDirectory; }
    void setFileDirectory(TagLib::String dir) { m_fileDirectory = dir; }
    
    TagLib::String fileUrlPrefix() { return m_fileUrlPrefix; }
    void setFileUrlPrefix(TagLib::String prefix) { m_fileUrlPrefix = prefix; }
    
    bool APEWritable() { return m_APEWritable; }
    void setAPEWritable(bool b) { m_APEWritable = b; }

    bool APEReadable() { return m_APEReadable; }
    void setAPEReadable(bool b) { m_APEReadable = b; }

    bool APEExpunged() { return m_APEExpunged; }
    void setAPEExpunged(bool b) { m_APEExpunged = b; }

    bool ID3v1Writable() { return m_ID3v1Writable; }
    void setID3v1Writable(bool b) { m_ID3v1Writable = b; }

    bool ID3v1Readable() { return m_ID3v1Readable; }
    void setID3v1Readable(bool b) { m_ID3v1Readable = b; }

    bool ID3v1Expunged() { return m_ID3v1Expunged; }
    void setID3v1Expunged(bool b) { m_ID3v1Expunged = b; }

    TagLib::String::Type ID3v1Encoding() { return m_ID3v1Encoding; }
    void setID3v1Encoding(TagLib::String::Type encoding) { m_ID3v1Encoding = encoding; }

    bool ID3v2Writable() { return m_ID3v2Writable; }
    void setID3v2Writable(bool b) { m_ID3v2Writable = b; }

    bool ID3v2Readable() { return m_ID3v2Readable; }
    void setID3v2Readable(bool b) { m_ID3v2Readable = b; }

    bool ID3v2Expunged() { return m_ID3v2Expunged; }
    void setID3v2Expunged(bool b) { m_ID3v2Expunged = b; }

    uint32_t ID3v2Version() { return m_ID3v2Version; }
    void setID3v2Version(uint32_t version) { m_ID3v2Version = version; }

    TagLib::String::Type ID3v2Encoding() { return m_ID3v2Encoding; }
    void setID3v2Encoding(TagLib::String::Type encoding) { m_ID3v2Encoding = encoding; }

    bool ID3v2UseFrameEncoding() { return m_ID3v2UseFrameEncoding; }
    void setID3v2UseFrameEncoding(bool b) { m_ID3v2UseFrameEncoding = b; }


private:
    Configuration(Configuration const&)   = delete;
    void operator=(Configuration const&)  = delete;


    int            m_fileExtracted = FILE_EXTRACTED_AS_FILENAME;
    TagLib::String m_fileDirectory = ".";
    TagLib::String m_fileUrlPrefix = "";

    bool m_APEWritable = true;
    bool m_APEReadable = true;
    bool m_APEExpunged = false;

    bool m_ID3v1Writable = true;
    bool m_ID3v1Readable = true;
    bool m_ID3v1Expunged = false;
    TagLib::String::Type m_ID3v1Encoding = TagLib::String::UTF8;
    
    bool m_ID3v2Writable = true;
    bool m_ID3v2Readable = true;
    bool m_ID3v2Expunged = false;
    uint32_t m_ID3v2Version = 4;
    TagLib::String::Type m_ID3v2Encoding = TagLib::String::UTF8;
    bool m_ID3v2UseFrameEncoding = false;

};

void ExportConfiguration(Configuration *configuration, v8::Object *object);
void ImportConfiguration(v8::Object *object, Configuration *configuration);


#endif //TAGIO_CONFIGURATION_H
