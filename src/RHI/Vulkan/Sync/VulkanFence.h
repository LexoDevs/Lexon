#include "../Helpers/VulkanConstants.h"
#include "../Core/VulkanContext.h"


class VulkanFence {

    public:
    VulkanFence();
    ~VulkanFence();


    void createSyncObjects(VkDevice device);
    void destroyFences();
    //void cleanSync();
    VkSemaphore GetimageAvailableSemaphore(int frame) {return imageAvailableSemaphore[frame];};
    VkSemaphore GetrenderFinishedSemaphore(int frame) {return renderFinishedSemaphore[frame];};
    VkFence GetinFlightFence(uint32_t frame) {return inFlightFence[frame];};

    private:
    VkSemaphore imageAvailableSemaphore[MAX_FRAMES_IN_FLIGHT]{};
    VkSemaphore renderFinishedSemaphore[MAX_SWAPCHAIN_IMAGES]{};
    VkFence inFlightFence[MAX_FRAMES_IN_FLIGHT]{};
    


    VkDevice cp_device = VK_NULL_HANDLE;





};