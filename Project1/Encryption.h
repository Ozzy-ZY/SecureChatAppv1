#pragma once
#include "utils.h"


std::string Encrypt(std::string str, int right_Shift, int left_shift) {
    std::string encrypted;
    encrypted = string_to_lower(str);

    for (int i = 0; i < encrypted.length(); i += 2) {
        if (isalpha(encrypted[i])) {
            encrypted[i] = (encrypted[i] - ASCII_LOWER_A + right_Shift) % ALPHABET_SIZE + ASCII_LOWER_A;
        }
    }

    for (int i = 1; i < encrypted.length(); i += 2) {
        if (isalpha(encrypted[i])) {
            encrypted[i] = (encrypted[i] - ASCII_LOWER_A - left_shift + ALPHABET_SIZE) % ALPHABET_SIZE + ASCII_LOWER_A;
        }
    }

    std::reverse(encrypted.begin(), encrypted.end());
    return encrypted;
}
