#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

namespace youngJSON {

enum ValueTypeT {
    vtNull,
    vtFalse,
    vtTrue,
    vtNumber,
    vtString,
    vtObject,
    vtArray
};

class JsonValue {
public:
    virtual ~JsonValue() {
    }

    virtual bool toBool() const {
        return false;
    }

    virtual int toInt32() const {
        return 0;
    }

    virtual long long toInt64() const {
        return 0ll;
    }

    virtual unsigned int toUInt32() const {
        return 0u;
    }

    virtual unsigned long long toUInt64() const {
        return 0ull;
    }

    virtual double toDouble() const {
        return 0.0;
    }

    virtual std::string toRawString() const {
        return toStringIndent(0);
    }

    virtual std::string toString() const {
        return toStringIndent(0);
    }

    virtual std::string toStringIndent(int indentation) const = 0;

    virtual std::string toCompactString() const {
        return toStringIndent(0);
    }

    virtual int size() const {
        return 0;
    }

    virtual const JsonValue& get(int index) const {
        return const_cast<JsonValue*>(this)->get(index);
    }

    virtual const JsonValue& get(const std::string& name) const {
        return const_cast<JsonValue*>(this)->get(name);
    }

    virtual JsonValue& get(int index) {
        throw std::invalid_argument("can't be accessed by index");
    }

    virtual JsonValue& get(const std::string& name) {
        throw std::invalid_argument("can't be accessed by name");
    }

    bool isNull() const {
        return mType == vtNull;
    }

    bool isTrue() const {
        return mType == vtTrue;
    }

    bool isFalse() const {
        return mType == vtFalse;
    }

    bool isBool() const {
        return mType == vtTrue || mType == vtFalse;
    }

    bool isNumber() const {
        return mType == vtNumber;
    }

    bool isString() const {
        return mType == vtString;
    }

    bool isObject() const {
        return mType == vtObject;
    }

    bool isArray() const {
        return mType == vtArray;
    }

    const JsonValue& operator [](const std::string& name) const {
        return get(name);
    }

    JsonValue& operator [](const std::string& name) {
        return get(name);
    }

    const JsonValue& operator [](int idx) const {
        return get(idx);
    }

    JsonValue& operator [](int idx) {
        return get(idx);
    }

protected:
    JsonValue(ValueTypeT type = vtNull) : mType(type) {
    }

protected:
    ValueTypeT  mType;
};


class ValueNull : public JsonValue {
public:
    ValueNull() : JsonValue(vtNull) {
    }
    
    std::string toStringIndent(int indentation) const override {
        return "null";
    }
};

class ValueFalse : public JsonValue {
public:
    ValueFalse() : JsonValue(vtFalse) {
    }

    std::string toStringIndent(int indentation) const override {
        return "false";
    }
};

class ValueTrue : public JsonValue {
public:

    ValueTrue() : JsonValue(vtTrue) {
    }

    bool toBool() const override {
        return true;
    }

    int toInt32() const override {
        return 1;
    }

    long long toInt64() const override {
        return 1ll;
    }

    unsigned int toUInt32() const override {
        return 1u;
    }

    unsigned long long toUInt64() const override {
        return 1ull;
    }

    double toDouble() const override {
        return 1.0;
    }

    std::string toStringIndent(int indentation) const override {
        return "true";
    }
};

class ValueString : public JsonValue {
public:
    ValueString() : JsonValue(vtString) {
    }

    explicit ValueString(const char* s) : JsonValue(vtString), mValue(s) {
    }

    explicit ValueString(const std::string& s) : JsonValue(vtString), mValue(s) {
    }

    bool toBool() const override {
        return !mValue.empty();
    }

    int toInt32() const override {
        return std::stoi(mValue);
    }

    long long toInt64() const override {
        return std::stoll(mValue);
    }

    unsigned int toUInt32() const override {
        return static_cast<int>(std::stoul(mValue));
    }

    unsigned long long toUInt64() const override {
        return std::stoull(mValue);
    }

    double toDouble() const override {
        return std::stod(mValue);
    }

    std::string toRawString() const override {
        return mValue;
    }

    std::string toStringIndent(int indentation) const override {
        return std::string("\"") + mValue + "\"";
    }

    void setValue(const char* s) {
        mValue = s;
    }

    void setValue(const std::string& s) {
        mValue = s;
    }

private:
    std::string mValue;
};

struct ValueNumber : public JsonValue {
    ValueNumber() : JsonValue(vtNumber) {
    }

    explicit ValueNumber(const char* s) : JsonValue(vtNumber), mValue(s) {
    }

    explicit ValueNumber(const std::string& s) : JsonValue(vtNumber), mValue(s) {
    }

    template <typename T>
    explicit ValueNumber(T value) : JsonValue(vtNumber), mValue(std::to_string(value)) {
    }

    bool toBool() const override {
        return toInt64() == 0ull && toDouble() == 0.0;
    }

    int toInt32() const override {
        return std::stoi(mValue);
    }

    long long toInt64() const override {
        return std::stoll(mValue);
    }

    unsigned int toUInt32() const override {
        return static_cast<int>(std::stoul(mValue));
    }

    unsigned long long toUInt64() const override {
        return std::stoull(mValue);
    }

    double toDouble() const override {
        return std::stod(mValue);
    }

    std::string toStringIndent(int indentation) const override {
        return mValue;
    }

    void setValue(const char* s) {
        mValue = s;
    }

    void setValue(const std::string& s) {
        mValue = s;
    }

    template <typename T>
    void setValue(T value) {
        mValue = std::to_string(value);
    }

private:
    std::string mValue;
};


struct ValueObject : public JsonValue {
    ValueObject() : JsonValue(vtObject) {
    }

