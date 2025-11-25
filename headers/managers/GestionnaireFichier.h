#ifndef GESTIONNAIREFICHIER_H
#define GESTIONNAIREFICHIER_H

#include <string>
#include <vector>
#include "Database.h"

class GestionnaireFichier {
private:
    std::string m_dossierSauvegarde;

    std::string getCheminComplet(const std::string& nomFichier) const;
    static bool fichierExiste(const std::string& chemin);

public:
    GestionnaireFichier(std::string dossier);
    bool initialiserDossier();

    bool sauvegarderDonnees(const std::string& nomFichier, const std::string& donnees) const;
    std::string chargerDonnees(const std::string& nomFichier) const;

    bool sauvegarderTable(const std::string& nomTable, Database& db) const;
    bool sauvegardeComplete(Database& db, const std::string& nomBackup) const;

    bool restaurerTable(const std::string& nomFichier, Database& db) const;

    std::vector<std::string> listerSauvegardes() const;
    bool supprimerSauvegarde(const std::string& nomFichier) const;
};

#endif // GESTIONNAIREFICHIER_H
