#ifndef TEXT_DATA_H
#define TEXT_DATA_H

#include "../DragonBones.h"

NAME_SPACE_DRAGON_BONES_BEGIN
class TextData
{
public:
    enum class AlignHType {LEFT, CENTER, RIGHT, JUSTIFY};
    enum class AlignVType {TOP, CENTER, BOTTOM};
    enum class TextType {STATIC, DYNAMIC, INPUT};
    enum class LineType {SINGLE_LINE, MULTILINE, MULTILINE_NO_WRAP, PASSWORD};

    /*
    public var color:uint;
    */

public:
    bool bold;
    bool italic;
    bool htmlText;
    unsigned char size;

    unsigned char alpha;
    unsigned char red;
    unsigned char green;
    unsigned char blue;

    unsigned short int width;
    unsigned short int height;

    short int letterSpacing;
    short int lineSpacing;
    short int maxCharacters;

    std::string face;
    std::string text;

    AlignHType alignH;
    AlignVType alignV;
    TextType textType;
    LineType lineType;

public:
    TextData():
        bold(false),
        italic(false),
        htmlText(false),
        size(0),
        red(0),
        green(0),
        blue(0),
        width(0),
        height(0),

        letterSpacing(0),
        lineSpacing(0),
        maxCharacters(0),

        face(),
        text(),

        alignH(AlignHType::LEFT),
        alignV(AlignVType::TOP),
        textType(TextType::STATIC),
        lineType(LineType::SINGLE_LINE)
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
        htmlText = copyData.htmlText;
        size = copyData.size;
        red = copyData.red;
        green = copyData.green;
        blue = copyData.blue;
        width = copyData.width;
        height = copyData.height;

        letterSpacing = copyData.letterSpacing;
        lineSpacing = copyData.lineSpacing;
        maxCharacters = copyData.maxCharacters;

        face = copyData.face;
        text = copyData.text;

        alignH = copyData.alignH;
        alignV = copyData.alignV;
        textType = copyData.textType;
        lineType = copyData.lineType;

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
        htmlText = false;
        size = 0;
        red = 0;
        green = 0;
        blue = 0;
        width = 0;
        height = 0;

        letterSpacing = 0;
        lineSpacing = 0;
        maxCharacters = 0;

        face.clear();
        text.clear();

        alignH = AlignHType::LEFT;
        alignV = AlignVType::TOP;
        textType = TextType::STATIC;
        lineType = LineType::SINGLE_LINE;
    }
};
NAME_SPACE_DRAGON_BONES_END
#endif  // TEXT_DATA_H
