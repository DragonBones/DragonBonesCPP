#ifndef TEXT_DATA_H
#define TEXT_DATA_H

#include "../DragonBones.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class TextData
{
public:
    enum class AlignHType {LEFT, CENTER, RIGHT, JUSTIFY};
    enum class AlignVType {TOP, CENTER, BOTTOM};

    /*
    public var color:uint;
    public var lineType:String;
    public var textType:String;
    */

public:
    bool bold;
    bool italic;
    unsigned char size;

    unsigned char alpha;
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    unsigned short int width;
    unsigned short int height;

    std::string face;
    std::string text;

    AlignHType alignH;
    AlignVType alignV;

public:
    TextData():
        bold(false),
        italic(false),
        size(0),
        red(0),
        green(0),
        blue(0),
        width(0),
        height(0),
        face(),
        text(),

        alignH(AlignHType::LEFT),
        alignV(AlignVType::TOP)
    {
    }
    TextData(const TextData &copyData)
    {
        operator=(copyData);
    }
    TextData& operator=(const TextData &copyData)
    {
        dispose();

        bold = copyData.bold;
        italic = copyData.italic;
        size = copyData.size;
        red = copyData.red;
        green = copyData.green;
        blue = copyData.blue;
        width = copyData.width;
        height = copyData.height;
        face = copyData.face;
        text = copyData.text;

        alignH = copyData.alignH;
        alignV = copyData.alignV;

        return *this;
    }
    virtual ~TextData()
    {
        dispose();
    }

    void dispose()
    {
        bold = false;
        italic = false;
        size = 0;
        red = 0;
        green = 0;
        blue = 0;
        width = 0;
        height = 0;
        face.clear();
        text.clear();

        alignH = AlignHType::LEFT;
        alignV = AlignVType::TOP;
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // TEXT_DATA_H
