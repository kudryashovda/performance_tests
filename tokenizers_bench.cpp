#include <algorithm>
#include <cassert>
#include <deque>
#include <future>
#include <iostream>
#include <list>
#include <numeric>
#include <vector>

#include "log_duration.h"

using namespace std;

string GenerateLongString(int count, string content, string delim) {
    string result;

    result.reserve(count * content.size());

    result = content;

    for (int i = 1; i < count; ++i) {
        result += delim + content;
    }

    return result;
}

void tokenizer1(std::deque<std::string>& vs, std::string_view str, std::string_view delim) {
    if (str.empty() || delim.empty()) {
        return;
    }

    size_t delim_pos = str.find(delim);

    while (delim_pos != std::string_view::npos) {
        vs.emplace_back(str.begin(), str.begin() + delim_pos);
        str.remove_prefix(delim_pos + delim.size()); // [_>_]
        delim_pos = str.find(delim);
    }

    vs.emplace_back(str.begin(), str.end());
}

void tokenizer2(std::vector<std::string>& vs, std::string_view str, std::string_view delim) {
    if (str.empty() || delim.empty()) {
        return;
    }

    size_t delim_pos = str.find(delim);

    while (delim_pos != std::string_view::npos) {
        vs.emplace_back(str.begin(), str.begin() + delim_pos);
        str.remove_prefix(delim_pos + delim.size()); // [_>_]
        delim_pos = str.find(delim);
    }

    vs.emplace_back(str.begin(), str.end());
}

std::deque<std::string_view> tokenizer3(std::string_view str, std::string_view delim) {
    if (str.empty() || delim.empty()) {
        return {};
    }

    std::deque<std::string_view> result;

    size_t delim_pos = str.find(delim);

    while (delim_pos != std::string_view::npos) {
        auto value = str.substr(0, delim_pos);
        result.emplace_back(value);
        str.remove_prefix(delim_pos + delim.size()); // [_>_]
        delim_pos = str.find(delim);
    }
    result.emplace_back(str);

    return result;
}

std::vector<std::string_view> tokenizer4(std::string_view str, std::string_view delim) {
    if (str.empty() || delim.empty()) {
        return {};
    }

    std::vector<std::string_view> result;

    size_t delim_pos = str.find(delim);

    while (delim_pos != std::string_view::npos) {
        auto value = str.substr(0, delim_pos);
        result.emplace_back(value);
        str.remove_prefix(delim_pos + delim.size()); // [_>_]
        delim_pos = str.find(delim);
    }
    result.emplace_back(str);

    return result;
}

std::deque<std::string_view> tokenizer5(std::string_view str, std::string_view delim) {
    if (str.empty() || delim.empty()) {
        return {};
    }

    std::deque<std::string_view> result;

    size_t delim_pos = str.find(delim);

    while (delim_pos != std::string_view::npos) {
        auto value = str.substr(0, delim_pos);
        result.push_back(value);
        str.remove_prefix(delim_pos + delim.size()); // [_>_]
        delim_pos = str.find(delim);
    }
    result.push_back(str);

    return result;
}

std::list<std::string_view> tokenizer6(std::string_view str, std::string_view delim) {
    if (str.empty() || delim.empty()) {
        return {};
    }

    std::list<std::string_view> result;

    size_t delim_pos = str.find(delim);

    while (delim_pos != std::string_view::npos) {
        auto value = str.substr(0, delim_pos);
        result.push_back(value);
        str.remove_prefix(delim_pos + delim.size()); // [_>_]
        delim_pos = str.find(delim);
    }
    result.push_back(str);

    return result;
}

std::deque<std::string_view> tokenizer7(std::string_view str, std::string_view delim) {
    if (str.empty() || delim.empty()) {
        return {};
    }

    std::deque<std::string_view> result;

    std::deque<std::string_view> result3;
    std::deque<std::string_view> result4;
    std::deque<std::string_view> result5;
    std::deque<std::string_view> result6;

    auto mid_pos = str.find(delim, str.size() / 2);
    auto str1 = str.substr(0, mid_pos);
    auto str2 = str.substr(mid_pos + 1);

    auto mid_pos11 = str.find(delim, str1.size() / 2);
    auto str11 = str1.substr(0, mid_pos11);
    auto str12 = str1.substr(mid_pos11 + 1);

    auto mid_pos12 = str.find(delim, str2.size() / 2);
    auto str21 = str2.substr(0, mid_pos12);
    auto str22 = str2.substr(mid_pos12 + 1);

    auto foo = [&delim](std::string_view str, std::deque<std::string_view>& res) {
        size_t delim_pos = str.find(delim);

        while (delim_pos != std::string_view::npos) {
            auto value = str.substr(0, delim_pos);
            res.push_back(value);

            str.remove_prefix(delim_pos + delim.size()); // [_>_]
            delim_pos = str.find(delim);
        }
        res.push_back(str);
    };

    auto vs_async3 = async(foo, str11, std::ref(result3));
    auto vs_async4 = async(foo, str12, std::ref(result4));
    auto vs_async5 = async(foo, str21, std::ref(result5));
    auto vs_async6 = async(foo, str22, std::ref(result6));

    vs_async3.get();
    vs_async4.get();
    vs_async5.get();
    vs_async6.get();

    result.insert(result.end(), result3.begin(), result3.end());
    result.insert(result.end(), result4.begin(), result4.end());
    result.insert(result.end(), result5.begin(), result5.end());
    result.insert(result.end(), result6.begin(), result6.end());

    return result;
}

int main() {

    constexpr int tokens_count = 1e7;

    auto long_str = GenerateLongString(tokens_count, "aaa"s, "_"s);

    {
        LOG_DURATION("tokenizer1");

        deque<string> v;

        tokenizer1(v, long_str, "_"s);

        assert(v.size() == tokens_count);
        assert(v[0] == "aaa"s);
    }

    {
        LOG_DURATION("tokenizer2");

        vector<string> v;

        tokenizer2(v, long_str, "_"s);

        assert(v.size() == tokens_count);
        assert(v[0] == "aaa"s);
    }

    {
        LOG_DURATION("tokenizer3");

        deque<string_view> v = tokenizer3(long_str, "_"s);

        assert(v.size() == tokens_count);
        assert(v[0] == "aaa"sv);
        assert(v.back() == "aaa"sv);
    }

    {
        LOG_DURATION("tokenizer4");

        vector<string_view> v = tokenizer4(long_str, "_"s);

        assert(v.size() == tokens_count);
        assert(v[0] == "aaa"sv);
        assert(v.back() == "aaa"sv);
    }

    {
        LOG_DURATION("tokenizer5");

        deque<string_view> v = tokenizer5(long_str, "_"s);

        assert(v.size() == tokens_count);
        assert(v[0] == "aaa"sv);
        assert(v.back() == "aaa"sv);
    }
    {
        LOG_DURATION("tokenizer6");

        list<string_view> v = tokenizer6(long_str, "_"s);

        assert(v.size() == tokens_count);
        assert(v.front() == "aaa"sv);
        assert(v.back() == "aaa"sv);
    }

    {
        LOG_DURATION("tokenizer7");

        deque<string_view> v = tokenizer7(long_str, "_"s);

        assert(v.size() == tokens_count);
        assert(v[0] == "aaa"sv);
        assert(v.back() == "aaa"sv);
    }

    return 0;
}
