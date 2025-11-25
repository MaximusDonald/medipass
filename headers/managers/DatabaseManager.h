#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "Database.h"
#include "GestionnaireFichier.h"
#include "Logger.h"
#include <string>

class DatabaseManager {
private:
    Database m_database;
    GestionnaireFichier m_gestionnaireFichier;
    Logger m_logger;
    std::string m_nomBase;
    std::string m_dossierSauvegarde;

public:
    DatabaseManager(const std::string& nomBase, const std::string& dossierSauvegarde);
    ~DatabaseManager();

    bool initialiserSysteme();
    bool creerTables();

    bool sauvegarderAvecLog(const std::string& nomSauvegarde);
    bool restaurerAvecLog(const std::string& nomSauvegarde);

    void logAction(const std::string& action, bool reussite);
    void afficherEtat() const;
    void afficherStatistiques() const;

    bool estOperationnel() const;

    // Méthodes dynamiques pour Medipass
    bool ajouterPatient(const std::vector<std::string>& params);
    std::vector<Row> listerPatients();
    bool supprimerPatient(const std::string& id);
    // Ajoute similaires pour consultations, antecedents, professionnels si besoin
};

#endif // DATABASEMANAGER_H
