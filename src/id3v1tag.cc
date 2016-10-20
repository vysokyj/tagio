#include "id3v1tag.h"
#include "wrapper.h"

using namespace v8;
using namespace std;

class StringHandler : public TagLib::ID3v1::StringHandler {
public:
    StringHandler(TagLib::String::Type encoding) : encoding(encoding) { };
    virtual TagLib::String parse(const TagLib::ByteVector &data) const;
    virtual TagLib::ByteVector render(const TagLib::String &s) const;
private:
    TagLib::String::Type encoding;
};

TagLib::String StringHandler::parse(const TagLib::ByteVector &data) const {
    TagLib::String s(data, encoding);
    return s;
}

TagLib::ByteVector StringHandler::render(const TagLib::String &s) const {
    TagLib::ByteVector v(s.data(encoding));
    return v;
}


void ExportID3v1Tag(TagLib::ID3v1::Tag *tag, Object *object) {
    TagLibWrapper o(object);
    o.SetString("title", tag->title());
    o.SetString("album", tag->album());
    o.SetString("artist", tag->artist());
    o.SetUint32("track", tag->track());
    o.SetUint32("year", tag->year());
    o.SetString("genre", tag->genre());
    //o.SetUint32("genreNumber", tag->genreNumber());
    o.SetString("comment", tag->comment());
}

void ImportID3v1Tag(Object *object, TagLib::ID3v1::Tag *tag, Configuration *conf) {
    tag->setStringHandler(new StringHandler(conf->ID3v1Encoding()));
    TagLibWrapper o(object);
    tag->setTitle(o.GetString("title"));
    tag->setAlbum(o.GetString("album"));
    tag->setArtist(o.GetString("artist"));
    tag->setTrack(o.GetUint32("track"));
    tag->setYear(o.GetUint32("year"));
    tag->setGenre(o.GetString("genre"));
    //tag->setGenreNumber(o.GetUint32("genreNumber"));
    tag->setComment(o.GetString("comment"));
}