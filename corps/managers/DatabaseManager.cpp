#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager(const std::string& nomBase,
                                 const std::string& dossierSauvegarde)
    : m_database(nomBase), m_gestionnaireFichier(dossierSauvegarde),
      m_logger("medipass.log"), m_nomBase(nomBase), m_dossierSauvegarde(dossierSauvegarde) {
    m_logger.info("DatabaseManager créé - Base: " + m_nomBase);
}

DatabaseManager::~DatabaseManager() {
    m_logger.info("DatabaseManager fermé");
    m_database.close();
}

bool DatabaseManager::initialiserSysteme() {
    m_logger.info("=== INITIALISATION SYSTÈME MEDIPASS ===");

    if (!m_database.open()) {
        m_logger.erreur("Échec ouverture base de données: " + m_nomBase);
        return false;
    }
    m_logger.info("Base de données ouverte: " + m_nomBase);

    if (!m_gestionnaireFichier.initialiserDossier()) {
        m_logger.avertissement("Problème dossier sauvegarde");
    } else {
        m_logger.info("Dossier sauvegarde prêt: " + m_dossierSauvegarde);
    }

    if (!creerTables()) {
        m_logger.erreur("Échec création tables");
        return false;
    }

    m_logger.info("=== SYSTÈME INITIALISÉ AVEC SUCCÈS ===");
    return true;
}

bool DatabaseManager::creerTables() {
    m_logger.info("Création des tables Medipass...");

    std::string sqlPatients = "CREATE TABLE IF NOT EXISTS patients ("
                              "id TEXT PRIMARY KEY, "
                              "nom TEXT NOT NULL, "
                              "prenom TEXT NOT NULL, "
                              "dateNaissance TEXT, "
                              "sexe TEXT, "
                              "contact TEXT, "
                              "idMedecinTraitant TEXT"
                              ");";

    if (!m_database.execute(sqlPatients)) {
        m_logger.erreur("Échec création table patients");
        return false;
    }

    std::string sqlProfessionnels = "CREATE TABLE IF NOT EXISTS professionnels ("
                                    "id TEXT PRIMARY KEY, "
                                    "nom TEXT NOT NULL, "
                                    "prenom TEXT NOT NULL, "
                                    "login TEXT, "
                                    "motDePasse TEXT, "
                                    "specialite TEXT, "
                                    "service TEXT"
                                    ");";

    if (!m_database.execute(sqlProfessionnels)) {
        m_logger.erreur("Échec création table professionnels");
        return false;
    }

    std::string sqlConsultations = "CREATE TABLE IF NOT EXISTS consultations ("
                                   "consultationId TEXT PRIMARY KEY, "
                                   "date TEXT, "
                                   "idPatient TEXT, "
                                   "idProfessionnel TEXT, "
                                   "nomProfessionnel TEXT, "
                                   "motif TEXT, "
                                   "observations TEXT"
                                   ");";

    if (!m_database.execute(sqlConsultations)) {
        m_logger.erreur("Échec création table consultations");
        return false;
    }

    std::string sqlAntecedents = "CREATE TABLE IF NOT EXISTS antecedents ("
                                 "id TEXT PRIMARY KEY, "
                                 "type TEXT, "
                                 "date TEXT, "
                                 "description TEXT, "
                                 "gravite INTEGER"
                                 ");";

    if (!m_database.execute(sqlAntecedents)) {
        m_logger.erreur("Échec création table antecedents");
        return false;
    }

    m_logger.info("Tables Medipass créées avec succès");
    return true;
}

bool DatabaseManager::sauvegarderAvecLog(const std::string& nomSauvegarde) {
    m_logger.info("Début sauvegarde: " + nomSauvegarde);

    if (m_gestionnaireFichier.sauvegardeComplete(m_database, nomSauvegarde)) {
        m_logger.info("Sauvegarde réussie: " + nomSauvegarde);
        return true;
    } else {
        m_logger.erreur("Échec sauvegarde: " + nomSauvegarde);
        return false;
    }
}

bool DatabaseManager::restaurerAvecLog(const std::string& nomSauvegarde) {
    m_logger.info("Tentative restauration: " + nomSauvegarde);

    if (m_gestionnaireFichier.restaurerTable(nomSauvegarde, m_database)) {
        m_logger.info("Restauration réussie: " + nomSauvegarde);
        return true;
    } else {
        m_logger.erreur("Échec restauration: " + nomSauvegarde);
        return false;
    }
}

void DatabaseManager::logAction(const std::string& action, bool reussite) {
    if (reussite) {
        m_logger.info("ACTION: " + action);
    } else {
        m_logger.erreur("ACTION ÉCHOUÉE: " + action);
    }
}

void DatabaseManager::afficherEtat() const {
    std::cout << "\n=== ÉTAT DU SYSTÈME MEDIPASS ===" << std::endl;
    std::cout << "📊 Base de données: " << (m_database.isOpen() ? "✅ Connectée" : "❌ Déconnectée") << std::endl;
    std::cout << "📁 Dossier sauvegarde: " << m_dossierSauvegarde << std::endl;
    std::cout << "📄 Fichier log: medipass.log" << std::endl;
    std::cout << "🎯 Système: " << (estOperationnel() ? "✅ Opérationnel" : "❌ Problème") << std::endl;
}

void DatabaseManager::afficherStatistiques() const {
    std::cout << "\n=== STATISTIQUES MEDIPASS ===" << std::endl;
    std::cout << "🗄️  Base: " << m_nomBase << std::endl;

    auto rows = m_database.select("SELECT COUNT(*) FROM patients");
    std::cout << "👥 Patients: " << (rows.empty() ? "0" : rows[0].values[0]) << std::endl;

    rows = m_database.select("SELECT COUNT(*) FROM consultations");
    std::cout << "📅 Consultations: " << (rows.empty() ? "0" : rows[0].values[0]) << std::endl;

    auto sauvegardes = m_gestionnaireFichier.listerSauvegardes();
    std::cout << "💾 Sauvegardes disponibles: " << sauvegardes.size() << std::endl;
    for (const auto& sauvegarde : sauvegardes) {
        std::cout << "   - " << sauvegarde << std::endl;
    }
}

bool DatabaseManager::estOperationnel() const {
    return m_database.isOpen() && m_logger.estActif();
}

bool DatabaseManager::ajouterPatient(const std::vector<std::string>& params) {
    bool success = m_database.executeWithParams("INSERT INTO patients (id, nom, prenom, dateNaissance, sexe, contact, idMedecinTraitant) VALUES (?, ?, ?, ?, ?, ?, ?)", params);
    logAction("Ajout patient", success);
    return success;
}

std::vector<Row> DatabaseManager::listerPatients() {
    return m_database.select("SELECT * FROM patients");
}

bool DatabaseManager::supprimerPatient(const std::string& id) {
    std::vector<std::string> params = {id};
    bool success = m_database.executeWithParams("DELETE FROM patients WHERE id = ?", params);
    logAction("Suppression patient " + id, success);
    return success;
}
