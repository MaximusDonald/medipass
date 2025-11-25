#ifndef CSVMANAGER_H
#define CSVMANAGER_H

#include <string>
#include <vector>
#include "Outils.h"

class CsvManager {
public:
    static bool exporterPatients(const std::string& fichier = "patients.csv");
    static bool exporterProfessionnels(const std::string& fichier = "professionnels.csv");
    static bool exporterConsultations(const std::string& fichier = "consultations.csv");
    static bool exporterAntecedents(const std::string& fichier = "antecedents.csv");

    static bool importerPatients(const std::string& fichier = "patients.csv");
    static bool importerProfessionnels(const std::string& fichier = "professionnels.csv");
    static bool importerConsultations(const std::string& fichier = "consultations.csv");
    static bool importerAntecedents(const std::string& fichier = "antecedents.csv");

    static bool exporterTout();
    static bool importerTout();

private:
    static std::vector<std::string> splitCsv(const std::string& ligne);
    static std::string escapeCsv(const std::string& s);
};

#endif // CSVMANAGER_H
