#ifndef SYSTEME_H
#define SYSTEME_H

#include "Patient.h"
#include "ProfessionnelSante.h"
#include "Consultation.h"
#include "Antecedent.h"
#include <vector>
#include <memory>

class Systeme {
private:
    static Systeme* instance;
    Systeme();

    std::vector<std::unique_ptr<Patient>> patients;
    std::vector<std::unique_ptr<ProfessionnelSante>> professionnels;
    std::vector<Consultation> consultations;
    std::vector<Antecedent> antecedents;

public:
    static Systeme& getInstance();

    // Patients
    void ajouterPatient(const Patient& p);
    Patient* trouverPatient(const std::string& id);
    const std::vector<std::unique_ptr<Patient>>& getPatients() const;
    void supprimerPatient(const std::string& id);

    // Professionnels
    void ajouterProfessionnel(const ProfessionnelSante& p);
    ProfessionnelSante* trouverProfessionnel(const std::string& id);
    const std::vector<std::unique_ptr<ProfessionnelSante>>& getProfessionnels() const;

    // Consultations
    void ajouterConsultation(const Consultation& c);
    std::vector<Consultation> getConsultations() const;

    // Antécédents
    void ajouterAntecedent(const Antecedent& a);
    std::vector<Antecedent> getAntecedents() const;

    // Statistiques
    int nombrePatients() const;
    int nombreProfessionnelsParSpecialite(const std::string& specialite) const;
    int nombreConsultationsParPeriode(const std::string& debut, const std::string& fin) const;
};

#endif // SYSTEME_H
