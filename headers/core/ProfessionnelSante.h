#ifndef PROFESSIONNELSANTE_H
#define PROFESSIONNELSANTE_H
#include "Utilisateur.h"
#include <string>

class ProfessionnelSante : public Utilisateur
{
    public:
        // Constructeurs
        ProfessionnelSante();
        ProfessionnelSante(const ProfessionnelSante& other);
        ProfessionnelSante(std::string n, std::string p, std::string l, std::string mdp, std::string spec, std::string serv);

        // Destructeur
        ~ProfessionnelSante();

        // Accesseurs
        std::string getTypeString() override;
        std::string get_specialite() const;
        std::string get_service() const;

        // Mutateurs
        void set_specialite(std::string val);
        void set_service(std::string val);
    protected:
    private:
        std::string specialite;
        std::string service;
};
#endif // PROFESSIONNELSANTE_H
