#ifndef DOSSIERMEDICAL_H
#define DOSSIERMEDICAL_H

#include <vector>
#include <string>
#include "Consultation.h"
#include "Antecedent.h"


class DossierMedical
{
    public:

        DossierMedical();

        DossierMedical(const DossierMedical& other);


        ~DossierMedical();


        const std::vector<Consultation>& getConsultations() const;
        const std::vector<Antecedent>&   getAntecedents() const;


        void ajouterConsultation(const Consultation& c);
        void ajouterAntecedent(const Antecedent& a);


        void afficher() const;


        void afficherHistoriqueComplet() const;


        void supprimerConsultation(const std::string& idConsultation);
        void supprimerAntecedent(const std::string& idAntecedent);


        Consultation* rechercherConsultation(const std::string& idConsultation);
        Antecedent*   rechercherAntecedent(const std::string& idAntecedent);


        int nombreConsultations() const;
        int nombreAntecedentsGraves() const;


        std::vector<Consultation> getConsultationsParPeriode(const std::string& debut, const std::string& fin) const;
        std::vector<Consultation> getConsultationsParProfessionnel(const std::string& idProf) const;

    private:
        std::vector<Consultation> consultations;
        std::vector<Antecedent>   antecedents;
};

#endif // DOSSIERMEDICAL_H
