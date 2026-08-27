#ifndef ISCHINESE_H
#define ISCHINESE_H
#include<bits/stdc++.h>
    int containsChinese(const std::string& str) {
    for (size_t i = 0; i < str.length(); ) {
        unsigned char c = str[i];
        if (c >= 0xE4 && c <= 0xE9 && i + 2 < str.length()) {
            unsigned char c2 = str[i + 1];
            unsigned char c3 = str[i + 2];
            if ((c2 >= 0x80 && c2 <= 0xBF) && (c3 >= 0x80 && c3 <= 0xBF)) {
                return 1;
            }
            i += 3;
        } else if (c < 0x80) {
            i += 1;
        } else {
            i += (c >= 0xC0 && c <= 0xDF) ? 2 :
                 (c >= 0xE0 && c <= 0xEF) ? 3 : 4;
        }
    }
    return 0;
}

// 检测是否全部是中文（UTF-8）
int isAllChinese(const std::string& str) {
    if (str.empty()) return 1;
    for (size_t i = 0; i < str.length(); ) {
        unsigned char c = str[i];
        if (c >= 0xE4 && c <= 0xE9 && i + 2 < str.length()) {
            unsigned char c2 = str[i + 1];
            unsigned char c3 = str[i + 2];
            if ((c2 >= 0x80 && c2 <= 0xBF) && (c3 >= 0x80 && c3 <= 0xBF)) {
                i += 3;
                continue;
            }
        }
        return 0;
    }
    return 1;
}

#endif