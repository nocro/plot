#ifndef RENDERVULKAN_H
#define RENDERVULKAN_H

#include <vector> 
#include "Vertex.hpp"
#include "Context.hpp"

#include "../global/render.hpp"

class VulkanRender: public virtual Render
{
public: 
    VulkanRender();
    ~VulkanRender();
    void draw();
    void draw(GlobalSetting s);

    void drawLine(const Line &l);
    void drawLine(const Line &l, GlobalSetting s);

    void drawHist(const Hist &h);
    void drawHist(const Hist &h, GlobalSetting s);
    
    void addLine(const Line &l);
    void addHist(const Hist &l);

    void clear();

private: 
    virtual void addHistFollowX(const Hist &h) ;
    virtual void addHistOther(const Hist &h) ;
    virtual void makeGlobalSettings(GlobalSetting s);

private:
    Context m_context;
    std::unique_ptr<Window> m_window;


};






#endif