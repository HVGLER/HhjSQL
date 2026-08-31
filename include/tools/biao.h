#ifndef BIAO_H
#define BIAO_H

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <bits/stdc++.h>
#include "tools/cjiandbzd.h"
#include "tools/aes.h"

#define AES_BLOCKLEN 16

// ============ PKCS7 填充 ============

void pkcs7_pad(uint8_t *data, size_t *len, size_t block_size) {
    size_t pad_len = block_size - (*len % block_size);
    if (pad_len == 0) pad_len = block_size;
    
    for (size_t i = 0; i < pad_len; i++) {
        data[*len + i] = (uint8_t)pad_len;
    }
    *len += pad_len;
}

int pkcs7_unpad(uint8_t *data, size_t *len) {
    if (*len == 0) return -1;
    
    uint8_t pad_len = data[*len - 1];
    if (pad_len == 0 || pad_len > 16) return -1;
    
    for (size_t i = *len - pad_len; i < *len; i++) {
        if (data[i] != pad_len) return -1;
    }
    
    *len -= pad_len;
    return 0;
}

// ============ AES 加密/解密 ============

int aes_cbc_encrypt(const uint8_t *key, const uint8_t *iv,
                    const uint8_t *plaintext, size_t plaintext_len,
                    uint8_t *ciphertext, size_t *ciphertext_len) {
    memcpy(ciphertext, plaintext, plaintext_len);
    size_t padded_len = plaintext_len;
    
    pkcs7_pad(ciphertext, &padded_len, AES_BLOCKLEN);
    
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_encrypt_buffer(&ctx, ciphertext, padded_len);
    
    *ciphertext_len = padded_len;
    return 0;
}

int aes_cbc_decrypt(const uint8_t *key, const uint8_t *iv,
                    const uint8_t *ciphertext, size_t ciphertext_len,
                    uint8_t *plaintext, size_t *plaintext_len) {
    if (ciphertext_len % AES_BLOCKLEN != 0) {
        return -1;
    }
    
    memcpy(plaintext, ciphertext, ciphertext_len);
    
    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_CBC_decrypt_buffer(&ctx, plaintext, ciphertext_len);
    
    size_t padded_len = ciphertext_len;
    if (pkcs7_unpad(plaintext, &padded_len) != 0) {
        return -1;
    }
    
    *plaintext_len = padded_len;
    return 0;
}

// ============ 辅助函数 ============

void generate_random_iv(uint8_t *iv, size_t size) {
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned)time(nullptr));
        seeded = true;
    }
    for (size_t i = 0; i < size; i++) {
        iv[i] = rand() & 0xFF;
    }
}

