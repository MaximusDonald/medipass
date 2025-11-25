#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <chrono>
#include <mutex>

enum class NiveauLog { INFO, AVERTISSEMENT, ERREUR, CRITIQUE, SECURITE };

class Logger {
private:
    std::ofstream m_fichierLog;
    std::string m_nomFichier;
    bool m_actif;
    std::mutex m_mutex;

    void ecrire(const std::string& message);
    std::string getHorodatage() const;
    std::string niveauToString(NiveauLog niveau) const;

public:
    explicit Logger(const std::string& nomFichier = "systeme_hospitalier.log");
    ~Logger();

    void activer(bool actif = true);
    bool estActif() const { return m_actif; }

    void log(NiveauLog niveau, const std::string& message,
             const std::string& utilisateur = "", const std::string& cible = "");

    void info(const std::string& msg, const std::string& user = "", const std::string& cible = "");
    void avertissement(const std::string& msg, const std::string& user = "", const std::string& cible = "");
    void erreur(const std::string& msg, const std::string& user = "", const std::string& cible = "");
    void critique(const std::string& msg, const std::string& user = "", const std::string& cible = "");
    void securite(const std::string& msg, const std::string& user, const std::string& cible);
};

#endif // LOGGER_H
