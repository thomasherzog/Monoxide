#ifndef MONOXIDE_IMGUIEXAMPLEWINDOW_H
#define MONOXIDE_IMGUIEXAMPLEWINDOW_H

#include <monoxide/windowing/ImGuiWindow.h>
#include <monoxide/graphics/vulkan/memory/Texture.h>

class ImGuiExampleWindow : public windowing::ImGuiWindow {
public:
    ImGuiExampleWindow();

    ~ImGuiExampleWindow();

    void onRenderUI() override;

private:
    std::unique_ptr<vulkan::Texture> texture;

    ImTextureID textureId;

};


#endif //MONOXIDE_IMGUIEXAMPLEWINDOW_H
