#ifndef STRING_UTIL_HPP
#define STRING_UTIL_HPP

#define STR_TOUPPER(str) std::transform(str.begin(), str.end(),str.begin(), ::toupper);
#define STR_TOLOWER(str) std::transform(str.begin(), str.end(),str.begin(), ::tolower);

#endif
