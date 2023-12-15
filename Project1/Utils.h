#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <fstream>
char generateRandDigit() {
    srand(time(0));
    return ((rand() % 10) + '0');
}
bool isVowel(char ch) {
    switch (ch) {
    case 97:
        return 1;
        break;
    case 101:
        return 1;
        break;
    case 105:
        return 1;
    case 111:
        return 1;
        break;
    case 117:
        return 1;
        break;
    default:
        return 0;

    }
}
std::string string_to_lower(std::string& given) {
    std::string lower = given;
    for (short i = 0; i < given.size(); i++) {
        lower[i] = tolower(given[i]);
    }
    return lower;
}
std::string removeDigits(std::string str) {
    std::string result = "";
    for (int i = 0; i < str.length(); i++) {
        if (isdigit(str[i])) {
            continue;
        }
        result += str[i];
    }

    return result;
}
std::string ReadFromFile(std::string FileName) {
    std::ifstream file(FileName);
	std::string str;
	std::string result;
    while (std::getline(file, str)) {
		result += str;
	}
	file.close();
	return result;
}
std::string FetchMACAddress() {
    system("GetMAC.exe");
    std::string MACAddress = ReadFromFile("mac_address.txt");
    return MACAddress;
}
bool Authenticate(std::string Client, std::vector<std::string> ListOfUsers) {
    for (int i = 0; i < ListOfUsers.size(); i++) {
        if (Client == ListOfUsers[i]) {
			return 1;
		}
	}
	return 0;
}