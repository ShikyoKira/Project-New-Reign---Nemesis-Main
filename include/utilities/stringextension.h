#pragma once

#include "utilities/types.h"

// case sensitive. Use StringStartWith_NC for case insensitive function
void StringReplace(std::string& _ch, const std::string& oldvalue, const std::string& newvalue);

// case insensitive. Use StringStartWith for case sensitive function
void StringReplace_NC(std::string& _ch, const std::string& oldvalue, const std::string& newvalue);

void StringSplit(const std::string& line, VecStr& container);
void StringSplit(const std::string& line, VecStr& container, char delimiter);
void StringSplit(const std::string& line, VecStr& container, const std::string& delimiters);

VecStr StringSplit(const std::string& line);
VecStr StringSplit(const std::string& line, char delimiter);
VecStr StringSplit(const std::string& line, const std::string& delimiters);

// case sensitive. Use StringStartWith_NC for case insensitive function
bool StringStartWith(const std::string& line, const std::string& start) noexcept;

// case sensitive. Use StringEndWith_NC for case insensitive function
bool StringEndWith(const std::string& line, const std::string& end) noexcept;

// case insensitive. Use StringStartWith for case sensitive function
bool StringStartWith_NC(const std::string& line, const std::string& start) noexcept;

// case insensitive. Use StringEndWith for case sensitive function
bool StringEndWith_NC(const std::string& line, const std::string& end) noexcept;
