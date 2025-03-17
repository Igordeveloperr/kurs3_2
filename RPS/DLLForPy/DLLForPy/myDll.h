#pragma once
#include <string>
#include <regex>

#ifdef FILESTATLIBCPP_EXPORTS
#define FILESTATLIBCPP_API __declspec(dllexport)
#else
#define FILESTATLIBCPP_API __declspec(dllimport)
#endif

extern "C" FILESTATLIBCPP_API int number_of_words(const char*);