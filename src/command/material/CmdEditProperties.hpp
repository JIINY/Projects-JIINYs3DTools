#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "common/Math.hpp"
#include "command/ICommand.hpp"
#include "render/Material.hpp"


class CmdEditProperties : public ICommand
{
public:
    CmdEditProperties(std::shared_ptr<Render::Material> targetMat,
        std::unordered_map<std::string, Render::MaterialPropData> propMap, std::unordered_map<std::string, Render::UIPropData> uiPropMap,
        std::vector<uint8_t> beforeBufferData, std::vector<uint8_t>afterBufferData)
        : targetMat_(targetMat), propMap_(std::move(propMap)), uiPropMap_(std::move(uiPropMap)), 
        beforeBufferData_(std::move(beforeBufferData)), afterBufferData_(std::move(afterBufferData)) {}
    ~CmdEditProperties() override = default;

    void execute() override
    {
        applyBuffer(afterBufferData_);
    }

    void undo() override
    {
        applyBuffer(beforeBufferData_);
    }

    bool emitsDataChanged() const override { return true; }


private:
    std::shared_ptr<Render::Material> targetMat_;
    std::unordered_map<std::string, Render::MaterialPropData> propMap_;
    std::unordered_map<std::string, Render::UIPropData> uiPropMap_;
    std::vector<uint8_t> beforeBufferData_;
    std::vector<uint8_t> afterBufferData_;

    void applyBuffer(const std::vector<uint8_t>& bufferData)
    {
        for (const auto& [name, prop] : propMap_)
        {
            const uint8_t* src = bufferData.data() + prop.offset;

            auto uiP = uiPropMap_.find(name);
            if (uiP == uiPropMap_.end()) { continue; }

            switch (uiP->second.data)
            {
            case UI::WidgetData::Float:
            {
                float v;
                memcpy(&v, src, sizeof(v));
                targetMat_->setProperty(name, v);
                break;
            }
            case UI::WidgetData::Float3:
            {
                Math::Vec3 v;
                memcpy(&v, src, sizeof(v));
                targetMat_->setProperty(name, v);
                break;
            }
            case UI::WidgetData::Float4:
            {
                Math::Vec4 v;
                memcpy(&v, src, sizeof(v));
                targetMat_->setProperty(name, v);
                break;
            }
            default:
                break;
            }
        }
    }
};
