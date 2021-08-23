#ifndef SEMAPHORES_H 
#define SEMAPHORES_H
#include "Device.hpp"


class Semaphores 
{
public:
    Semaphores(VkDevice &device);
    void destroy(VkDevice device);
private:
    void createSyncObjects(VkDevice &device);

public: 
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

};











#endif 