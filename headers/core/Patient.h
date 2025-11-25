#ifndef PATIENT_H
#define PATIENT_H

#include <string>
#include "DossierMedical.h"


class Patient
{
    public:

        Patient();

        Patient(const Patient& other);

        Patient(std::string nom, std::string prenom,
                std::string dateNaiss, std::string sexe,
                std::string contact, std::string idMedecinTraitant = "");

        Patient(std::string id, std::string nom, std::string prenom,
                std::string dateNaiss, std::string sexe,
                std::string contact, std::string idMedecinTraitant = "");


        ~Patient();


        std::string get_id() const;
        std::string get_nom() const;
        std::string get_prenom() const;
        std::string get_nomComplet() const;
        std::string get_dateNaissance() const;
        std::string get_sexe() const;
        std::string get_contact() const;
        std::string get_idMedecinTraitant() const;


        const DossierMedical& get_dossier() const;
        DossierMedical&       get_dossierModifiable();


        void set_id(const std::string & i);
        void set_nom(const std::string& n);
        void set_prenom(const std::string& p);
        bool set_dateNaissance(const std::string& date);  // retourne false si invalide
        void set_sexe(const std::string& s);
        void set_contact(const std::string& c);
        void set_idMedecinTraitant(const std::string& idProf);


        void afficherInfos() const;
        void afficherComplet() const;


        static std::string genererId();

        static bool validerDateNaissance(const std::string& date);

    private:
        static int compteurId;

        std::string id;
        std::string nom;
        std::string prenom;
        std::string dateNaissance;       // Format strict : AAAA-MM-JJ
        std::string sexe;
        std::string contact;
        std::string idMedecinTraitant;
        DossierMedical dossier;
};

#endif // PATIENT_H
