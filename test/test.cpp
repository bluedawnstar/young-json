#include <cassert>
#include <iostream>

#include "../src/include/jsonparser.h"

using namespace std;
using namespace youngJSON;

const string gTestInput[] = {
"{\n\
    \"glossary\": {\n\
        \"title\": \"example glossary\",\n\
        \"GlossDiv\" : {\n\
            \"title\": \"S\",\n\
            \"GlossList\" : {\n\
                \"GlossEntry\": {\n\
                    \"ID\": \"SGML\",\n\
                    \"SortAs\" : \"SGML\",\n\
                    \"GlossTerm\" : \"Standard Generalized Markup Language\",\n\
                    \"Acronym\" : \"SGML\",\n\
                    \"Abbrev\" : \"ISO 8879:1986\",\n\
                    \"GlossDef\" : {\n\
                        \"para\": \"A meta-markup language, used to create markup languages such as DocBook.\",\n\
                        \"GlossSeeAlso\" : [\"GML\", \"XML\"]\n\
                    },\n\
                    \"GlossSee\" : \"markup\"\n\
                }\n\
            }\n\
        }\n\
    }\n\
}",
"{ \"widget\": {\n\
    \"debug\": \"on\",\n\
        \"window\" : {\n\
        \"title\": \"Sample Konfabulator Widget\",\n\
            \"name\" : \"main_window\",\n\
            \"width\" : 500,\n\
            \"height\" : 500\n\
    },\n\
        \"image\": {\n\
            \"src\": \"Images/Sun.png\",\n\
                \"name\" : \"sun1\",\n\
                \"hOffset\" : 250,\n\
                \"vOffset\" : 250,\n\
                \"alignment\" : \"center\"\n\
        },\n\
            \"text\": {\n\
                \"data\": \"Click Here\",\n\
                    \"size\" : 36,\n\
                    \"style\" : \"bold\",\n\
                    \"name\" : \"text1\",\n\
                    \"hOffset\" : 250,\n\
                    \"vOffset\" : 100,\n\
                    \"alignment\" : \"center\",\n\
                    \"onMouseUp\" : \"sun1.opacity = (sun1.opacity / 100) * 90;\"\n\
            }\n\
} }",
"{ \"menu\": {\n\
    \"header\": \"SVG Viewer\",\n\
        \"items\" : [\n\
    {\"id\": \"Open\"},\n\
    { \"id\": \"OpenNew\", \"label\" : \"Open New\" },\n\
        null,\n\
        { \"id\": \"ZoomIn\", \"label\" : \"Zoom In\" },\n\
        { \"id\": \"ZoomOut\", \"label\" : \"Zoom Out\" },\n\
        { \"id\": \"OriginalView\", \"label\" : \"Original View\" },\n\
        null,\n\
        { \"id\": \"Quality\" },\n\
        { \"id\": \"Pause\" },\n\
        { \"id\": \"Mute\" },\n\
        null,\n\
        { \"id\": \"Find\", \"label\" : \"Find...\" },\n\
        { \"id\": \"FindAgain\", \"label\" : \"Find Again\" },\n\
        { \"id\": \"Copy\" },\n\
        { \"id\": \"CopyAgain\", \"label\" : \"Copy Again\" },\n\
        { \"id\": \"CopySVG\", \"label\" : \"Copy SVG\" },\n\
        { \"id\": \"ViewSVG\", \"label\" : \"View SVG\" },\n\
        { \"id\": \"ViewSource\", \"label\" : \"View Source\" },\n\
        { \"id\": \"SaveAs\", \"label\" : \"Save As\" },\n\
        null,\n\
        { \"id\": \"µµ¿ò¸»\\uC8FE\" },\n\
        { \"id\": \"About\", \"label\" : \"About Adobe CVG Viewer...\" }\n\
        ]\n\
}}"
};

int main(void) {
    JsonReader reader(etUTF8);

    for (auto& s : gTestInput) {
        try {
            auto value = reader.read(s);
            cout << value->toString() << endl;
            cout << value->toCompactString() << endl;

            JsonReader reader2;
            auto value2 = reader2.read(value->toCompactString());
            cout << value2->toString() << endl;
            cout << value2->toCompactString() << endl;
            assert(*value == *value2);
        } catch (invalid_argument& e) {
            cout << "range_error exception occurred!" << endl;
        } catch (invalid_format_error& e) {
            cout << "invalid_format_error exception occurred!" << endl;
        }
    }

    cout << "-------------------" << endl;

    ValueObject root;

    root.add("key1", ValueNumber(3));
    root.add("key2", ValueNull());
    root.add("key3", ValueTrue());
    root.add("key4", ValueFalse());

    root.add("key5", ValueString("aaa"));
    auto& va1 = root.add("key6", ValueArray());
    {
        va1.add(ValueNumber(5));
        auto& vo1 = va1.add(ValueObject());
        vo1.add("key6_1", ValueString("bbb"));
    }
    auto& vo2 = root.add("key7", ValueObject());
    {
        vo2.add("key7_1", ValueNumber(3.1e3));
        vo2.add("key7_2", ValueNumber(2));
    }

    cout << root.toString() << endl;
    cout << root.toCompactString() << endl;

    return 0;
}