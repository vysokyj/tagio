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
    
    int FileExtracted() { return fileExtracted; }
    void SetFileExtracted(int as) { fileExtracted = as; }
    
    TagLib::String FileDirectory() { return fileDirectory; }
    void SetFileDirectory(TagLib::String dir) { fileDirectory = dir; }
    
    TagLib::String FileUrlPrefix() { return fileUrlPrefix; }
    void SetFileUrlPrefix(TagLib::String prefix) { fileUrlPrefix = prefix; }

    bool ConfigurationReadable() { return configurationReadable; }
    void SetConfigurationReadable(bool b) { configurationReadable = b; }

    bool AudioPropertiesReadable() { return audioPropertiesReadable; }
    void SetAudioPropertiesReadable(bool b) { audioPropertiesReadable = b; }

    bool TagReadable() { return tagReadable; }
    void SetTagReadable(bool b) { tagReadable = b; }
    
    bool APEWritable() { return apeWritable; }
    void SetAPEWritable(bool b) { apeWritable = b; }

    bool APEReadable() { return apeReadable; }
    void SetAPEReadable(bool b) { apeReadable = b; }

    bool ID3v1Writable() { return id3v1Writable; }
    void SetID3v1Writable(bool b) { id3v1Writable = b; }

    bool ID3v1Readable() { return id3v1Readable; }
    void SetID3v1Readable(bool b) { id3v1Readable = b; }

    TagLib::String::Type ID3v1Encoding() { return id3v1Encoding; }
    void SetID3v1Encoding(TagLib::String::Type encoding) { id3v1Encoding = encoding; }

    bool ID3v2Writable() { return id3v2Writable; }
    void SetID3v2Writable(bool b) { id3v2Writable = b; }

    bool ID3v2Readable() { return id3v2Readable; }
    void SetID3v2Readable(bool b) { id3v2Readable = b; }

    uint32_t ID3v2Version() { return id3v2Version; }
    void SetID3v2Version(uint32_t version) { id3v2Version = version; }

    TagLib::String::Type ID3v2Encoding() { return id3v2Encoding; }
    void SetID3v2Encoding(TagLib::String::Type encoding) { id3v2Encoding = encoding; }

    bool ID3v2UseFrameEncoding() { return id3v2UseFrameEncoding; }
    void SetID3v2UseFrameEncoding(bool b) { id3v2UseFrameEncoding = b; }


private:
    Configuration(Configuration const&)   = delete;
    void operator=(Configuration const&)  = delete;


    int            fileExtracted = FILE_EXTRACTED_AS_FILENAME;
    TagLib::String fileDirectory = ".";
    TagLib::String fileUrlPrefix = "";

    bool configurationReadable = false;
    bool audioPropertiesReadable = true;
    bool tagReadable = true;

    bool apeWritable = false;
    bool apeReadable = false;

    bool id3v1Writable = true;
    bool id3v1Readable = true;
    TagLib::String::Type id3v1Encoding = TagLib::String::UTF8;
    
    bool id3v2Writable = true;
    bool id3v2Readable = true;
    uint32_t id3v2Version = 4;
    TagLib::String::Type id3v2Encoding = TagLib::String::UTF8;
    bool id3v2UseFrameEncoding = false;

};

void ExportConfiguration(Configuration *configuration, v8::Object *object);
void ImportConfiguration(v8::Object *object, Configuration *configuration);


#endif //TAGIO_CONFIGURATION_H
