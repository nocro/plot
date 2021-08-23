#ifndef RENDER_HPP
#define RENDER_HPP
#include "line.hpp"
#include "hist.hpp"


class Render
{
public: 
    Render();
    Render(std::string t); // un render c'est une fenetre et une fenetre a un titre mais ce titre n'est pas focement  enregistré 
    virtual ~Render() ;// je pense pas qu'il y ai grand chose la dedans

    virtual void draw() = 0; // lance l'affichage dans la fenetre avec des settings par defaut 
    virtual void draw(GlobalSetting s) = 0 ; // lance l'affichage dans la fentre avec des settings

    // virtual void drawLine(const Line &l); // en soi drawLine c'est addLine() + draw() du coup pas forcement besoin de toutes ces fonctions 
    // virtual void drawLine(const Line &l, GlobalSetting s);

    // virtual void drawHist(const Hist &h);
    // virtual void drawHist(const Hist &h, GlobalSetting s);
    
    virtual void addLine(const Line &l) = 0;
    virtual void addHist(const Hist &l) = 0;

    virtual void clear() = 0;

protected : 
    virtual void addHistFollowX(const Hist &h) = 0;
    virtual void addHistOther(const Hist &h) = 0;

    virtual void makeGlobalSettings(GlobalSetting s) = 0; // doit etre virtuel car dans un gnuRender on vas faire des appel a gnuplot et dans opengl on vas modifier un parametre de la class
};

#endif