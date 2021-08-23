#ifndef PLOT_H
#define PLOT_H

#include "plotCaller.hpp"
PlotCaller p;
// template<typename... Plots>
// PlotCaller plot(Plots&&... ps) //-> decltype(  (std::forward<Plots>(ps).draw(Graphique*, std::pair<float, float>, std::pair<float, float>), ...) )
// {
// 	PlotCaller p;
// 	//GlobalSetting s;

// 	(p.add(ps), ...);
// 	p.next();
	
// 	return p;
// }

template<typename... Plots>
void plot(Plots&&... ps) //-> decltype(  (std::forward<Plots>(ps).draw(Graphique*, std::pair<float, float>, std::pair<float, float>), ...) )
{
	
	//GlobalSetting s;

	(p.add(ps), ...);
	p.next();
	
}



#endif