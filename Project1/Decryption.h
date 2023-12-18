#pragma once
#include "utils.h"

std::string Decrypt(std::string str, int right_Shift, int left_shift) {
    std::string decrypted;
    decrypted = str;  // Start with the encrypted string

    // Reverse the string to its original order
    std::reverse(decrypted.begin(), decrypted.end());

    for (int i = 1; i < decrypted.length(); i += 2) {
        if (isalpha(decrypted[i])) {
            // Reverse the left shift operation
            decrypted[i] = (decrypted[i] - ASCII_LOWER_A + left_shift) % ALPHABET_SIZE + ASCII_LOWER_A;
        }
    }

    for (int i = 0; i < decrypted.length(); i += 2) {
        if (isalpha(decrypted[i])) {
            // Reverse the right shift operation
            decrypted[i] = (decrypted[i] - ASCII_LOWER_A - right_Shift + ALPHABET_SIZE) % ALPHABET_SIZE + ASCII_LOWER_A;
        }
    }

    return decrypted;
}
