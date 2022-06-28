#ifndef CUTIL_H
#define CUTIL_H

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int isSpace(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int isPrintable(int c) {
    return c >= ' ' && c <= '~';
}

int isDigit(int c) {
    return c >= '0' && c <= '9';
}

int isAlpha(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isAlphaNum(int c) {
    return isAlpha(c) || isDigit(c);
}

int isHex(int c) {
    return isDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

int isOctal(int c) {
    return c >= '0' && c <= '7';
}

int isBinary(int c) {
    return c == '0' || c == '1';
}

int isControl(int c) {
    return c < ' ' || c == 0x7F;
}

int isASCII(int c) {
    return c >= 0 && c <= 0x7F;
}

// Source: https://gist.github.com/rafaelglikis/ee7275bf80956a5308af5accb4871135
/**
 * Cross-platform sleep function for C
 * @param int milliseconds
 */
void sleep_ms(int milliseconds)
{
    #ifdef WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

#endif
