#include "Antecedent.h"
#include <iostream>
#include <iomanip>
#include <sstream>

int Antecedent::compteurId = 0;

std::string Antecedent::genererId()
{
    ++compteurId;
    std::stringstream ss;
    ss << "ANT-" << std::setfill('0') << std::setw(5) << compteurId;
    return ss.str();
}

Antecedent::Antecedent()
    : id(genererId()), type(""), date(Outils::getCurrentDate()), description(""), gravite(1)
{
}

Antecedent::Antecedent(const Antecedent& other)
    : id(other.id), type(other.type), date(other.date),
      description(other.description), gravite(other.gravite)
{
}

Antecedent::Antecedent(std::string type, std::string description, int gravite)
    : id(genererId()), type(type), description(description), gravite(gravite)
{
    date = Outils::getCurrentDate();
}


Antecedent::~Antecedent()
{
}

std::string Antecedent::get_id() const          { return id; }
std::string Antecedent::get_type() const        { return type; }
std::string Antecedent::get_date() const        { return date; }
std::string Antecedent::get_description() const { return description; }
int         Antecedent::get_gravite() const     { return gravite; }

void Antecedent::set_type(const std::string& val)       { type = val; }
void Antecedent::set_description(const std::string& val){ description = val; }
void Antecedent::set_gravite(int val)
{
    if (val >= 1 && val <= 4) gravite = val;
}

std::string Antecedent::get_graviteLibelle() const
{
    switch (gravite) {
        case 1: return "Légère";
        case 2: return "Modérée";
        case 3: return "Sévère";
        case 4: return "Critique";
        default: return "Inconnue";
    }
}

void Antecedent::afficher() const
{
    std::cout << "Antécédent [" << id << "]\n";
    std::cout << "  Type      : " << type << "\n";
    std::cout << "  Date      : " << date << "\n";
    std::cout << "  Gravité   : " << gravite << " - " << get_graviteLibelle() << "\n";
    std::cout << "  Description : " << description << "\n";
    std::cout << "----------------------------------------\n";
}
