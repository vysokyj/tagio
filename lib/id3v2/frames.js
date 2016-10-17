module.exports = [
    {
        "id": "AENC",
        "title": "Audio encryption",
        "description": "This frame indicates if the actual audio stream is encrypted, and by whom. Since standardisation of such encryption scheme is beyond this document, all 'AENC' frames begin with a terminated string with a URL containing an email address, or a link to a location where an email address can be found, that belongs to the organisation responsible for this specific encrypted audio file. Questions regarding the encrypted audio should be sent to the email address specified. If a $00 is found directly after the 'Frame size' and the audio file indeed is encrypted, the whole file may be considered useless."
    },
    {
        "id": "APIC",
        "title": "Attached picture",
        "description": "This frame contains a picture directly related to the audio file. Image format is the MIME type and subtype [MIME] for the image. In the event that the MIME media type name is omitted, 'image/' will be implied. The 'image/png' [PNG] or 'image/jpeg' [JFIF] picture format should be used when interoperability is wanted. Description is a short description of the picture, represented as a terminated text string. There may be several pictures attached to one file, each in their individual 'APIC' frame, but only one with the same content descriptor. There may only be one picture with the picture type declared as picture type $01 and $02 respectively. There is the possibility to put only a link to the image file by using the 'MIME type' --> and having a complete URL [URL] instead of picture data. The use of linked files should however be used sparingly since there is the risk of separation of files."
    },
    {
        "id": "ASPI",
        "title": "Audio seek point index",
        "description": "Audio files with variable bit rates are intrinsically difficult to deal with in the case of seeking within the file. The ASPI frame makes seeking easier by providing a list a seek points within the audio file. The seek points are a fractional offset within the audio data, providing a starting point from which to find an appropriate point to start decoding. The presence of an ASPI frame requires the existence of a TLEN frame, indicating the duration of the file in milliseconds. There may only be one 'audio seek point index' frame in a tag."
    },
    {
        "id": "COMM",
        "title": "Comments",
        "description": "This frame is intended for any kind of full text information that does not fit in any other frame. It consists of a frame header followed by encoding, language and content descriptors and is ended with the actual comment as a text string. Newline characters are allowed in the comment text string. There may be more than one comment frame in each tag, but only one with the same language and content descriptor."
    },
    {
        "id": "ENCR",
        "title": "Encryption method registration",
        "description": "To identify with which method a frame has been encrypted the encryption method must be registered in the tag with this frame. The 'Owner identifier' is a null-terminated string with a URL [URL] containing an email address, or a link to a location where an email address can be found, that belongs to the organisation responsible for this specific encryption method. Questions regarding the encryption method should be sent to the indicated email address. The 'Method symbol' contains a value that is associated with this method throughout the whole tag, in the range $80-F0. All other values are reserved. The 'Method symbol' may optionally be followed by encryption specific data. There may be several 'ENCR' frames in a tag but only one containing the same symbol and only one containing the same owner identifier. The method must be used somewhere in the tag. See the description of the frame encryption flag in the ID3v2 structure document [ID3v2-strct] for more information."
    },
    {
        "id": "EQU2",
        "title": "Equalisation",
        "description": "This is another subjective, alignment frame. It allows the user to predefine an equalisation curve within the audio file. There may be more than one 'EQU2' frame in each tag, but only one with the same identification string."
    },
    {
        "id": "ETCO",
        "title": "Event timing codes",
        "description": "This frame allows synchronisation with key events in the audio."
    },
    {
        "id": "GEOB",
        "title": "General encapsulated object",
        "description": " In this frame any type of file can be encapsulated. After the header, 'Frame size' and 'Encoding' follows 'MIME type' [MIME] represented as a terminated string encoded with ISO 8859-1 [ISO-8859-1]. The filename is case sensitive and is encoded as 'Encoding'. Then follows a content description as terminated string, encoded as 'Encoding'. The last thing in the frame is the actual object. The first two strings may be omitted, leaving only their terminations. MIME type is always an ISO-8859-1 text string. There may be more than one 'GEOB' frame in each tag, but only one with the same content descriptor."
    },
    {
        "id": "GRID",
        "title": "Group identification registration",
        "description": " This frame enables grouping of otherwise unrelated frames. This can be used when some frames are to be signed. To identify which frames belongs to a set of frames a group identifier must be registered in the tag with this frame. The 'Owner identifier' is a null-terminated string with a URL [URL] containing an email address, or a link to a location where an email address can be found, that belongs to the organisation responsible for this grouping. Questions regarding the grouping should be sent to the indicated email address. The 'Group symbol' contains a value that associates the frame with this group throughout the whole tag, in the range $80-F0. All other values are reserved. The 'Group symbol' may optionally be followed by some group specific data, e.g. a digital signature. There may be several 'GRID' frames in a tag but only one containing the same symbol and only one containing the same owner identifier. The group symbol must be used somewhere in the tag. See the description of the frame grouping flag in the ID3v2 structure document [ID3v2-strct] for more information."
    },
    {
        "id": "LINK",
        "title": "Linked information",
        "description": "To keep information duplication as low as possible this frame may be used to link information from another ID3v2 tag that might reside in another audio file or alone in a binary file. It is RECOMMENDED that this method is only used when the files are stored on a CD-ROM or other circumstances when the risk of file separation is low. The frame contains a frame identifier, which is the frame that should be linked into this tag, a URL [URL] field, where a reference to the file where the frame is given, and additional ID data, if needed. Data should be retrieved from the first tag found in the file to which this link points. There may be more than one 'LINK' frame in a tag, but only one with the same contents. A linked frame is to be considered as part of the tag and has the same restrictions as if it was a physical part of the tag (i.e. only one 'RVRB' frame allowed, whether it's linked or not)."
    },
    {
        "id": "MCDI",
        "title": "Music CD identifier",
        "description": "This frame is intended for music that comes from a CD, so that the CD can be identified in databases such as the CDDB [CDDB]. The frame consists of a binary dump of the Table Of Contents, TOC, from the CD, which is a header of 4 bytes and then 8 bytes/track on the CD plus 8 bytes for the 'lead out', making a maximum of 804 bytes. The offset to the beginning of every track on the CD should be described with a four bytes absolute CD-frame address per track, and not with absolute time. When this frame is used the presence of a valid 'TRCK' frame is REQUIRED, even if the CD's only got one track. It is recommended that this frame is always added to tags originating from CDs. There may only be one 'MCDI' frame in each tag."
    },
    {
        "id": "MLLT",
        "title": "MPEG location lookup table",
        "description": "To increase performance and accuracy of jumps within a MPEG [MPEG] audio file, frames with time codes in different locations in the file might be useful. This ID3v2 frame includes references that the software can use to calculate positions in the file. After the frame header follows a descriptor of how much the 'frame counter' should be increased for every reference. If this value is two then the first reference points out the second frame, the 2nd reference the 4th frame, the 3rd reference the 6th frame etc. In a similar way the 'bytes between reference' and 'milliseconds between reference' points out bytes and milliseconds respectively. Each reference consists of two parts; a certain number of bits, as defined in 'bits for bytes deviation', that describes the difference between what is said in 'bytes between reference' and the reality and a certain number of bits, as defined in 'bits for milliseconds deviation', that describes the difference between what is said in 'milliseconds between reference' and the reality. The number of bits in every reference, i.e. 'bits for bytes deviation'+'bits for milliseconds deviation', must be a multiple of four. There may only be one 'MLLT' frame in each tag."
    },
    {
        "id": "OWNE",
        "title": "Ownership frame",
        "description": "The ownership frame might be used as a reminder of a made transaction or, if signed, as proof. Note that the 'USER' and 'TOWN' frames are good to use in conjunction with this one. The frame begins, after the frame ID, size and encoding fields, with a 'price paid' field. The first three characters of this field contains the currency used for the transaction, encoded according to ISO 4217 [ISO-4217] alphabetic currency code. Concatenated to this is the actual price paid, as a numerical string using '.' as the decimal separator. Next is an 8 character date string (YYYYMMDD) followed by a string with the name of the seller as the last field in the frame. There may only be one 'OWNE' frame in a tag."
    },
    {
        "id": "PRIV",
        "title": "Private frame",
        "description": "This frame is used to contain information from a software producer that its program uses and does not fit into the other frames. The frame consists of an 'Owner identifier' string and the binary data. The 'Owner identifier' is a null-terminated string with a URL [URL] containing an email address, or a link to a location where an email address can be found, that belongs to the organisation responsible for the frame. Questions regarding the frame should be sent to the indicated email address. The tag may contain more than one 'PRIV' frame but only with different contents."
    },
    {
        "id": "PCNT",
        "title": "Play counter",
        "description": "This is simply a counter of the number of times a file has been played. The value is increased by one every time the file begins to play. There may only be one 'PCNT' frame in each tag. When the counter reaches all one's, one byte is inserted in front of the counter thus making the counter eight bits bigger. The counter mustbe at least 32-bits long to begin with."
    },
    {
        "id": "POPM",
        "title": "Popularimeter",
        "description": "The purpose of this frame is to specify how good an audio file is. Many interesting applications could be found to this frame such as a playlist that features better audio files more often than others or it could be used to profile a person's taste and find other 'good' files by comparing people's profiles. The frame contains the email address to the user, one rating byte and a four byte play counter, intended to be increased with one for every time the file is played. The email is a terminated string. The rating is 1-255 where 1 is worst and 255 is best. 0 is unknown. If no personal counter is wanted it may be omitted. When the counter reaches all one's, one byte is inserted in front of the counter thus making the counter eight bits bigger in the same away as the play counter ('PCNT'). There may be more than one 'POPM' frame in each tag, but only one with the same email address."
    },
    {
        "id": "POSS",
        "title": "Position synchronisation frame",
        "description": "This frame delivers information to the listener of how far into the audio stream he picked up; in effect, it states the time offset from the first frame in the stream."
    },
    {
        "id": "RBUF",
        "title": "Recommended buffer size",
        "description": "Sometimes the server from which an audio file is streamed is aware of transmission or coding problems resulting in interruptions in the audio stream. In these cases, the size of the buffer can be recommended by the server using this frame. If the 'embedded info flag' is true (1) then this indicates that an ID3 tag with the maximum size described in 'Buffer size' may occur in the audio stream. In such case the tag should reside between two MPEG [MPEG] frames, if the audio is MPEG encoded. If the position of the next tag is known, 'offset to next tag' may be used. The offset is calculated from the end of tag in which this frame resides to the first byte of the header in the next. This field may be omitted. Embedded tags are generally not recommended since this could render unpredictable behaviour from present software/hardware. For applications like streaming audio it might be an idea to embed tags into the audio stream though. If the clients connects to individual connections like HTTP and there is a possibility to begin every transmission with a tag, then this tag should include a 'recommended buffer size' frame. If the client is connected to a arbitrary point in the stream, such as radio or multicast, then the 'recommended buffer size' frame SHOULD be included in every tag. The 'Buffer size' should be kept to a minimum. There may only be one 'RBUF' frame in each tag."
    },
    {
        "id": "RVA2",
        "title": "Relative volume adjustment",
        "description": "This is a more subjective frame than the previous ones. It allows the user to say how much he wants to increase/decrease the volume on each channel when the file is played. The purpose is to be able to align all files to a reference volume, so that you don't have to change the volume constantly. This frame may also be used to balance adjust the audio. The volume adjustment is encoded as a fixed point decibel value, 16 bit signed integer representing (adjustment*512), giving +/- 64 dB with a precision of 0.001953125 dB. E.g. +2 dB is stored as $04 00 and -2 dB is $FC 00. There may be more than one 'RVA2' frame in each tag, but only one with the same identification string."
    },
    {
        "id": "RVRB",
        "title": "Reverb",
        "description": "Yet another subjective frame, with which you can adjust echoes of different kinds. Reverb left/right is the delay between every bounce in ms. Reverb bounces left/right is the number of bounces that should be made. $FF equals an infinite number of bounces. Feedback is the amount of volume that should be returned to the next echo bounce. $00 is 0%, $FF is 100%. If this value were $7F, there would be 50% volume reduction on the first bounce, 50% of that on the second and so on. Left to left means the sound from the left bounce to be played in the left speaker, while left to right means sound from the left bounce to be played in the right speaker. 'Premix left to right' is the amount of left sound to be mixed in the right before any reverb is applied, where $00 id 0% and $FF is 100%. 'Premix right to left' does the same thing, but right to left. Setting both premix to $FF would result in a mono output (if the reverb is applied symmetric). There may only be one 'RVRB' frame in each tag."
    },
    {
        "id": "SEEK",
        "title": "Seek frame",
        "description": "This frame indicates where other tags in a file/stream can be found. The 'minimum offset to next tag' is calculated from the end of this tag to the beginning of the next. There may only be one 'seek frame' in a tag."
    },
    {
        "id": "SIGN",
        "title": "Signature frame",
        "description": "This frame enables a group of frames, grouped with the 'Group identification registration', to be signed. Although signatures can reside inside the registration frame, it might be desired to store the signature elsewhere, e.g. in watermarks. There may be more than one 'signature frame' in a tag, but no two may be identical."
    },
    {
        "id": "SYLT",
        "title": "Synchronised lyric/text",
        "description": "This is another way of incorporating the words, said or sung lyrics, in the audio file as text, this time, however, in sync with the audio. It might also be used to describing events e.g. occurring on a stage or on the screen in sync with the audio. The header includes a content descriptor, represented with as terminated text string. If no descriptor is entered, 'Content descriptor' is $00 (00) only."
    },
    {
        "id": "SYTC",
        "title": "Synchronised tempo codes",
        "description": " For a more accurate description of the tempo of a musical piece, this frame might be used. After the header follows one byte describing which time stamp format should be used. Then follows one or more tempo codes. Each tempo code consists of one tempo part and one time part. The tempo is in BPM described with one or two bytes. If the first byte has the value $FF, one more byte follows, which is added to the first giving a range from 2 - 510 BPM, since $00 and $01 is reserved. $00 is used to describe a beat-free time period, which is not the same as a music-free time period. $01 is used to indicate one single beat-stroke followed by a beat-free period. The tempo descriptor is followed by a time stamp. Every time the tempo in the music changes, a tempo descriptor may indicate this for the player. All tempo descriptors MUST be sorted in chronological order. The first beat-stroke in a time-period is at the same time as the beat description occurs. There may only be one 'SYTC' frame in each tag."
    },
    {
        "id": "TALB",
        "title": "Album/Movie/Show title",
        "description": "The 'Album/Movie/Show title' frame is intended for the title of the recording (or source of sound) from which the audio in the file is taken.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TBPM",
        "title": "BPM (beats per minute)",
        "description": "The 'BPM' frame contains the number of beats per minute in the main part of the audio. The BPM is an integer and represented as a numerical string.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TCOM",
        "title": "Composer",
        "description": "The 'Composer' frame is intended for the name of the composer.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TCON",
        "title": "Content type",
        "description": "The 'Content type', which ID3v1 was stored as a one byte numeric value only, is now a string. You may use one or several of the ID3v1 types as numerical strings, or, since the category list would be impossible to maintain with accurate and up to date categories, define your own. Example: '21' $00 'Eurodisco' $00",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TCOP",
        "title": "Copyright message",
        "description": "The 'Copyright message' frame, in which the string must begin with a year and a space character (making five characters), is intended for the copyright holder of the original sound, not the audio file itself. The absence of this frame means only that the copyright information is unavailable or has been removed, and must not be interpreted to mean that the audio is public domain. Every time this field is displayed the field must be preceded with 'Copyright (C)' , where (C) is one character showing a C in a circle.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TDEN",
        "title": "Encoding time",
        "description": "The 'Encoding time' frame contains a timestamp describing when the audio was encoded. Timestamp format is described in the ID3v2 structure document [ID3v2-strct].",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TDLY",
        "title": "Playlist delay",
        "description": "The 'Playlist delay' defines the numbers of milliseconds of silence that should be inserted before this audio. The value zero indicates that this is a part of a multifile audio track that should be played continuously.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TDOR",
        "title": "Original release time",
        "description": "The 'Original release time' frame contains a timestamp describing when the original recording of the audio was released. Timestamp format is described in the ID3v2 structure document [ID3v2-strct].",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }    
    },
    {
        "id": "TDRC",
        "title": "Recording time",
        "description": "The 'Recording time' frame contains a timestamp describing when the audio was recorded. Timestamp format is described in the ID3v2 structure document [ID3v2-strct].",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TDRL",
        "title": "Release time",
        "description": "The 'Release time' frame contains a timestamp describing when the audio was first released. Timestamp format is described in the ID3v2 structure document [ID3v2-strct].",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TDTG",
        "title": "Tagging time",
        "description": "The 'Tagging time' frame contains a timestamp describing then the audio was tagged. Timestamp format is described in the ID3v2 structure document [ID3v2-strct].",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TENC",
        "title": "Encoded by",
        "description": "The 'Encoded by' frame contains the name of the person or organisation that encoded the audio file. This field may contain a copyright message, if the audio file also is copyrighted by the encoder.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TEXT",
        "title": "Lyricist/Text writer",
        "description": "The 'Lyricist/Text writer' frame is intended for the writer of the text or lyrics in the recording."
    },
    {
        "id": "TFLT",
        "title": "File type",
        "description": "The 'File type' frame indicates which type of audio this tag defines. The following types and refinements are defined: MPG1, MPG2, MPG3, MPG2.5 MPGAAC, VQF, PCM but other types may be used, but not for these types though. This is used in a similar way to the predefined types in the 'TMED' frame, but without parentheses. If this frame is not present audio type is assumed to be 'MPG'.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TIPL",
        "title": "Involved people list",
        "description": "The 'Involved people list' is very similar to the musician credits list, but maps between functions, like producer, and names.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TIT1",
        "title": "Content group description",
        "description": "The 'Content group description' frame is used if the sound belongs to a larger category of sounds/music. For example, classical music is often sorted in different musical sections (e.g. 'Piano Concerto',  'Weather - Hurricane').",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TIT2",
        "title": "Title/songname/content description",
        "description": "The 'Title/Songname/Content description' frame is the actual name of the piece (e.g. 'Adagio', 'Hurricane Donna').",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TIT3",
        "title": "Subtitle/Description refinement",
        "description": "The 'Subtitle/Description refinement' frame is used for information directly related to the contents title (e.g. 'Op. 16' or 'Performed live at Wembley').",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TKEY",
        "title": "Initial key",
        "description": "The 'Initial key' frame contains the musical key in which the sound starts. It is represented as a string with a maximum length of three characters. The ground keys are represented with 'A','B','C','D','E', 'F' and 'G' and halfkeys represented with 'b' and '#'. Minor is represented as 'm', e.g. 'Dbm' $00. Off key is represented with an 'o' only.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TLAN",
        "title": "Language(s)",
        "description": "The 'Language' frame should contain the languages of the text or lyrics spoken or sung in the audio. The language is represented with three characters according to ISO-639-2 [ISO-639-2]. If more than one language is used in the text their language codes should follow according to the amount of their usage, e.g. 'eng' $00 'sve' $00.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TLEN",
        "title": "Length",
        "description": "The 'Length' frame contains the length of the audio file in milliseconds, represented as a numeric string.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TMCL",
        "title": "Musician credits list",
        "description": "The 'Musician credits list' is intended as a mapping between instruments and the musician that played it. Every odd field is an instrument and every even is an artist or a comma delimited list of artists.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TMED",
        "title": "Media type",
        "description": "The 'Media type' frame describes from which media the sound originated. This may be a text string or a reference to the predefined media types found in the list below. Example: 'VID/PAL/VHS' $00.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TMOO",
        "title": "Mood",
        "description": "The 'Mood' frame is intended to reflect the mood of the audio with a few keywords, e.g. 'Romantic' or 'Sad'.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TOAL",
        "title": "Original album/movie/show title",
        "description": "The 'Original album/movie/show title' frame is intended for the title of the original recording (or source of sound), if for example the music in the file should be a cover of a previously released song.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TOFN",
        "title": "Original filename",
        "description":  "The 'Original filename' frame contains the preferred filename for the file, since some media doesn't allow the desired length of the filename. The filename is case sensitive and includes its suffix.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TOLY",
        "title": "Original lyricist(s)/text writer(s)",
        "description": "The 'Original lyricist/text writer' frame is intended for the text writer of the original recording, if for example the music in the file should be a cover of a previously released song.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TOPE",
        "title": "Original artist(s)/performer(s)",
        "description": "The 'Original artist/performer' frame is intended for the performer of the original recording, if for example the music in the file should be a cover of a previously released song.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TOWN",
        "title": "File owner/licensee",
        "description": "The 'File owner/licensee' frame contains the name of the owner or licensee of the file and it's contents.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TPE1",
        "title": "Lead performer(s)/Soloist(s)",
        "description": "The 'Lead artist/Lead performer/Soloist/Performing group' is used for the main artist.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TPE2",
        "title": "Band/orchestra/accompaniment",
        "description": "The 'Band/Orchestra/Accompaniment' frame is used for additional information about the performers in the recording.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TPE3",
        "title": "Conductor/performer refinement",
        "description": "The 'Conductor' frame is used for the name of the conductor.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TPE4",
        "title": "Interpreted, remixed, or otherwise modified by",
        "description": "The 'Interpreted, remixed, or otherwise modified by' frame contains more information about the people behind a remix and similar interpretations of another existing piece.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TPOS",
        "title": "Part of a set",
        "description": "The 'Part of a set' frame is a numeric string that describes which part of a set the audio came from. This frame is used if the source described in the 'TALB' frame is divided into several mediums, e.g. a double CD. The value MAY be extended with a '/' character and a numeric string containing the total number of parts in the set. E.g. '1/2'.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TPRO",
        "title": "Produced notice",
        "description": "The 'Produced notice' frame, in which the string must begin with a year and a space character (making five characters), is intended for the production copyright holder of the original sound, not the audio file itself. The absence of this frame means only that the production copyright information is unavailable or has been removed, and must not be interpreted to mean that the audio is public domain. Every time this field is displayed the field must be preceded with 'Produced (P)', where (P) is one character showing a P in a circle.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TPUB",
        "title": "Publisher",
        "description": "The 'Publisher' frame simply contains the name of the label or publisher.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TRCK",
        "title": "Track number/Position in set",
        "description": "The 'Track number/Position in set' frame is a numeric string containing the order number of the audio-file on its original recording. This MAY be extended with a "/" character and a numeric string containing the total number of tracks/elements on the original recording. E.g. '4/9'.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TRSN",
        "title": "Internet radio station name",
        "description": "The 'Internet radio station name' frame contains the name of the internet radio station from which the audio is streamed.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TRSO",
        "title": "Internet radio station owner",
        "description": "The 'Internet radio station owner' frame contains the name of the owner of the internet radio station from which the audio is streamed.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TSOA",
        "title": "Album sort order",
        "description": "The 'Album sort order' frame defines a string which should be used instead of the album name (TALB) for sorting purposes. E.g. an album named 'A Soundtrack' might preferably be sorted as 'Soundtrack'.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TSOP",
        "title": "Performer sort order",
        "description": "The 'Performer sort order' frame defines a string which should be used instead of the performer (TPE2) for sorting purposes.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TSOT",
        "title": "Title sort order",
        "description": "The 'Title sort order' frame defines a string which should be used instead of the title (TIT2) for sorting purposes.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TSRC",
        "title": "ISRC (international standard recording code)",
        "description": "The 'ISRC' frame should contain the International Standard Recording Code [ISRC] (12 characters).",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TSSE",
        "title": "Software/Hardware and settings used for encoding",
        "description": "The 'Software/Hardware and settings used for encoding' frame includes the used audio encoder and its settings when the file was encoded. Hardware refers to hardware encoders, not the computer on which a program was run.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TSST",
        "title": "Set subtitle",
        "description": "The 'Set subtitle' frame is intended for the subtitle of the part of a set this track belongs to.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "TXXX",
        "title": "User defined text information frame",
        "description": "This frame is intended for one-string text information concerning the audio file in a similar way to the other 'T'-frames. The frame body consists of a description of the string, represented as a terminated string, followed by the actual string. There may be more than one 'TXXX' frame in each tag, but only one with the same description.",
        "schema": {
            "$schema": "http://json-schema.org/schema#",
            "id": {
                "type": "string",
                "pattern": "^\S{4}$"
            },
            "text": {
                "type": "string",
                "pattern": "^.+$"
            }
        }
    },
    {
        "id": "UFID",
        "title": "Unique file identifier",
        "description": "This frame's purpose is to be able to identify the audio file in a database, that may provide more information relevant to the content. Since standardisation of such a database is beyond this document, all UFID frames begin with an 'owner identifier' field. It is a null-terminated string with a URL [URL] containing an email address, or a link to a location where an email address can be found, that belongs to the organisation responsible for this specific database implementation. Questions regarding the database should be sent to the indicated email address. The URL should not be used for the actual database queries. The string 'http://www.id3.org/dummy/ufid.html' should be used for tests. The 'Owner identifier' must be non-empty (more than just a termination). The 'Owner identifier' is then followed by the actual identifier, which may be up to 64 bytes. There may be more than one 'UFID' frame in a tag, but only one with the same 'Owner identifier'."
    },
    {
        "id": "USER",
        "title": "Terms of use",
        "description": "This frame contains a brief description of the terms of use and ownership of the file. More detailed information concerning the legal terms might be available through the 'WCOP' frame. Newlines are allowed in the text. There may be more than one 'Terms of use' frame in a tag, but only one with the same 'Language'."
    },
    {
        "id": "USLT",
        "title": "Unsynchronised lyric/text transcription",
        "description": "This frame contains the lyrics of the song or a text transcription of other vocal activities. The head includes an encoding descriptor and a content descriptor. The body consists of the actual text. The 'Content descriptor' is a terminated string. If no descriptor is entered, 'Content descriptor' is $00 (00) only. Newline characters are allowed in the text. There may be more than one 'Unsynchronised lyrics/text transcription' frame in each tag, but only one with the same language and content descriptor."
    },
    {
        "id": "WCOM",
        "title": "Commercial information",
        "description": "The 'Commercial information' frame is a URL pointing at a webpage with information such as where the album can be bought. There may be more than one 'WCOM' frame in a tag, but not with the same content."
    },
    {
        "id": "WCOP",
        "title": "Copyright/Legal information",
        "description": "The 'Copyright/Legal information' frame is a URL pointing at a webpage where the terms of use and ownership of the file is described."
    },
    {
        "id": "WOAF",
        "title": "Official audio file webpage",
        "description": "The 'Official audio file webpage' frame is a URL pointing at a file specific webpage."
    },
    {
        "id": "WOAR",
        "title": "Official artist/performer webpage",
        "description": "The 'Official artist/performer webpage' frame is a URL pointing at the artists official webpage. There may be more than one 'WOAR' frame in a tag if the audio contains more than one performer, but not with the same content."
    },
    {
        "id": "WOAS",
        "title": "Official audio source webpage",
        "description": "The 'Official audio source webpage' frame is a URL pointing at the official webpage for the source of the audio file, e.g. a movie."
    },
    {
        "id": "WORS",
        "title": "Official Internet radio station homepage",
        "description": "The 'Official Internet radio station homepage' contains a URL pointing at the homepage of the internet radio station."
    },
    {
        "id": "WPAY",
        "title": "Payment",
        "description": "The 'Payment' frame is a URL pointing at a webpage that will handle the process of paying for this file."
    },
    {
        "id": "WPUB",
        "title": "Publishers official webpage",
        "description": "The 'Publishers official webpage' frame is a URL pointing at the official webpage for the publisher."
    },
    {
        "id": "WXXX",
        "title": "User defined URL link frame",
        "description": "This frame is intended for URL [URL] links concerning the audio file in a similar way to the other 'W'-frames. The frame body consists of a description of the string, represented as a terminated string, followed by the actual URL. The URL is always encoded with ISO-8859-1 [ISO-8859-1]. There may be more than one 'WXXX' frame in each tag, but only one with the same description."
    }
];