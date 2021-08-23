#include "gnuRender.hpp"
namespace fs = std::filesystem;

std::string next(std::string s)
{
    int size = s.size();
    bool retenu = false; 
    int i= size-1;
    do{
        if(i == -1){
            return s+"a";
        }
        else{
            s[i] += 1;
            
            if(s[i] > 'z'){
                s[i] = 'a';
                retenu = true;
            }else{
                retenu = false;
            }
        }
        i--;
    }while(retenu);
    return s;

}

GnuRender::GnuRender()
{
    //std::cout << "construction de l'object GnuRender" << std::endl;
    #if ( defined(unix) || defined(__unix) || defined(__unix__) ) && !defined(__APPLE__)
    if (getenv("DISPLAY") == NULL)
    {
        
        std::cout << "Can't find DISPLAY variable" << std::endl;

    }
    #endif

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    std::string m_GNUPlotFileName = "pgnuplot.exe";
    std::string m_GNUPlotPath = "C:/program files/gnuplot/bin/";
    #elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    std::string m_GNUPlotFileName = "gnuplot-x11";
    std::string m_GNUPlotPath = "/usr/bin/"; // depend de l'ordi , il faut chercher l'emplacement dynamiquement 
    #endif


    std::string tmp = m_GNUPlotPath  +  m_GNUPlotFileName; // ce fichier peu ne pas exister cf ligne 1764 de gnuplot_i.hpp


    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    gnucmd = _popen(tmp.c_str(),"w");
    #elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    gnucmd = popen(tmp.c_str(),"w");
    #endif
    

    if (!gnucmd)
    {
        std::cout << "Couldn't open connection to gnuplot" << std::endl;
    }
    //fputs("set output \n", gnucmd);
    //fflush(gnucmd);
    fputs("set terminal x11 \n", gnucmd);
    fflush(gnucmd);

}

GnuRender::GnuRender(std::string t)
{

    #if ( defined(unix) || defined(__unix) || defined(__unix__) ) && !defined(__APPLE__)
    if (getenv("DISPLAY") == NULL)
    {
        
        std::cout << "Can't find DISPLAY variable" << std::endl;

    }
    #endif

    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    std::string m_GNUPlotFileName = "pgnuplot.exe";
    std::string m_GNUPlotPath = "C:/program files/gnuplot/bin/";
    #elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    std::string m_GNUPlotFileName = "gnuplot-x11";
    std::string m_GNUPlotPath = "/usr/bin/"; // depend de l'ordi , il faut chercher l'emplacement dynamiquement a test sur d'autre pc 
    #endif


    std::string tmp = m_GNUPlotPath  +  m_GNUPlotFileName; // ce fichier peu ne pas exister cf ligne 1764 de gnuplot_i.hpp


    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    gnucmd = _popen(tmp.c_str(),"w");
    #elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    gnucmd = popen(tmp.c_str(),"w");
    #endif
    
    //
    // second look in PATH for Gnuplot
    //
    

    if (!gnucmd)
    {
        std::cout << "Couldn't open connection to gnuplot" << std::endl;
    }
    
    std::string output= "set terminal x11 title \" " + t +"\"\n";
    fputs( output.c_str(), gnucmd);
    fflush(gnucmd);

}

GnuRender::~GnuRender()
{
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
    if (_pclose(gnucmd) == -1)
    #elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    if (pclose(gnucmd) == -1)
    #endif
        std::cout << "erreur lors de la destruction du render" << std::endl;
    
    
}


 void GnuRender::makeGlobalSettings(GlobalSetting s)
 {

    std::string str = "set object 1 rectangle from screen 0,0 to screen 1,1 fillcolo rgb\""+ s.hexaBackColor+ "\" behind";
    fputs( (str+"\n").c_str(), gnucmd );
    fflush(gnucmd);

 }

