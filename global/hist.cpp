#include "hist.hpp"



Hist::Hist(std::vector<std::vector<double>> datas, std::vector<std::string> labels, std::vector<std::string> clusters ):
settings(),m_data(datas), m_labels(labels), m_clusters(clusters)
{

}

/*
datas : tableau des valeur
n: nombre de cluster pour la discretisation 
*/
Hist::Hist(std::vector<double> datas,unsigned int n): m_data()  // ce constructeur est pour les histograme dans un espace dense
{
    srand(time(NULL));
    if(n == 0)
        std::cout << " choix dynamique de cluster non implementé veuiller choisir un nombre de batons pour l'histogram" << std::endl;

    if(n> datas.size())
    {
        std::cout<< " plus de cluster que de datas : impossible "<<std::endl;
    } 


    // k-means
    std::vector<std::vector<double>> result; // vas stoquer les diferentes valeur de datas seulons les different cluster
    std::vector<double> means(n) ; // les moyennes pour chaque cluster
    // step 1 choix des n valeur aleatoirement 
    std::vector<int>  alreadyUsed;
    for(unsigned int i = 0 ; i< n ; i++)
    {
        int ind;
        do{
            ind = rand() % datas.size();
            // je me dit que trié pour selectionner les quantile d'odre n ca pourrai etre pas mal 

        }while( std::find(alreadyUsed.begin(),alreadyUsed.end(), ind) != alreadyUsed.end());
        
        alreadyUsed.push_back(ind);
        means[i]=datas[ind];
        result.push_back(std::vector<double>());
        
    }

    // debut de boucle
    
    for(unsigned i =0; i< 20; i++)
    {
        std::cout<< i << std::endl;
        // on vide les données pour bien repartir 
        for(unsigned int k=0; k<result.size(); k++) //result.size() == n
        {
            if(result[i].size() != 0)
                result[i].clear();
        }
        std::cout << "fin clear" << std::endl;
        // step 2 associer chaque data a un des cluster avec l'ecart au diferente moyen 
        for(unsigned int k=0; k< datas.size(); k++)
        {
            double best = std::abs(datas[k]-means[0]);
            unsigned int bestind = 0;
            std::cout << "data :" << k << std::endl; 
            for(unsigned int m =1 ; m< means.size(); m++) // means.size() == n aussi
            {
                double tmp = std::abs(datas[k]- means[m]);
                if(tmp < best )
                {
                    best = tmp;
                    bestind = m;
                }
            }
            std::cout<< "avant push_back a l'indice " << bestind << " la valeur de n est " << n << std::endl;
            result[bestind].push_back(datas[k]); // ce truc plante des foi, ca n'a aucun sens 
            std::cout << "pushback fait" << std::endl; 
        }
        std::cout<< "fin step 2" << std::endl;

        // step 3 recalculer les moyennes de chaque classe 
        for(unsigned int i= 0 ; i< means.size(); i++)
        {
            
            double acc=0;
            for(unsigned k =0; k< result[i].size(); k++)
            {
                acc += result[i][k];
            }
            means[i] = acc/((double)(result[i].size()));
        }
        std::cout<< "fin step 3" << std::endl;
    }

    std::vector<double> y(n);
    // unsigned int total = datas.size();
    for(unsigned int i=0; i<result.size(); i++)
    {
        // j'utilise des double ,meme si en theorie il faudrai des int, car je vais au final le ranger dans un tableau de double 
       
        y[i]= (double)(result[i].size());
        // total+=result[i].size();
    }
    std::cout << "fin creation y" << std::endl;
    // dans ce cas le premier vecteur de m_data sera les x et le deuxieme les y 
    m_data.push_back(means); // ca c'est les x 
    m_data.push_back(y);
    settings.followXAxe = true;
    std::cout << "fin " << std::endl;

}