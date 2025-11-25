#ifndef OUTILS_H
#define OUTILS_H

#include <string>
#include <iostream>
#include <ctime>
#include <cstdio>
#ifdef _WIN32
#include <windows.h>
#endif

namespace Outils {

    enum Color { DEFAULT = 7, BLUE = 9, GREEN = 10, CYAN = 11, RED = 12, YELLOW = 14, WHITE = 15 };

    void clearScreen();
    void pause();
    void setColor(Color c);
    void resetColor();

    std::string getCurrentDate();        // AAAA-MM-JJ
    std::string getCurrentDateFR();      // JJ/MM/AAAA (affichage)

    void printHeader(const std::string& title, Color c = BLUE);
    void printError(const std::string& msg);
    void printSuccess(const std::string& msg);

    std::string readLine();
    std::string readNonEmpty(const std::string& prompt);
    bool confirm(const std::string& question);

    std::string trim(const std::string& s);
    std::string toUpper(const std::string& s);
}

#endif // OUTILS_H
