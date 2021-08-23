#include "settings.hpp"



LineSettings::LineSettings(): color(glm::vec3(1.0, 0.0, 0.0)), name("no name"),hexcolor("#FF0000"), mode("lines")
{

}

HistSettings::HistSettings(): color(glm::vec3(1.0, 0.0, 0.0)), name("no name"),hexcolor("#FF0000"), gap(1), style("solid border"), followXAxe(false), freq(false)
{

}

GlobalSetting::GlobalSetting(): backColor(glm::vec3(1.0,1.0,1.0)), hexaBackColor("ffffff"), bornesX(1.0,-1.0), bornesY(1.0,-1.0)
{

}