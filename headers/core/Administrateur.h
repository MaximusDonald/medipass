#ifndef ADMINISTRATEUR_H
#define ADMINISTRATEUR_H

#include <string>
#include "Utilisateur.h"


class Administrateur : public Utilisateur
{
    public:

        // Constructeurs
        Administrateur(const Administrateur& other);

        Administrateur(std::string n, std::string p, std::string l, std::string mdp);

        // Destructeurs
        ~Administrateur();

        // Autres méthodes
        std::string getTypeString() override;
};

#endif // ADMINISTRATEUR_H
