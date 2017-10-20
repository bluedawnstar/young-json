# youngJSON
<b>youngJSON</b> is very simple JSON parser written by C++.

### To parse JSON string
```
    using namespace std;
    using namespace youngJSON;

    JsonReader reader(etUTF8);

    try {
        auto value = reader.read(s);
        cout << value->toString() << endl;
        cout << value->toCompactString() << endl;
    } catch (invalid_argument& e) {
        cout << "range_error exception occurred!" << endl;
    } catch (invalid_format_error& e) {
        cout << "invalid_format_error exception occurred!" << endl;
    }
```

### To generate JSON string
```
    using namespace std;
    using namespace youngJSON;

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
```

- output
```
{
	"key1": 3,
	"key2": null,
	"key3": true,
	"key4": false,
	"key5": "aaa",
	"key6": [
	5,
	{
		"key6_1": "bbb"
	}
	],
	"key7": {
	"key7_1": 3100.000000,
	"key7_2": 2
	}
}
{"key1":3,"key2":null,"key3":true,"key4":false,"key5":"aaa","key6":[5,{"key6_1":"bbb"}],"key7":{"key7_1":3100.000000,"key7_2":2}}
```