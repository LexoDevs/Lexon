#include "../Helpers/VulkanConstants.h"
#include "../Core/VulkanContext.h"


class VulkanFence {

    public:
    VulkanFence(VulkanContext& context)
        : m_Context(context)
    {
    }

    void createSyncObjects();
    void destroyFences();
    void cleanSync();
    

    private:
    VulkanContext& m_Context;






};