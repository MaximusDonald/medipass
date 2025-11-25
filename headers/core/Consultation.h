#ifndef CONSULTATION_H
#define CONSULTATION_H

#include <vector>
#include <string>
#include <map>
#include "Outils.h"


class Consultation
{
    public:

        Consultation();

        Consultation(const Consultation& other);


        Consultation(std::string pId, std::string profId, std::string profNom,
                    std::string m, std::string obs);


        ~Consultation();


        std::string get_consultationId() const;
        std::string get_date() const;
        std::string get_idPatient() const;
        std::string get_idProfessionnel() const;
        std::string get_nomProfessionnel() const;
        std::string get_motif() const;
        std::string get_observations() const;


        void set_consultationId(std::string val);
        void set_date(std::string val);
        void set_idPatient(std::string val);
        void set_idProfessionnel(std::string val);
        void set_nomProfessionnel(std::string val);
        void set_motif(std::string val);
        void set_observations(std::string val);


        void afficher_details() const;

        static std::string genererId(const std::string& idPatient);


    private:
        std::string consultationId;
        std::string date;
        std::string idPatient;
        std::string idProfessionnel;
        std::string nomProfessionnel;
        std::string motif;
        std::string observations;
        static int compteurParPatient;
        static std::map<std::string, int> compteursPatients;
};

#endif // CONSULTATION_H
