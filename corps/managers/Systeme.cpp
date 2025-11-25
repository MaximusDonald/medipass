#include "Systeme.h"
#include <algorithm>

Systeme* Systeme::instance = nullptr;

Systeme::Systeme() {}

Systeme& Systeme::getInstance() {
    if (!instance) {
        instance = new Systeme();
    }
    return *instance;
}

void Systeme::ajouterPatient(const Patient& p) {
    patients.push_back(std::make_unique<Patient>(p));
}

Patient* Systeme::trouverPatient(const std::string& id) {
    auto it = std::find_if(patients.begin(), patients.end(), [&id](const auto& p) { return p->get_id() == id; });
    return (it != patients.end()) ? it->get() : nullptr;
}

const std::vector<std::unique_ptr<Patient>>& Systeme::getPatients() const {
    return patients;
}

void Systeme::supprimerPatient(const std::string& id) {
    patients.erase(std::remove_if(patients.begin(), patients.end(), [&id](const auto& p) { return p->get_id() == id; }), patients.end());
}

void Systeme::ajouterProfessionnel(const ProfessionnelSante& p) {
    professionnels.push_back(std::make_unique<ProfessionnelSante>(p));
}

ProfessionnelSante* Systeme::trouverProfessionnel(const std::string& id) {
    auto it = std::find_if(professionnels.begin(), professionnels.end(), [&id](const auto& p) { return p->get_id() == id; });
    return (it != professionnels.end()) ? it->get() : nullptr;
}

const std::vector<std::unique_ptr<ProfessionnelSante>>& Systeme::getProfessionnels() const {
    return professionnels;
}

void Systeme::ajouterConsultation(const Consultation& c) {
    consultations.push_back(c);
}

std::vector<Consultation> Systeme::getConsultations() const {
    return consultations;
}

void Systeme::ajouterAntecedent(const Antecedent& a) {
    antecedents.push_back(a);
}

std::vector<Antecedent> Systeme::getAntecedents() const {
    return antecedents;
}

int Systeme::nombrePatients() const {
    return patients.size();
}

int Systeme::nombreProfessionnelsParSpecialite(const std::string& specialite) const {
    return std::count_if(professionnels.begin(), professionnels.end(), [&specialite](const auto& p) { return p->get_specialite() == specialite; });
}

int Systeme::nombreConsultationsParPeriode(const std::string& debut, const std::string& fin) const {
    return std::count_if(consultations.begin(), consultations.end(), [&debut, &fin](const Consultation& c) {
        return c.get_date() >= debut && c.get_date() <= fin;
    });
}
