#include "DossierMedical.h"
#include <iostream>
#include <algorithm>

DossierMedical::DossierMedical()
{
}

DossierMedical::DossierMedical(const DossierMedical& other)
    : consultations(other.consultations), antecedents(other.antecedents)
{
}

DossierMedical::~DossierMedical()
{
}

const std::vector<Consultation>& DossierMedical::getConsultations() const { return consultations; }
const std::vector<Antecedent>&   DossierMedical::getAntecedents() const   { return antecedents; }

void DossierMedical::ajouterConsultation(const Consultation& c)
{
    consultations.push_back(c);
}

void DossierMedical::ajouterAntecedent(const Antecedent& a)
{
    antecedents.push_back(a);
}

void DossierMedical::afficher() const
{
    std::cout << "===== DOSSIER MEDICAL =====\n";
    std::cout << "Consultations : " << consultations.size() << "\n";
    std::cout << "Antécédents   : " << antecedents.size() << "\n";
    std::cout << "==============================\n";
}

void DossierMedical::afficherHistoriqueComplet() const
{
    afficher();
    std::cout << "\n--- Antécédents ---\n";
    for (const auto& a : antecedents) {
        a.afficher();
    }
    std::cout << "\n--- Consultations (chronologique) ---\n";
    for (const auto& c : consultations) {
        c.afficher_details();
    }
}

void DossierMedical::supprimerConsultation(const std::string& idConsultation)
{
    consultations.erase(
        std::remove_if(consultations.begin(), consultations.end(),
            [&idConsultation](const Consultation& c) { return c.get_consultationId() == idConsultation; }),
        consultations.end()
    );
}

void DossierMedical::supprimerAntecedent(const std::string& idAntecedent)
{
    antecedents.erase(
        std::remove_if(antecedents.begin(), antecedents.end(),
            [&idAntecedent](const Antecedent& a) { return a.get_id() == idAntecedent; }),
        antecedents.end()
    );
}

Consultation* DossierMedical::rechercherConsultation(const std::string& idConsultation)
{
    auto it = std::find_if(consultations.begin(), consultations.end(),
        [&idConsultation](const Consultation& c) { return c.get_consultationId() == idConsultation; });
    return (it != consultations.end()) ? &(*it) : nullptr;
}

Antecedent* DossierMedical::rechercherAntecedent(const std::string& idAntecedent)
{
    auto it = std::find_if(antecedents.begin(), antecedents.end(),
        [&idAntecedent](const Antecedent& a) { return a.get_id() == idAntecedent; });
    return (it != antecedents.end()) ? &(*it) : nullptr;
}

int DossierMedical::nombreConsultations() const
{
    return static_cast<int>(consultations.size());
}

int DossierMedical::nombreAntecedentsGraves() const
{
    return std::count_if(antecedents.begin(), antecedents.end(),
        [](const Antecedent& a) { return a.get_gravite() >= 3; });
}

std::vector<Consultation> DossierMedical::getConsultationsParPeriode(const std::string& debut, const std::string& fin) const
{
    std::vector<Consultation> resultat;
    for (const auto& c : consultations) {
        std::string date = c.get_date();
        if (date >= debut && date <= fin) {
            resultat.push_back(c);
        }
    }
    return resultat;
}

std::vector<Consultation> DossierMedical::getConsultationsParProfessionnel(const std::string& idProf) const
{
    std::vector<Consultation> resultat;
    for (const auto& c : consultations) {
        if (c.get_idProfessionnel() == idProf) {
            resultat.push_back(c);
        }
    }
    return resultat;
}
