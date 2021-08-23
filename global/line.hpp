#pragma once

#include <iostream>
#include <vector> 
#include "settings.hpp"

class Line
{
public:
	Line(std::vector<double> x, std::vector<double> y);
	Line(std::vector<double> x, double f(double));
	Line(std::vector<double> x, std::vector<double> y, LineSettings s);
	Line(std::vector<double> x, double f(double), LineSettings s);

	std::vector<double> getX(); 
	std::vector<double> getY(); 
	~Line();
	

	LineSettings settings;
	/*je met les vecteurs en public car j'ai pas envi de faire de copie dans gnuRender */
	std::vector<double> m_x;
	std::vector<double> m_y; 

	

};

