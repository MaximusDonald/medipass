#include "Administrateur.h"


Administrateur::Administrateur(const Administrateur& other)
    : Utilisateur(other)
{
}

Administrateur::Administrateur(std::string n, std::string p, std::string l, std::string mdp)
    : Utilisateur(n, p, l, mdp, ADMINISTRATEUR)
{
}

Administrateur::~Administrateur()
{
}

std::string Administrateur::getTypeString()
{
    return "Administrateur";
}
