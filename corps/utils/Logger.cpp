#include "../../headers/utils/Logger.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Logger::Logger(const std::string& nomFichier)
    : m_nomFichier(nomFichier), m_actif(true) {
    m_fichierLog.open(m_nomFichier, std::ios::app);
    if (m_fichierLog.is_open()) {
        static bool premier = true;
        if (premier) {
            ecrire("=== DÉMARRAGE MEDIPASS - " + getHorodatage() + " ===");
            premier = false;
        }
    }
}

Logger::~Logger() {
    if (m_fichierLog.is_open()) {
        ecrire("=== ARRÊT DU SYSTÈME ===");
        m_fichierLog.close();
    }
}

void Logger::activer(bool actif) { m_actif = actif; }

void Logger::ecrire(const std::string& message) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_fichierLog.is_open()) {
        m_fichierLog << message << std::endl;
        m_fichierLog.flush();
    }
}

std::string Logger::getHorodatage() const {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string Logger::niveauToString(NiveauLog niveau) const {
    switch (niveau) {
        case NiveauLog::INFO: return "INFO";
        case NiveauLog::AVERTISSEMENT: return "AVERT";
        case NiveauLog::ERREUR: return "ERREUR";
        case NiveauLog::CRITIQUE: return "CRITIQUE";
        case NiveauLog::SECURITE: return "SÉCURITÉ";
        default: return "INCONNU";
    }
}

void Logger::log(NiveauLog niveau, const std::string& message,
                 const std::string& utilisateur, const std::string& cible) {
    if (!m_actif) return;

    std::stringstream ss;
    ss << "[" << getHorodatage() << "] ";
    ss << "[" << niveauToString(niveau) << "] ";
    if (!utilisateur.empty()) ss << utilisateur << " → ";
    ss << message;
    if (!cible.empty()) ss << " | Cible: " << cible;

    ecrire(ss.str());

    if (niveau == NiveauLog::ERREUR || niveau == NiveauLog::CRITIQUE || niveau == NiveauLog::SECURITE) {
        std::cerr << "LOG: " << ss.str() << std::endl;
    }
}

void Logger::info(const std::string& msg, const std::string& user, const std::string& cible) {
    log(NiveauLog::INFO, msg, user, cible);
}

void Logger::avertissement(const std::string& msg, const std::string& user, const std::string& cible) {
    log(NiveauLog::AVERTISSEMENT, msg, user, cible);
}

void Logger::erreur(const std::string& msg, const std::string& user, const std::string& cible) {
    log(NiveauLog::ERREUR, msg, user, cible);
}

void Logger::critique(const std::string& msg, const std::string& user, const std::string& cible) {
    log(NiveauLog::CRITIQUE, msg, user, cible);
}

void Logger::securite(const std::string& msg, const std::string& user, const std::string& cible) {
    log(NiveauLog::SECURITE, msg, user, cible);
}
