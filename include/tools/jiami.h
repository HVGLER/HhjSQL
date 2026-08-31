#ifndef JIAMI_H
#define JIAMI_H
#include<bits/stdc++.h>
// ============================================
// 超暴力移位加密（管你什么编码，全是字节）
// ============================================
class BrutalCipher {
private:
    int shift;
    
public:
    BrutalCipher(int s) : shift(s % 256) {}
    
    // 加密：对每个字节暴力移位
    std::string encrypt(const std::string& text) {
        std::string result = text;
        for(size_t i = 0; i < result.size(); i++) {
            unsigned char c = result[i];
            // 暴力移位 + 位置扰动
            c = (c + shift + i * 7) % 256;
            result[i] = c;
        }
        return result;
    }
    
    // 解密：反向操作
    std::string decrypt(const std::string& text) {
        std::string result = text;
        for(size_t i = 0; i < result.size(); i++) {
            unsigned char c = result[i];
            c = (c - shift - i * 7 + 25600) % 256; // 加个大数防负数
            result[i] = c;
        }
        return result;
    }
};

// ============================================
// 疯狂混淆引擎（简单粗暴版）
// ============================================
class CrazyObfuscator {
private:
    // 随机数生成器
    int getJunk() {
        static std::mt19937 rng(
            std::chrono::steady_clock::now().time_since_epoch().count()
        );
        static std::uniform_int_distribution<int> dist(0, 255);
        return dist(rng);
    }
    
    // 垃圾函数（啥也不干，就是浪费CPU）
    void wasteTime(int n = 10) {
        volatile int x = 0;
        for(int i = 0; i < n; i++) {
            x += getJunk() % 7;
            x ^= 0x55;
            x &= 0xFF;
            x = (x << 1) | (x >> 31);
        }
    }
    
    // 假函数1
    void fake1() {
        volatile int arr[10];
        for(int i = 0; i < 10; i++) {
            arr[i] = getJunk() ^ i;
            wasteTime(5);
        }
    }
    
    // 假函数2
    void fake2() {
        volatile long long x = 0x123456789ABCDEF0LL;
        for(int i = 0; i < 20; i++) {
            x ^= (x << 5) | (x >> 59);
            x += getJunk();
            wasteTime(3);
        }
    }
    
    // 假函数3
    void fake3() {
        volatile double d = 3.14159;
        for(int i = 0; i < 15; i++) {
            d = d * 1.1 + 0.5;
            if((int)d % 3 == 0) wasteTime(7);
        }
    }

public:
    // ============================================
    // 混淆加密
    // ============================================
    std::string crazyEncrypt(const std::string& plain, int key) {
        // 垃圾初始化
        volatile int junk1 = getJunk();
        volatile int junk2 = getJunk() ^ 0xAA;
        volatile int junk3 = junk1 + junk2;
        
        // 调用假函数
        fake1();
        fake2();
        
        // 伪造控制流
        int fakeSwitch = (key ^ junk1) % 5;
        switch(fakeSwitch) {
            case 0: goto REAL_ENCRYPT;
            case 1: goto FAKE_PATH1;
            case 2: goto FAKE_PATH2;
            case 3: goto REAL_ENCRYPT;
            case 4: goto FAKE_PATH3;
        }
        
    FAKE_PATH1:
        fake1();
        fake3();
        goto REAL_ENCRYPT;
        
    FAKE_PATH2:
        fake2();
        wasteTime(20);
        goto REAL_ENCRYPT;
        
    FAKE_PATH3:
        fake3();
        wasteTime(15);
        // fall through
        
    REAL_ENCRYPT:
        // 计算真实移位量（用复杂方式）
        int shift = (key & 0xFF) ^ ((key >> 8) & 0xFF);
        shift = (shift + ((key >> 16) & 0xFF)) % 256;
        shift = (shift ^ ((key >> 24) & 0xFF)) % 256;
        
        // 创建加密器（用指针间接调用）
        BrutalCipher* cipher = new BrutalCipher(shift);
        
        // 用lambda再包一层
        auto encryptor = [&](const std::string& s) -> std::string {
            wasteTime(5);
            fake1();
            return cipher->encrypt(s);
        };
        
        std::string result = encryptor(plain);
        delete cipher;
        
        // 后处理：假装做了额外处理
        for(size_t i = 0; i < result.size(); i += 2) {
            if(i + 1 < result.size()) {
                std::swap(result[i], result[i+1]);
                std::swap(result[i], result[i+1]); // 换回来，纯粹浪费时间
            }
        }
        
        wasteTime(10);
        fake2();
        
        return result;
    }
    
    // ============================================
    // 混淆解密
    // ============================================
    std::string crazyDecrypt(const std::string& cipher, int key) {
        // 垃圾初始化
        volatile char junk[50];
        for(int i = 0; i < 50; i++) {
            junk[i] = getJunk() % 256;
        }
        
        // 假循环
        for(int i = 0; i < 3; i++) {
            volatile int x = getJunk() * i + 7;
            fake3();
            wasteTime(8);
        }
        
        // 计算移位量（和加密保持一致）
        int shift = (key & 0xFF) ^ ((key >> 8) & 0xFF);
        shift = (shift + ((key >> 16) & 0xFF)) % 256;
        shift = (shift ^ ((key >> 24) & 0xFF)) % 256;
        
        // 创建解密器
        BrutalCipher* cipherObj = new BrutalCipher(shift);
        
        // lambda间接调用
        auto decryptor = [&](const std::string& s) -> std::string {
            wasteTime(5);
            fake2();
            return cipherObj->decrypt(s);
        };
        
        std::string result = decryptor(cipher);
        delete cipherObj;
        
        // 后处理
        for(size_t i = 0; i < result.size(); i += 3) {
            if(i + 2 < result.size()) {
                // 假装交换，其实又换回来
                char tmp = result[i];
                result[i] = result[i+2];
                result[i+2] = tmp;
                result[i+2] = result[i];
                result[i] = tmp;
            }
        }
        
        wasteTime(10);
        fake1();
        fake3();
        
        return result;
    }
};
#endif