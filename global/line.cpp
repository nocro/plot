#include "line.hpp"




Line::Line(const std::vector<double> x, const std::vector<double> y): settings(), m_x(x), m_y(y) 
{

}

Line::Line(const std::vector<double> x, double f(double)):  settings(), m_x(x)
{
	//std::vector<double> tmp; 
	for (unsigned int i = 0; i < m_x.size(); i++) {
		m_y.push_back(f(m_x[i]));
	}
	
}

Line::Line(const std::vector<double> x, const std::vector<double> y, LineSettings s): settings(s), m_x(x), m_y(y) 
{

}

Line::Line(const std::vector<double> x, double f(double), LineSettings s):  settings(s), m_x(x), m_y()
{
	for (unsigned int i = 0; i < m_x.size(); i++) {
		m_y.push_back(f(m_x[i]));
	}
}




Line::~Line()
{
}

std::vector<double> Line::getX() {
	return m_x;
}
std::vector<double> Line::getY() {
	return m_y;
}