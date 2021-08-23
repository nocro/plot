#ifndef PLOTCALLER_H
#define PLOTCALLER_H

#include "../gnuPlot/gnuRender.hpp"
class PlotCaller
{
public:
 	PlotCaller();
	~PlotCaller();
    PlotCaller(PlotCaller&& p) = default;
    PlotCaller(const PlotCaller& p) = default;
    PlotCaller& operator=(PlotCaller&& p) = default;
    PlotCaller& operator=(const PlotCaller& p) = default;


	void add(const Line &l);
    void add(const Hist &h);
    void draw();
    void draw(GlobalSetting s);
    void next();

private:
    int current;
	std::vector<Render*> m_listeOfGraph;


};
#endif