void GnuRender::draw() 
{
    std::string cmd = memory[0]; 
    for(unsigned int i=1; i<memory.size(); i++)
    {
        cmd += ", " + memory[i];
    }
    std::cout<< cmd << std::endl;
    std::cout << "c'est sensé s'afficher" << std::endl;

    fputs( ("plot " +cmd+"\n").c_str(), gnucmd );
    fflush(gnucmd);

}
void GnuRender::draw(GlobalSetting s)
{
    makeGlobalSettings(s);
    std::string cmd = memory[0]; 
    for(unsigned int i=1; i<memory.size(); i++)
    {
        cmd += ", " + memory[i];
    }
    std::cout << cmd << std::endl;
    
    fputs( ("plot " +cmd+"\n").c_str(), gnucmd );
    fflush(gnucmd);

}

// void GnuRender::drawLine(std::vector<double> x , std::vector<double> y, std::string name, glm::vec3 color)
// {
//     if (x.size() == 0 || y.size() == 0)
//     {
//         std::cout<<"tu te fou de ma geule ???????? envoi moi des donnée fdp" <<std::endl;
//     }

//     if (x.size() != y.size())
//     {
//         std::cout<<" si tu m'envoie meme pas le meme nombre de X que de Y comment veux tu que je comprenne ce que tu veux ?"<<std::endl;
//     }

//     fs::path temp_dir = fs::temp_directory_path();
//     std::string filename = "a";
//     while(fs::exists(fs::path(temp_dir/filename)))
//     {
//         filename = next(filename);
//     }
//     /*statistiquement improbable mais si on lance un autre draw sur un autre gnuRender en parametre il peu y avoir un conflit */

    
//     std::ofstream tmp;
//     tmp.open(fs::path(temp_dir/filename));
//     for (unsigned int i = 0; i < x.size(); i++)
//         tmp << x[i] << " " << y[i] << std::endl;
//     tmp.flush();
//     tmp.close();


//     std::string path = fs::path(temp_dir/filename).string();
//     std::string graph = "\"" + path + "\" using  1 : 2 with lines title \" " + name + "\""; 
//     //1 signifie collonne x et le 2 y


//     memory.push_back(graph);


//     draw();
// }

void GnuRender::drawLine(const Line &l)
{
    // /*on considere que les tableau de x et y sont bien proportioner  */

    // fs::path temp_dir = fs::temp_directory_path();
    // std::string filename = "a";
    // while(fs::exists(fs::path(temp_dir/filename)))
    // {
    //     filename = next(filename);
    // }
    // /*statistiquement improbable mais si on lance un autre draw sur un autre gnuRender en parametre il peu y avoir un conflit */

    
    // std::ofstream tmp;
    // tmp.open(fs::path(temp_dir/filename));
    // for (unsigned int i = 0; i < l.m_x.size(); i++)
    //     tmp << l.m_x[i] << " " << l.m_y[i] << std::endl;
    // tmp.flush();
    // tmp.close();


    // std::string path = fs::path(temp_dir/filename).string();
    // std::string graph = "\"" + path + "\" using  1 : 2 with " + l.settings.mode + " title \" " + l.settings.name + "\"" + " lt rgb \"" + l.settings.hexcolor + "\""; 
    // //1 signifie collonne x et le 2 y
    // memory.push_back(graph);
    addLine(l);
    draw();
}

void GnuRender::drawLine(const Line &l, GlobalSetting s)
{
    addLine(l);
    draw(s);
}

void GnuRender::drawHist(const Hist &h)
{
    if(h.settings.followXAxe)
    {
        addHistFollowX(h);
    }
    else
    {
        addHistOther(h);
    }
    fputs( "set style data histogram \n set xtics rotate out\n 	set style fill solid border\n ", gnucmd );
    draw();
    
}

void GnuRender::drawHist(const Hist &h, GlobalSetting s)
{
    if(h.settings.followXAxe)
    {
        addHistFollowX(h);
    }
    else
    {
        addHistOther(h);
    }
    fputs( "set style data histogram \n set xtics rotate out\n 	set style fill solid border\n ", gnucmd );
    draw(s);
    
}

