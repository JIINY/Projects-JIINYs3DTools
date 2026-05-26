#pragma once
#include <string>


#define utf8(text) reinterpret_cast<const char*>(u8##text) //컴파일타임 리터럴(u8"...")을 const char*로 캐스팅

std::string toUTF8(const std::string& str); //런타임 std::string을 받아서 filesystem::path를 거쳐 변환
