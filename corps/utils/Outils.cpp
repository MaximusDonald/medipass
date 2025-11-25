#include "Outils.h"
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

namespace Outils {

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void pause() {
        cout << "\nAppuyez sur Entrée pour continuer...";
        cin.ignore(10000, '\n');
        cin.get();
    }

    void setColor(Color c) {
        #ifdef _WIN32
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
        #endif
    }

    void resetColor() {
        setColor(DEFAULT);
    }

    string getCurrentDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[20];
        sprintf(buffer, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
        return string(buffer);
    }

    string getCurrentDateFR() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        char buffer[20];
        sprintf(buffer, "%02d/%02d/%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
        return string(buffer);
    }


    void printHeader(const string& title, Color c) {
        clearScreen();
        setColor(c);
        cout << "====================================================" << endl;
        cout << "               MEDIPASS - " << title << endl;
        cout << "====================================================" << endl << endl;
        resetColor();
    }

    void printError(const string& msg) {
        setColor(RED);
        cout << "ERREUR : " << msg << endl;
        resetColor();
    }

    void printSuccess(const string& msg) {
        setColor(GREEN);
        cout << "SUCCÈS : " << msg << endl;
        resetColor();
    }

    string readLine() {
        string input;
        getline(cin, input);
        return input;
    }

    string readNonEmpty(const string& prompt) {
        string input;
        do {
            cout << prompt;
            getline(cin, input);
            input = trim(input);
            if (input.empty()) {
                printError("Ce champ ne peut pas être vide.");
            }
        } while (input.empty());
        return input;
    }

    bool confirm(const string& question) {
        cout << question << " (o/n) : ";
        string rep;
        getline(cin, rep);
        return (rep == "o" || rep == "O" || rep == "oui" || rep == "OUI");
    }

    string trim(const string& s) {
        size_t debut = s.find_first_not_of(" \t\r\n");
        if (debut == string::npos) return "";
        size_t fin = s.find_last_not_of(" \t\r\n");
        return s.substr(debut, fin - debut + 1);
    }

    string toUpper(const string& s) {
        string res = s;
        transform(res.begin(), res.end(), res.begin(), ::toupper);
        return res;
    }
}
