#include <iostream>
#include <string> 
#include <vector> 
#include <sstream> 
#include <fstream> 
#include <cstdlib> 
#include <filesystem> 
#include <glm/glm.hpp>
#include "../global/render.hpp"

class GnuRender: public virtual Render
{

public:
    GnuRender();
    GnuRender(std::string t);
    //GnuRender(GlobalSetting s);

    ~GnuRender();
    // void draw(std::vector<double> x, std::vector<double> y); // a enlever 
    // void draw(GlobalSetting)
    void draw();
    void draw(GlobalSetting s);


    // void drawLine(std::vector<double> x, std::vector<double> y, std::string name , glm::vec3 color ); // a enlever
    void drawLine(const Line &l);
    void drawLine(const Line &l, GlobalSetting s);

    void drawHist(const Hist &h);
    void drawHist(const Hist &h, GlobalSetting s);
    
    void addLine(const Line &l);
    void addHist(const Hist &l);

    void clear();

protected: 
    void addHistFollowX(const Hist &h);
    void addHistOther(const Hist &h);
    void makeGlobalSettings(GlobalSetting s);

    FILE* gnucmd;
    // std::string m_GNUPlotFileName;
    // std::string m_GNUPlotPath;
    std::string create_tmpfile(std::ofstream &tmp);  

    std::vector<std::string> memory; //mettre la dedans toutes les string de plot (sans le "plot") comme ca si on appel deux foi draw on peut reaficher les ancien draw 



};

   



/*
note: cmd => 
    fputs( (cmdstr+"\n").c_str(), gnucmd );
    fflush(gnucmd); 
std::filesystem pour gestion de fichier 
*/