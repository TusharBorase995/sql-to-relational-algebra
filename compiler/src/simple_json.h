#ifndef SIMPLE_JSON_H
#define SIMPLE_JSON_H

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <cctype>

struct JsonVal {
    enum Type { NIL, BOOL, NUM, STR, ARR, OBJ } type = NIL;
    bool b = false;
    double num = 0;
    std::string str;
    std::vector<std::shared_ptr<JsonVal>> arr;
    std::unordered_map<std::string, std::shared_ptr<JsonVal>> obj;

    bool is_null() const { return type == NIL; }
    bool is_bool() const { return type == BOOL; }
    bool is_num() const { return type == NUM; }
    bool is_str() const { return type == STR; }
    bool is_arr() const { return type == ARR; }
    bool is_obj() const { return type == OBJ; }

    const JsonVal& operator[](const std::string& key) const {
        static JsonVal null_val;
        if (type != OBJ) return null_val;
        auto it = obj.find(key);
        if (it != obj.end() && it->second) return *(it->second);
        return null_val;
    }

    const JsonVal& operator[](size_t idx) const {
        static JsonVal null_val;
        if (type != ARR || idx >= arr.size() || !arr[idx]) return null_val;
        return *(arr[idx]);
    }

    std::string as_string(const std::string& def = "") const {
        return (type == STR) ? str : def;
    }

    bool as_bool(bool def = false) const {
        return (type == BOOL) ? b : def;
    }

    double as_num(double def = 0.0) const {
        return (type == NUM) ? num : def;
    }

    bool has_key(const std::string& key) const {
        return type == OBJ && obj.find(key) != obj.end();
    }
};

class SimpleJsonParser {
public:
    static bool parse(const std::string& src, JsonVal& out) {
        size_t pos = 0;
        skip_ws(src, pos);
        if (pos >= src.size()) return false;
        return parse_value(src, pos, out);
    }

private:
    static void skip_ws(const std::string& s, size_t& pos) {
        while (pos < s.size() && (s[pos] == ' ' || s[pos] == '\t' || s[pos] == '\r' || s[pos] == '\n')) {
            pos++;
        }
    }

    static bool parse_value(const std::string& s, size_t& pos, JsonVal& val) {
        skip_ws(s, pos);
        if (pos >= s.size()) return false;

        char c = s[pos];
        if (c == '{') return parse_object(s, pos, val);
        if (c == '[') return parse_array(s, pos, val);
        if (c == '"') return parse_string(s, pos, val);
        if (c == 't' || c == 'f') return parse_bool(s, pos, val);
        if (c == 'n') return parse_null(s, pos, val);
        if (c == '-' || (c >= '0' && c <= '9')) return parse_number(s, pos, val);
        return false;
    }

    static bool parse_string(const std::string& s, size_t& pos, JsonVal& val) {
        if (s[pos] != '"') return false;
        pos++;
        std::string res;
        while (pos < s.size()) {
            char c = s[pos++];
            if (c == '"') {
                val.type = JsonVal::STR;
                val.str = res;
                return true;
            }
            if (c == '\\') {
                if (pos >= s.size()) return false;
                char esc = s[pos++];
                switch (esc) {
                    case '"': res += '"'; break;
                    case '\\': res += '\\'; break;
                    case '/': res += '/'; break;
                    case 'b': res += '\b'; break;
                    case 'f': res += '\f'; break;
                    case 'n': res += '\n'; break;
                    case 'r': res += '\r'; break;
                    case 't': res += '\t'; break;
                    case 'u': {
                        if (pos + 4 <= s.size()) pos += 4;
                        res += '?';
                        break;
                    }
                    default: res += esc; break;
                }
            } else {
                res += c;
            }
        }
        return false;
    }

    static bool parse_object(const std::string& s, size_t& pos, JsonVal& val) {
        if (s[pos] != '{') return false;
        pos++;
        val.type = JsonVal::OBJ;
        val.obj.clear();

        skip_ws(s, pos);
        if (pos < s.size() && s[pos] == '}') {
            pos++;
            return true;
        }

        while (pos < s.size()) {
            skip_ws(s, pos);
            JsonVal key_val;
            if (!parse_string(s, pos, key_val)) return false;

            skip_ws(s, pos);
            if (pos >= s.size() || s[pos] != ':') return false;
            pos++; // skip ':'

            auto item = std::make_shared<JsonVal>();
            if (!parse_value(s, pos, *item)) return false;

            val.obj[key_val.str] = item;

            skip_ws(s, pos);
            if (pos < s.size() && s[pos] == '}') {
                pos++;
                return true;
            }
            if (pos < s.size() && s[pos] == ',') {
                pos++;
            } else {
                return false;
            }
        }
        return false;
    }

    static bool parse_array(const std::string& s, size_t& pos, JsonVal& val) {
        if (s[pos] != '[') return false;
        pos++;
        val.type = JsonVal::ARR;
        val.arr.clear();

        skip_ws(s, pos);
        if (pos < s.size() && s[pos] == ']') {
            pos++;
            return true;
        }

        while (pos < s.size()) {
            auto item = std::make_shared<JsonVal>();
            if (!parse_value(s, pos, *item)) return false;
            val.arr.push_back(item);

            skip_ws(s, pos);
            if (pos < s.size() && s[pos] == ']') {
                pos++;
                return true;
            }
            if (pos < s.size() && s[pos] == ',') {
                pos++;
            } else {
                return false;
            }
        }
        return false;
    }

    static bool parse_number(const std::string& s, size_t& pos, JsonVal& val) {
        size_t start = pos;
        if (s[pos] == '-') pos++;
        while (pos < s.size() && isdigit((unsigned char)s[pos])) pos++;
        if (pos < s.size() && s[pos] == '.') {
            pos++;
            while (pos < s.size() && isdigit((unsigned char)s[pos])) pos++;
        }
        if (pos < s.size() && (s[pos] == 'e' || s[pos] == 'E')) {
            pos++;
            if (pos < s.size() && (s[pos] == '+' || s[pos] == '-')) pos++;
            while (pos < s.size() && isdigit((unsigned char)s[pos])) pos++;
        }
        std::string num_str = s.substr(start, pos - start);
        try {
            val.type = JsonVal::NUM;
            val.num = std::stod(num_str);
            return true;
        } catch (...) {
            return false;
        }
    }

    static bool parse_bool(const std::string& s, size_t& pos, JsonVal& val) {
        if (s.substr(pos, 4) == "true") {
            val.type = JsonVal::BOOL;
            val.b = true;
            pos += 4;
            return true;
        }
        if (s.substr(pos, 5) == "false") {
            val.type = JsonVal::BOOL;
            val.b = false;
            pos += 5;
            return true;
        }
        return false;
    }

    static bool parse_null(const std::string& s, size_t& pos, JsonVal& val) {
        if (s.substr(pos, 4) == "null") {
            val.type = JsonVal::NIL;
            pos += 4;
            return true;
        }
        return false;
    }
};

#endif // SIMPLE_JSON_H
