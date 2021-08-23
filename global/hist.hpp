#pragma once 

#include "settings.hpp" 
#include <time.h>

class Hist
{
public:
    Hist(std::vector<std::vector<double>> datas, std::vector<std::string> labels, std::vector<std::string> clusters = std::vector<std::string>());
    // Hist(std::vector<int> datas);
    // Hist(std::vector<double> datas, std::vector<std::string> labels);
    Hist(std::vector<double> datas,unsigned int n=0);// ce constructeur est pour les histograme dans un espace dense, n'as pas de cluster ni de label 



    HistSettings settings; 

    std::vector<std::vector<double> > m_data; //les datas ou chaque vecteur est un cluster et chaque element corestpond a un des label
    std::vector<std::string> m_labels; // les labels qui sont les nom de chaque elmeent 
    std::vector<std::string> m_clusters; // nom des cluster  



};