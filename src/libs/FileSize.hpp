/* Modifié depuis : http://memoirsofacoder.blogspot.fr/2014/08/c-get-file-size-in-kbmbgb-format.html */

#ifndef FILESIZE_HPP
#define FILESIZE_HPP

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// Utility functions:

string convertToString(double num) {
    ostringstream convert;
    convert << num;
    return convert.str();
}

double roundOff(double n) {
    double d = n * 100.0;
    int i = d + 0.5;
    d = (float)i / 100.0;
    return d;
}

string convertSize(size_t size) {
    static const char *SIZES[] = { "o", "Ko", "Mo", "Go" };
    int div = 0;
    size_t rem = 0;

    while (size >= 1000 && div < (sizeof SIZES / sizeof *SIZES)) {
        rem = (size % 1000);
        div++;
        size /= 1000;
    }

    double size_d = (float)size + (float)rem / 1000.0;
    string result = convertToString(roundOff(size_d)) + SIZES[div];
    return result;
}

#endif
