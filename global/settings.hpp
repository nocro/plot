#pragma once

#include <string> 
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>



struct GlobalSetting 
{
	GlobalSetting();
	glm::vec3 backColor;
	std::string hexaBackColor;
	std::pair<double, double> bornesX;
	std::pair<double, double> bornesY;
	bool legend;
};


struct LineSettings
{
	LineSettings();


	glm::vec3 color;
	std::string name;
	std::string hexcolor;
	std::string mode;
	/* valeur possible de mode :
		lines , points, impulses, linespoints
	 */

};



struct HistSettings
{
	HistSettings();
	glm::vec3 color;
	std::string name;
	std::string hexcolor;
	int gap; //espace entre les different cluster
	std::string style ; // set style fill solid border
	bool followXAxe; // nom a changer, savoir si on suit une echelle sur x ou la position sur l'axe x n'a pas d'importance
					 // par exemple pour des stats on vas vouloir que la position de la barre sur l'axe x suive une echelle standare 
					 // pour une election ou le y donne la proportion de vote et en x c'est jsute le nom du candidat, il n'y a donc pas d'echelle en x 
	double width; // largeur des rectangles 
	bool freq; // dit si on divise toute les occurence par la somme total 
	
};