    int size() const override {
        return static_cast<int>(mChildren.size());
    }

    const JsonValue& get(int index) const override {
        return const_cast<ValueObject*>(this)->get(index);
    }

    const JsonValue& get(const std::string& name) const override {
        return const_cast<ValueObject*>(this)->get(name);
    }

    JsonValue& get(int index) override {
        throw std::invalid_argument("can't be accessed by index");
    }

    JsonValue& get(const std::string& name) override {
        return *mChildren[name];
    }

    bool toBool() const override {
        return !mChildren.empty();
    }

    std::string toStringIndent(int indentation) const override {
        std::string res;
        std::string spaces1(indentation, ' ');
        std::string spaces2(indentation + 2, ' ');

        res += "{\n";

        int cnt = 0;
        for (auto& it : mChildren) {
            if (cnt++ > 0)
                res += ",\n";
            res += spaces2;
            res += '\"';
            res += it.first;
            res += "\": ";
            res += it.second->toStringIndent(indentation + 2);
        }
        res += "\n";
        res += spaces1;
        res += "}";

        return res;
    }

    std::string toCompactString() const override {
        std::string res;

        res += "{";

        int cnt = 0;
        for (auto& it : mChildren) {
            if (cnt++ > 0)
                res += ",";
            res += '\"';
            res += it.first;
            res += "\":";
            res += it.second->toCompactString();
        }

        res += "}";

        return res;
    }

    template <typename Function>
    void forEach(Function fn) const {
        for (const auto& it : mChildren) {
            fn(it.first, *it.second);
        }
    }

    template <typename Function>
    void forEach(Function fn) {
        for (auto& it : mChildren) {
            fn(it.first, *it.second);
        }
    }

    JsonValue& add(const std::string& name, std::shared_ptr<JsonValue> value) {
        mChildren[name] = value;
        return *value;
    }

    template <typename T>
    T& add(const std::string& name, const T& value) {
        auto p = std::make_shared<T>(value);
        mChildren[name] = p;
        return dynamic_cast<T&>(*p);
    }

private:
    std::unordered_map<std::string, std::shared_ptr<JsonValue>> mChildren;
};

struct ValueArray : public JsonValue {
    ValueArray() : JsonValue(vtArray) {
    }

    int size() const override {
        return static_cast<int>(mChildren.size());
    }

    const JsonValue& get(int index) const override {
        return const_cast<ValueArray*>(this)->get(index);
    }

    const JsonValue& get(const std::string& name) const override {
        return const_cast<ValueArray*>(this)->get(name);
    }

    JsonValue& get(int index) override {
        return *mChildren[index];
    }

    JsonValue& get(const std::string& name) override {
        return JsonValue::get(name);
    }

    bool toBool() const override {
        return !mChildren.empty();
    }

    std::string toStringIndent(int indentation) const override {
        std::string res;
        std::string spaces1(indentation, ' ');
        std::string spaces2(indentation + 2, ' ');

        res += "[\n";

        int cnt = 0;
        for (auto& it : mChildren) {
            if (cnt++ > 0)
                res += ",\n";
            res += spaces2;
            res += it->toStringIndent(indentation + 2);
        }
        res += "\n";
        res += spaces1;
        res += "]";

        return res;
    }

    std::string toCompactString() const override {
        std::string res;

        res += "[";

        int cnt = 0;
        for (auto& it : mChildren) {
            if (cnt++ > 0)
                res += ",";
            res += it->toCompactString();
        }

        res += "]";

        return res;
    }

    template <typename Function>
    void forEach(Function fn) const {
        for (int i = 0; i < (int)mChildren.size(); i++) {
            fn(i, *mChildren[i]);
        }
    }

    template <typename Function>
    void forEach(Function fn) {
        for (int i = 0; i < (int)mChildren.size(); i++) {
            fn(i, *mChildren[i]);
        }
    }

    JsonValue& add(std::shared_ptr<JsonValue> value) {
        mChildren.push_back(value);
        return *value;
    }

    template <typename T>
    T& add(const T& value) {
        auto p = std::make_shared<T>(value);
        mChildren.push_back(p);
        return dynamic_cast<T&>(*p);
    }

private:
    std::vector<std::shared_ptr<JsonValue>> mChildren;
};

inline bool operator ==(const JsonValue& lhs, const JsonValue& rhs) {
    if (lhs.isNull())
        return rhs.isNull();
    else if (lhs.isTrue())
        return rhs.isTrue();
    else if (lhs.isFalse())
        return rhs.isFalse();
    else if (lhs.isNumber())
        return rhs.isNumber() && lhs.toString() == rhs.toString();
    else if (lhs.isString())
        return rhs.isString() && lhs.toString() == rhs.toString();
    else if (lhs.isObject()) {
        if (rhs.isObject()) {
            bool ok = true;
            dynamic_cast<const ValueObject&>(lhs).forEach([&rhs, &ok](const std::string& name, const JsonValue& value) {
                if (!(value == rhs.get(name)))
                    ok = false;
            });
            return ok;
        }
    } else if (lhs.isArray()) {
        if (rhs.isArray()) {
            bool ok = true;
            dynamic_cast<const ValueArray&>(lhs).forEach([&rhs, &ok](int idx, const JsonValue& value) {
                if (!(value == rhs.get(idx)))
                    ok = false;
            });
            return ok;
        }
    }
    return false;
}

inline bool operator !=(const JsonValue& lhs, const JsonValue& rhs) {
    return !operator ==(lhs, rhs);
}

}