void print_hex(const uint8_t *data, size_t len, const char *label) {
    printf("%s: ", label);
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

// ============ 数据库操作 ============

int createTable(const std::string& tableName) {
    dbcjzd(tableName);
    
    std::string aaaa = std::string("./data/") + tableName;
    std::ofstream file(aaaa);
    if (file.is_open()) {
        file << "# Table: " << tableName << std::endl;
        file.close();
        return 0;
    }
    else {
        return -1;
    }
}

/**
 * 创建表内容（加密保存）
 */
int createbiao_neirong(const std::string& tablename, 
                       const std::string& name, 
                       const std::string& type, 
                       const std::string& neirong) {
    // 1. 构建数据
    std::string jjgg = "名称：" + name + "|类型：" + type + "|内容：" + neirong;
    const char* message = jjgg.c_str();
    size_t plaintext_len = strlen(message);
    
    // 2. 准备密钥和IV
    uint8_t key[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    
    uint8_t iv[16];
    generate_random_iv(iv, sizeof(iv));
    printf("加密 IV: ");
    print_hex(iv, sizeof(iv), "");
    
    // 3. 分配缓冲区
    size_t max_cipher_len = plaintext_len + AES_BLOCKLEN;
    uint8_t *ciphertext = (uint8_t*)malloc(max_cipher_len);
    size_t ciphertext_len;
    
    if (!ciphertext) {
        printf("内存分配失败\n");
        return -1;
    }
    
    // 4. 加密
    if (aes_cbc_encrypt(key, iv, (uint8_t*)message, plaintext_len,
                        ciphertext, &ciphertext_len) == 0) {
        printf("✅ 加密成功，密文长度: %zu\n", ciphertext_len);
    } else {
        printf("❌ 加密失败\n");
        free(ciphertext);
        return -1;
    }
    
    // 5. 保存到文件（格式：IV(16) + 长度(4) + 密文）
    std::string aaaa = std::string("./data/") + tablename;
    std::ofstream file(aaaa, std::ios::binary | std::ios::app);
    
    if (file.is_open()) {
        // 写入 IV (16字节)
        file.write((char*)iv, sizeof(iv));
        
        // 写入密文长度 (4字节)
        uint32_t len = (uint32_t)ciphertext_len;
        file.write((char*)&len, sizeof(len));
        
        // 写入密文
        file.write((char*)ciphertext, ciphertext_len);
        
        file.close();
        free(ciphertext);
        printf("✅ 数据已保存到: %s\n", aaaa.c_str());
        return 0;
    } else {
        free(ciphertext);
        printf("❌ 无法打开文件: %s\n", aaaa.c_str());
        return -1;
    }
}

/**
 * 读取表（自动解密）
 */
int readTable(const std::string& tablename) {
    std::string aaaa = std::string("./data/") + tablename;
    std::ifstream file(aaaa, std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "❌ 无法打开文件: " << aaaa << std::endl;
        return -1;
    }
    
    // 准备密钥
    uint8_t key[16] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    
    std::cout << "\n========== " << tablename << " 的内容 ==========" << std::endl;
    
    // 检查文件头
    char first_char;
    file.read(&first_char, 1);
    file.seekg(0);
    
    if (first_char == '#') {
        // 新格式：跳过文件头
        std::string header;
        std::getline(file, header);
        std::cout << "文件头: " << header << std::endl;
    }
    
    int count = 0;
    while (file.peek() != EOF) {
        uint8_t iv[16];
        uint32_t ciphertext_len;
        
        // 1. 读取 IV (16字节)
        file.read((char*)iv, sizeof(iv));
        if (file.eof() || file.gcount() != sizeof(iv)) {
            break;
        }
        
        // 2. 读取密文长度 (4字节)
        file.read((char*)&ciphertext_len, sizeof(ciphertext_len));
        if (file.eof() || file.gcount() != sizeof(ciphertext_len)) {
            break;
        }
        
        // 检查长度是否合理
        if (ciphertext_len == 0 || ciphertext_len > 1024 * 1024) {  // 最大1MB
            std::cerr << "⚠️ 无效的数据长度: " << ciphertext_len << std::endl;
            break;
        }
        
        // 3. 读取密文
        uint8_t *ciphertext = (uint8_t*)malloc(ciphertext_len);
        if (!ciphertext) {
            std::cerr << "❌ 内存分配失败" << std::endl;
            break;
        }
        
        file.read((char*)ciphertext, ciphertext_len);
        if (file.gcount() != ciphertext_len) {
            std::cerr << "❌ 读取数据失败" << std::endl;
            free(ciphertext);
            break;
        }
        
        // 4. 解密
        uint8_t *plaintext = (uint8_t*)malloc(ciphertext_len);
        size_t plaintext_len;
        
        if (aes_cbc_decrypt(key, iv, ciphertext, ciphertext_len,
                            plaintext, &plaintext_len) == 0) {
            count++;
            std::string result((char*)plaintext, plaintext_len);
            std::cout << count << ". " << result << std::endl;
        } else {
            std::cerr << "❌ 解密失败 (记录 " << count + 1 << ")" << std::endl;
            // 打印前32字节用于调试
            std::cout << "   密文前32字节: ";
            for (size_t i = 0; i < std::min<size_t>(32, ciphertext_len); i++) {
                printf("%02x ", ciphertext[i]);
            }
            std::cout << std::endl;
        }
        
        free(ciphertext);
        free(plaintext);
    }
    
    if (count == 0) {
        std::cout << "  (空)" << std::endl;
    }
    
    file.close();
    std::cout << "========================================" << std::endl;
    return 0;
}

#endif