void GnuRender::addHist(const Hist &h)
{
    if(h.settings.followXAxe)
    {
        addHistFollowX(h);
    }
    else
    {
        addHistOther(h);
    }  
    fputs( "set style data histogram \n set xtics rotate out\n 	set style fill solid border\n ", gnucmd ); 
}

void GnuRender::addLine(const Line &l)
{
    fs::path temp_dir = fs::temp_directory_path();
    std::string filename = "a";
    while(fs::exists(fs::path(temp_dir/filename)))
    {
        filename = next(filename);
    }
    /*statistiquement improbable mais si on lance un autre draw sur un autre gnuRender en parametre il peu y avoir un conflit */

    
    std::ofstream tmp;
    tmp.open(fs::path(temp_dir/filename));
    for (unsigned int i = 0; i < l.m_x.size(); i++)
        tmp << l.m_x[i] << " " << l.m_y[i] << std::endl;
    tmp.flush();
    tmp.close();


    std::string path = fs::path(temp_dir/filename).string();
    std::string graph = "\"" + path + "\" using  1 : 2 with " + l.settings.mode + " title \" " + l.settings.name + "\"" + " lt rgb \"" + l.settings.hexcolor + "\""; 
    //1 signifie collonne x et le 2 y


    memory.push_back(graph);

}

void GnuRender::addHistFollowX(const Hist &h)
{
    fs::path temp_dir = fs::temp_directory_path();
    std::string filename = "a";
    while(fs::exists(fs::path(temp_dir/filename)))
    {
        filename = next(filename);
    }
    /*statistiquement improbable mais si on lance un autre draw sur un autre gnuRender en parametre il peu y avoir un conflit */

    
    std::ofstream tmp;
    tmp.open(fs::path(temp_dir/filename));
    for (unsigned int i = 0; i < h.m_data[0].size(); i++)
        tmp << h.m_data[0][i] << " " << h.m_data[1][i] << std::endl;
    tmp.flush();
    tmp.close();


    std::string path = fs::path(temp_dir/filename).string();
    std::string graph = "\"" + path + "\" using  1 : 2 with boxes title \" " + h.settings.name + "\"" + " lt rgb \"" + h.settings.hexcolor + "\""; 
    //1 signifie collonne x et le 2 y


    memory.push_back(graph);

}

void GnuRender::addHistOther(const Hist &h)
{
    fs::path temp_dir = fs::temp_directory_path();
    std::string filename = "a";
    while(fs::exists(fs::path(temp_dir/filename)))
    {
        filename = next(filename);
    }
    /*statistiquement improbable mais si on lance un autre draw sur un autre gnuRender en parametre il peu y avoir un conflit */

    
    std::ofstream tmp;
    tmp.open(fs::path(temp_dir/filename));
    if(h.m_clusters.size() != h.m_data.size())
    {
        std::cout << h.m_clusters.size() << " clusters et " << h.m_data.size() << " tableau de données" << std::endl;
        std::cout << "le constructeur de Hist doit gérer le fait qu'il y ait un nom par cluster  " << std::endl; 
    }
    tmp << "caca " ;
    for(unsigned int i=0 ; i< h.m_labels.size(); i++)
    {
        tmp << h.m_labels[i] << " ";
    }
    tmp<< std::endl;
    for (unsigned int i = 0; i < h.m_data.size(); i++)
    {
        tmp << h.m_clusters[i] << " ";
        for(unsigned int j = 0; j< h.m_data[i].size(); j++)
        {
            tmp << h.m_data[i][j] << " ";
            
        }
        tmp << std::endl;

    }
        
    tmp.flush();
    tmp.close();


    std::string path = fs::path(temp_dir/filename).string();
    std::string graph = "for [COL= 2:" + std::to_string(h.m_data[0].size()+1) +"] \""+ path + "\"" + " using COL:xticlabels(1) title columnheader(COL)" ;


    memory.push_back(graph);
}


void GnuRender::clear()
{
    memory.clear();
}