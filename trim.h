// Author: Benjamin Vogel
#pragma once

#include <string>

/**
 * A helper method to trim the left side of the string of whitespace characters
 * @param s: The string to trim the whitespace off the left side
 * @returns A left-trimmed string
 **/
std::string ltrim(const std::string& s);

/**
 * A helper method to trim the right side of the string of whitespace characters
 * @param s: The string to trim the whitespace off the right side
 * @returns A right-trimmed string
 **/
std::string rtrim(const std::string& s);

/**
 * A helper method that trims the edges of a string of trailing whitespace
 * @param s: The string to trim
 * @returns A trimmed string
 **/
std::string trim(const std::string& s);