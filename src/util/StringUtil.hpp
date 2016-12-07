#ifndef STRING_UTIL_HPP
#define STRING_UTIL_HPP

#define STR_TOUPPER(str) std::transform(str.begin(), str.end(),str.begin(), ::toupper);
#define STR_TOLOWER(str) std::transform(str.begin(), str.end(),str.begin(), ::tolower);
#define STR_STARTSWITH(haystack, needle) (strncmp(haystack.c_str(), needle.c_str(), needle.size()) == 0)

/**
* Fonction pour décoder une URL (%20 -> ' ' ...)
* http://stackoverflow.com/questions/154536/encode-decode-urls-in-c
*/
string urlDecode(string &SRC) {
    string ret;
    char ch;
    int i, ii;
    for (i=0; i<SRC.length(); i++) {
        if (int(SRC[i])==37) {
            sscanf(SRC.substr(i+1,2).c_str(), "%x", &ii);
            ch=static_cast<char>(ii);
            ret+=ch;
            i=i+2;
        } else {
            ret+=SRC[i];
        }
    }
    return (ret);
}

#endif
