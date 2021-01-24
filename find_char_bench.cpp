#include <algorithm>
#include <cassert>
#include <execution>
#include <iostream>
#include <vector>

#include "log_duration.h"

void TestSearchSpeed();
void TestSplitString();
void TestMoveString();

using namespace std;

int main() {
    TestSearchSpeed();
    TestSplitString();
    TestMoveString();

    return 0;
}

string CountChars(const string& str) {
    return str;
}

void TestMoveString() {
    const uint64_t size = 5e5;
    const uint64_t vsize = 5000;

    // Generate long string aaa...
    string long_a(size, 'a');
    // Generate big vector with long string aaa...
    vector<string> v(vsize, long_a);

    {
        LOG_DURATION("count chars");

        uint64_t result = 0;
        for (uint64_t i = 0; i < vsize; ++i) {
            result += CountChars(v[i]).size();
        }

        assert(result == size * vsize);
    }
    {
        LOG_DURATION("count chars move");

        uint64_t result = 0;
        for (uint64_t i = 0; i < vsize; ++i) {
            result += CountChars(move(v[i])).size(); // Danger - here vector must be empty
        }

        assert(result == size * vsize);
    }
}

void TestSplitString() {
    const int half_size = 5e7;

    // Generate long string aaa...=bbb...
    string long_a(half_size, 'a');
    string long_b(half_size, 'b');

    string s = long_a + '=' + long_b;

    string_view sv(s); // for other test
    string_view sv_long_a(long_a);
    string_view sv_long_b(long_b);

    int pos = 0;
    {
        LOG_DURATION("with string::substr");

        pos = s.find('=');

        string left = s.substr(0, pos);
        string right = s.substr(pos + 1); // warning with no = char or empty string

        assert(left == long_a);
        assert(right == long_b);
    }

    {
        LOG_DURATION("with begin()-end()");

        pos = s.find('=');

        string left(s.begin(), s.begin() + pos);
        string right(s.begin() + pos + 1, s.end()); // warning with no = char or empty string

        assert(left == long_a);
        assert(right == long_b);
    }

    {
        LOG_DURATION("with string_view substr");
        pos = sv.find('=');

        string_view sleft = sv.substr(0, pos);
        string_view sright = sv.substr(pos + 1); // warning with no = char or empty string

        assert(sleft == sv_long_a);
        assert(sright == sv_long_b);
    }

    cout << "all asserts are OK\n";
}

void TestSearchSpeed() {
    const int half_size = 5e7;

    // Generate long string aaa...=bbb...
    string s(half_size, 'a');
    s += '=';
    s += string(half_size, 'b');

    string_view sv(s); // for other test

    cout << "Test speed for '=' char search in long string aaa...=bbb..." << endl;

    int pos = 0;
    {
        LOG_DURATION("find with exec::seq");

        auto it_pos = find(execution::seq, s.begin(), s.end(), '=');
        pos = it_pos - s.begin();
        assert(pos == half_size);
    }
    {
        LOG_DURATION("find with exec::par");

        auto it_pos = find(execution::par, s.begin(), s.end(), '=');
        pos = it_pos - s.begin();
        assert(pos == half_size);
    }
    {
        LOG_DURATION("find with string::find method");

        pos = s.find('=');
        assert(pos == half_size);
    }

    cout << "\nNow change string on string_view\n";

    {
        LOG_DURATION("find with exec::seq");

        auto it_pos = find(execution::seq, sv.begin(), sv.end(), '=');
        pos = it_pos - sv.begin();
        assert(pos == half_size);
    }
    {
        LOG_DURATION("find with exec::par");

        auto it_pos = find(execution::par, sv.begin(), sv.end(), '=');
        pos = it_pos - sv.begin();
        assert(pos == half_size);
    }
    {
        LOG_DURATION("find with string::find method");

        pos = sv.find('=');
        assert(pos == half_size);
    }

    cout << "all asserts are OK\n";
    cout << "char '=' is on "s << pos << " position\n";
}

// Test speed for '=' char search in long string aaa...=bbb...
// find with exec::seq operation time: 462 ms
// find with exec::par operation time: 153 ms
// find with string::find method operation time: 3 ms

// Now change string on string_view
// find with exec::seq operation time: 385 ms
// find with exec::par operation time: 110 ms
// find with string::find method operation time: 3 ms
// all asserts are OK
// char '=' is on 50000000 position
// with string::substr operation time: 59 ms
// with begin()-end() operation time: 66 ms
// with string_view substr operation time: 13 ms
// all asserts are OK
// count chars operation time: 234 ms
// count chars move operation time: 249 ms
