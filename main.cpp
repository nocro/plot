//#include "global/plot.hpp"
#include "vulkan/VulkanRender.hpp"

double f(double x)
{
    return x*x;
}

// PlotCaller p0,p1;

int main (int argc, char *argv[])
{
    std::vector<double> x1;
    std::vector<double> y1;
    for(double i=0.0; i< 2.0; i+=0.01)
    {
        x1.push_back(i);
        y1.push_back(f(i)); 

    }
    std::vector<double> x = {0.0, 1.0, 1.2, 1.3, 1.6};
    std::vector<double> y = {0.0, 1.0, 1.6, 2.0, 1.0};
    // GnuRender render("je suis le roi du monde ");
    /*du coup le tire devrai etre un parametre de la ligne */
    
    
    
    LineSettings s;
    s.mode = "linespoints";
    s.name = "titre";
    s.hexcolor = "#00FF00";
    //plot(Line(x1,f,s));

    std::vector<double> data = {0.1,0.2,0.3,1.5,1.6,1.8,3.0};
    //plot(Hist(data,4));


    VulkanRender render;
    render.drawLine(Line(x1,f,s));

    
     
    
}

/*
note: 
    -faire une fonction qui permet a l'utilisateur de juste envoyer des comande préalable dans la console gnuplot 
    -il y a des style pour chaque "lines" mais il faut des settings pour la fenetres aussi, exemple backgroundcolor ou titre de la fenetre/graphs 
 */