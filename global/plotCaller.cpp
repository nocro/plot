#include "plotCaller.hpp"



PlotCaller::PlotCaller(): current(0)
{
    m_listeOfGraph.push_back(new GnuRender());

}


void PlotCaller::next()
{
    m_listeOfGraph[current]->draw();
    m_listeOfGraph.push_back(new GnuRender());
    current ++;
}

void wait_for_key ()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
    cout << endl << "Press any key to continue..." << endl;

    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    _getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
    std::cout << std::endl << "Press ENTER to continue..." << std::endl;

    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
#endif
    return;
}

PlotCaller::~PlotCaller()
{
    wait_for_key();
    
    for(unsigned i =0; i< m_listeOfGraph.size(); i++)
        delete m_listeOfGraph[i];
}

void PlotCaller::draw(GlobalSetting s)
{
    m_listeOfGraph[current]->draw(s);
}

void PlotCaller::draw()
{
    m_listeOfGraph[current]->draw();
}


void PlotCaller::add(const Line &l)
{
    m_listeOfGraph[current]->addLine(l);
}

void PlotCaller::add(const Hist &h)
{
    m_listeOfGraph[current]->addHist(h);
}