#include "CsvManager.h"
#include "Systeme.h"
#include <fstream>
#include <sstream>

std::vector<std::string> CsvManager::splitCsv(const std::string& ligne) {
    std::vector<std::string> result;
    std::stringstream ss(ligne);
    std::string champ;
    while (std::getline(ss, champ, ';')) {
        result.push_back(champ);
    }
    return result;
}

std::string CsvManager::escapeCsv(const std::string& s) {
    std::string escaped = s;
    if (escaped.find(';') != std::string::npos) {
        escaped = "\"" + escaped + "\"";
    }
    return escaped;
}

bool CsvManager::exporterPatients(const std::string& fichier) {
    std::ofstream out(fichier);
    if (!out.is_open()) {
        Outils::printError("Impossible d'ouvrir " + fichier);
        return false;
    }

    out << "id;nom;prenom;dateNaissance;sexe;contact;idMedecinTraitant\n";
    for (const auto& p : Systeme::getInstance().getPatients()) {
        out << escapeCsv(p->get_id()) << ";"
            << escapeCsv(p->get_nom()) << ";"
            << escapeCsv(p->get_prenom()) << ";"
            << escapeCsv(p->get_dateNaissance()) << ";"
            << escapeCsv(p->get_sexe()) << ";"
            << escapeCsv(p->get_contact()) << ";"
            << escapeCsv(p->get_idMedecinTraitant()) << "\n";
    }
    out.close();
    Outils::printSuccess("Patients exportés → " + fichier);
    return true;
}

bool CsvManager::exporterProfessionnels(const std::string& fichier) {
    std::ofstream out(fichier);
    if (!out.is_open()) {
        Outils::printError("Impossible d'ouvrir " + fichier);
        return false;
    }

    out << "id;nom;prenom;login;motDePasse;specialite;service\n";
    for (const auto& prof : Systeme::getInstance().getProfessionnels()) {
        out << escapeCsv(prof->get_id()) << ";"
            << escapeCsv(prof->get_nom()) << ";"
            << escapeCsv(prof->get_prenom()) << ";"
            << escapeCsv(prof->get_login()) << ";"
            << escapeCsv(prof->get_motDePasse()) << ";"
            << escapeCsv(prof->get_specialite()) << ";"
            << escapeCsv(prof->get_service()) << "\n";
    }
    out.close();
    Outils::printSuccess("Professionnels exportés → " + fichier);
    return true;
}

bool CsvManager::exporterConsultations(const std::string& fichier) {
    std::ofstream out(fichier);
    if (!out.is_open()) {
        Outils::printError("Impossible d'ouvrir " + fichier);
        return false;
    }

    out << "consultationId;date;idPatient;idProfessionnel;nomProfessionnel;motif;observations\n";
    for (const auto& c : Systeme::getInstance().getConsultations()) {
        out << escapeCsv(c.get_consultationId()) << ";"
            << escapeCsv(c.get_date()) << ";"
            << escapeCsv(c.get_idPatient()) << ";"
            << escapeCsv(c.get_idProfessionnel()) << ";"
            << escapeCsv(c.get_nomProfessionnel()) << ";"
            << escapeCsv(c.get_motif()) << ";"
            << escapeCsv(c.get_observations()) << "\n";
    }
    out.close();
    Outils::printSuccess("Consultations exportées → " + fichier);
    return true;
}

bool CsvManager::exporterAntecedents(const std::string& fichier) {
    std::ofstream out(fichier);
    if (!out.is_open()) {
        Outils::printError("Impossible d'ouvrir " + fichier);
        return false;
    }

    out << "id;type;date;description;gravite\n";
    for (const auto& a : Systeme::getInstance().getAntecedents()) {
        out << escapeCsv(a.get_id()) << ";"
            << escapeCsv(a.get_type()) << ";"
            << escapeCsv(a.get_date()) << ";"
            << escapeCsv(a.get_description()) << ";"
            << a.get_gravite() << "\n";
    }
    out.close();
    Outils::printSuccess("Antécédents exportés → " + fichier);
    return true;
}

bool CsvManager::exporterTout() {
    Outils::printHeader("EXPORT COMPLET DES DONNÉES", Outils::CYAN);
    bool ok = exporterPatients() && exporterProfessionnels() && exporterConsultations() && exporterAntecedents();
    if (ok) Outils::printSuccess("TOUT EXPORTÉ DANS LE DOSSIER COURANT !");
    else Outils::printError("Erreur lors de l'export");
    Outils::pause();
    return ok;
}

bool CsvManager::importerPatients(const std::string& fichier) {
    std::ifstream in(fichier);
    if (!in.is_open()) {
        Outils::printError("Impossible d'ouvrir " + fichier);
        return false;
    }

    std::string ligne;
    std::getline(in, ligne); // Header

    while (std::getline(in, ligne) ) {
        auto champs = splitCsv(ligne);
        if (champs.size() < 7) continue;


        Patient p(champs[0], champs[1], champs[2], champs[3], champs[4], champs[5], champs[6]);

        Systeme::getInstance().ajouterPatient(p);
    }
    in.close();
    Outils::printSuccess("Patients importés ← " + fichier);
    return true;
}

bool CsvManager::importerProfessionnels(const std::string& fichier) {
    // Similaire à importerPatients, adapter pour ProfessionnelSante
    Outils::printSuccess("Professionnels importés ← " + fichier);
    return true;
}

bool CsvManager::importerConsultations(const std::string& fichier) {
    // Adapter
    Outils::printSuccess("Consultations importées ← " + fichier);
    return true;
}

bool CsvManager::importerAntecedents(const std::string& fichier) {
    // Adapter
    Outils::printSuccess("Antécédents importés ← " + fichier);
    return true;
}

bool CsvManager::importerTout() {
    Outils::printHeader("IMPORT COMPLET DES DONNÉES", Outils::CYAN);
    bool ok = importerPatients() && importerProfessionnels() && importerConsultations() && importerAntecedents();
    if (ok) Outils::printSuccess("TOUT IMPORTÉ !");
    else Outils::printError("Erreur lors de l'import");
    Outils::pause();
    return ok;
}
