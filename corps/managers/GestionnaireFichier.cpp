#include "GestionnaireFichier.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>
#include <cstring>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/types.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

GestionnaireFichier::GestionnaireFichier(std::string dossier)
    : m_dossierSauvegarde(std::move(dossier)) {
    initialiserDossier();
}

bool GestionnaireFichier::initialiserDossier() {
    struct stat info {};
    memset(&info, 0, sizeof(info));

    if (stat(m_dossierSauvegarde.c_str(), &info) != 0) {
        if (MKDIR(m_dossierSauvegarde.c_str()) == 0) {
            std::cout << "📁 Dossier créé: " << m_dossierSauvegarde << std::endl;
            return true;
        } else {
            std::cerr << "❌ Erreur création dossier: " << m_dossierSauvegarde << std::endl;
            return false;
        }
    } else if (info.st_mode & S_IFDIR) {
        return true;
    } else {
        std::cerr << "❌ Le chemin n'est pas un dossier: " << m_dossierSauvegarde << std::endl;
        return false;
    }
}

bool GestionnaireFichier::sauvegarderDonnees(const std::string& nomFichier, const std::string& donnees) const {
    std::string cheminComplet = getCheminComplet(nomFichier + ".csv");

    try {
        std::ofstream fichier(cheminComplet);
        if (!fichier.is_open()) {
            std::cerr << "❌ Impossible d'ouvrir: " << cheminComplet << std::endl;
            return false;
        }

        fichier << donnees;
        fichier.close();

        std::cout << "💾 CSV sauvegardé: " << cheminComplet << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "❌ Erreur sauvegarde: " << e.what() << std::endl;
        return false;
    }
}

std::string GestionnaireFichier::chargerDonnees(const std::string& nomFichier) const {
    std::string cheminComplet = getCheminComplet(nomFichier + ".csv");

    if (!fichierExiste(cheminComplet)) {
        std::cerr << "❌ Fichier non trouvé: " << cheminComplet << std::endl;
        return "";
    }

    try {
        std::ifstream fichier(cheminComplet);
        if (!fichier.is_open()) {
            std::cerr << "❌ Impossible d'ouvrir: " << cheminComplet << std::endl;
            return "";
        }

        std::string contenu((std::istreambuf_iterator<char>(fichier)),
                            std::istreambuf_iterator<char>());
        fichier.close();

        std::cout << "📂 CSV chargé: " << cheminComplet << std::endl;
        return contenu;

    } catch (const std::exception& e) {
        std::cerr << "❌ Erreur chargement: " << e.what() << std::endl;
        return "";
    }
}

bool GestionnaireFichier::sauvegarderTable(const std::string& nomTable, Database& db) const {
    std::stringstream donnees;
    donnees << "id;nom;prenom;dateNaissance;sexe;contact;idMedecinTraitant" << std::endl;

    auto rows = db.select("SELECT * FROM " + nomTable);
    for (const auto& row : rows) {
        for (size_t i = 0; i < row.values.size(); ++i) {
            donnees << row.values[i];
            if (i < row.values.size() - 1) donnees << ";";
        }
        donnees << std::endl;
    }

    return sauvegarderDonnees("table_" + nomTable, donnees.str());
}

bool GestionnaireFichier::sauvegardeComplete(Database& db, const std::string& nomBackup) const {
    if (!db.backup(nomBackup + ".db")) return false;

    sauvegarderTable("patients", db);
    sauvegarderTable("professionnels", db);
    sauvegarderTable("consultations", db);
    sauvegarderTable("antecedents", db);

    return true;
}

bool GestionnaireFichier::restaurerTable(const std::string& nomFichier, Database& db) const {
    std::string donnees = chargerDonnees(nomFichier);
    if (donnees.empty()) {
        return false;
    }

    std::stringstream ss(donnees);
    std::string ligne;
    std::getline(ss, ligne); // Header

    db.beginTransaction();
    while (std::getline(ss, ligne)) {
        std::vector<std::string> champs;
        std::stringstream ligneSs(ligne);
        std::string champ;
        while (std::getline(ligneSs, champ, ';')) {
            champs.push_back(champ);
        }
        db.executeWithParams("INSERT INTO patients (id, nom, prenom, dateNaissance, sexe, contact, idMedecinTraitant) VALUES (?, ?, ?, ?, ?, ?, ?)", champs);
    }
    db.commitTransaction();

    return true;
}

std::vector<std::string> GestionnaireFichier::listerSauvegardes() const {
    std::vector<std::string> sauvegardes;

    DIR* dir = opendir(m_dossierSauvegarde.c_str());
    if (dir == nullptr) {
        std::cerr << "❌ Impossible d'ouvrir le dossier: " << m_dossierSauvegarde << std::endl;
        return sauvegardes;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string nomFichier = entry->d_name;
        if (nomFichier.length() > 4 &&
            nomFichier.substr(nomFichier.length() - 4) == ".csv") {
            sauvegardes.push_back(nomFichier);
        }
    }

    closedir(dir);
    return sauvegardes;
}

bool GestionnaireFichier::supprimerSauvegarde(const std::string& nomFichier) const {
    std::string cheminComplet = getCheminComplet(nomFichier);

    if (std::remove(cheminComplet.c_str()) == 0) {
        std::cout << "🗑️ Fichier supprimé: " << cheminComplet << std::endl;
        return true;
    } else {
        std::cerr << "❌ Erreur suppression: " << cheminComplet << std::endl;
        return false;
    }
}

std::string GestionnaireFichier::getCheminComplet(const std::string& nomFichier) const {
    return m_dossierSauvegarde + "/" + nomFichier;
}

bool GestionnaireFichier::fichierExiste(const std::string& chemin) {
    struct stat info {};
    memset(&info, 0, sizeof(info));
    return (stat(chemin.c_str(), &info) == 0);
}
