/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MANA_DEBUGWINDOW_HPP
#define MANA_DEBUGWINDOW_HPP

#include "imgui.h"

#include "engine/display/window.hpp"

#include "engine/render/3d/passes/imguipass.hpp"

#include "gui/stringformat.hpp"

class DebugWindow : public ImGuiPass::Widget {
public:
    struct LayerTreeItem {
        LayerTreeItem() = default;

        LayerTreeItem(int order, const Compositor::Layer &layer)
                : order(order), layer(layer) {}

        int order = 0;
        bool active = true;
        bool pinned = false;
        Compositor::Layer layer;
    };

    void swapItems(int item0, int item1) {
        auto bitem0 = items.at(item0);
        auto bitem1 = items.at(item1);
        bitem0.order = item1;
        bitem1.order = item0;
        items.at(item0) = bitem1;
        items.at(item1) = bitem0;
    }

    void drawLayerNodeButtons(LayerTreeItem &item) {
        if (!item.active) {
            ImGui::SameLine();
            if (ImGui::SmallButton(
                    ("Enable###0_" + std::to_string(item.order)).c_str())) {
                item.active = true;
            }
        } else if (!item.pinned) {
            ImGui::SameLine();
            if (ImGui::SmallButton(
                    ("Disable###1_" + std::to_string(item.order)).c_str())) {
                item.active = false;
            }

            ImGui::SameLine();
            if (ImGui::SmallButton(
                    ("Up###2_" + std::to_string(item.order)).c_str())) {
                auto nIndex = item.order;

                bool foundDeactivated = false;
                //Decrement nIndex until it doesnt point to a deactivated item
                while (true) {
                    if (--nIndex < 0)
                        nIndex = static_cast<int>(items.size() - 1);
                    if (nIndex == item.order) {
                        break;
                    } else if (items.at(nIndex).active && !items.at(nIndex).pinned) {
                        foundDeactivated = true;
                        break;
                    }
                }

                if (foundDeactivated)
                    swapItems(nIndex, item.order);
            }

            ImGui::SameLine();
            if (ImGui::SmallButton(
                    ("Down###3_" + std::to_string(item.order)).c_str())) {
                auto nIndex = item.order;

                bool foundDeactivated = false;
                //Increment nIndex until it doesnt point to a deactivated item
                while (true) {
                    if (++nIndex >= items.size())
                        nIndex = 0;
                    if (nIndex == item.order) {
                        break;
                    } else if (items.at(nIndex).active && !items.at(nIndex).pinned) {
                        foundDeactivated = true;
                        break;
                    }
                }

                if (foundDeactivated)
                    swapItems(nIndex, item.order);
            }
        }
    }

    void drawLayerNode(int nodeIndex, LayerTreeItem &item) {
        if (ImGui::TreeNode((void *) (intptr_t) nodeIndex, "%s", item.layer.name.c_str())) {
            drawLayerNodeButtons(item);

            if (ImGui::TreeNode("Color Textures")) {
                for (auto &color: item.layer.color)
                    ImGui::Text("%s", color.c_str());
                ImGui::TreePop();
            }

            if (!item.layer.depth.empty())
                ImGui::Text("Depth Texture: %s", item.layer.depth.c_str());

            ImGui::Text("Depth Test:    %s",
                        formatDepthTestMode(item.layer.depthTestMode).c_str());
            ImGui::Text("Blend Source:  %s",
                        formatBlendMode(item.layer.colorBlendModeSource).c_str());
            ImGui::Text("Blend Destination: %s",
                        formatBlendMode(item.layer.colorBlendModeDest).c_str());

            ImGui::TreePop();
        } else {
            drawLayerNodeButtons(item);
        }
    }

    void draw(Scene &scene) override {
        ImGui::StyleColorsDark();

        ImGui::Begin("Debug Window");

        ImGui::BeginTabBar("TabBar");

        if (ImGui::BeginTabItem("Rendering")) {
            if (ImGui::TreeNode("Render Layers")) {
                std::map<int, std::vector<std::reference_wrapper<LayerTreeItem>>> activeItems;
                std::vector<std::reference_wrapper<LayerTreeItem>> inactiveItems;
                std::vector<std::reference_wrapper<LayerTreeItem>> pinnedItems;

                for (auto &item: items) {
                    if (item.pinned)
                        pinnedItems.emplace_back(item);
                    else if (item.active)
                        activeItems[item.order].emplace_back(item);
                    else
                        inactiveItems.emplace_back(item);
                }

                if (ImGui::TreeNode("Active")) {
                    int nodeIndex = 0;
                    for (auto &pair: activeItems) {
                        for (auto &item: pair.second)
                            drawLayerNode(nodeIndex++, item);
                    }
                    for (auto &item: pinnedItems) {
                        drawLayerNode(nodeIndex++, item);
                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNode("Inactive")) {
                    int nodeIndex = 0;
                    for (auto &item: inactiveItems)
                        drawLayerNode(nodeIndex++, item);
                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                        ImGui::GetIO().Framerate);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Camera")) {
            ImGui::InputFloat3("Position", (float *) (&scene.camera.transform.position), "%.3f");
            auto euler = scene.camera.transform.rotation.getEulerAngles();
            ImGui::InputFloat3("Rotation", (float *) (&euler), "%.3f");
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();

        ImGui::End();
    }

    std::vector<Compositor::Layer> getSelectedLayers() {
        std::vector<Compositor::Layer> pin;
        std::map<int, std::vector<Compositor::Layer>> tmp;
        for (auto &item: items)
            if (item.pinned)
                pin.emplace_back(item.layer);
            else if (item.active)
                tmp[item.order].emplace_back(item.layer);
        std::vector<Compositor::Layer> ret;
        for (auto &pair: tmp)
            for (auto &layer: pair.second)
                ret.emplace_back(layer);
        for (auto &layer: pin)
            ret.emplace_back(layer);
        return ret;
    }

    void resetSelection() {
        int i = 0;
        for (auto &item: items) {
            item.order = i++;
            item.active = true;
        }
    }

    void setLayers(const std::vector<Compositor::Layer> &l) {
        items.clear();
        int i = 0;
        for (auto &layer: l) {
            items.emplace_back(LayerTreeItem(i++, static_cast<Compositor::Layer>(layer)));
        }
    }

    void setLayerActive(const std::string &name, bool active) {
        for (auto &item: items) {
            if (item.layer.name == name) {
                item.active = active;
                break;
            }
        }
    }

    void setLayerPinned(const std::string &name, bool pin) {
        for (auto &item: items) {
            if (item.layer.name == name) {
                item.pinned = pin;
                break;
            }
        }
    }

private:
    std::string appendButtonLabelId(const std::string &label, int index) {
        return label + "###" + std::to_string(index);
    }

    std::vector<LayerTreeItem> items;
};

#endif //MANA_DEBUGWINDOW_HPP