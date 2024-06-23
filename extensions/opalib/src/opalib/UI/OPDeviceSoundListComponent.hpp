//
// Created by Talles Borges  on 16/07/20.
//

#ifndef OPALIBTEMPLATE_CLASSES_COMPONENTS_KITEDITOR_OPDEVICESOUNDLISTCOMPONENT_HPP_
#define OPALIBTEMPLATE_CLASSES_COMPONENTS_KITEDITOR_OPDEVICESOUNDLISTCOMPONENT_HPP_

#include "./OPFairyComponent.hpp"
#include "../Native/OPDeviceSound.hpp"
#include "../Audio/OPPlayer.hpp"

struct OPDeviceSoundListComponentParams : public OPFairyComponentParams
{
    std::string listId;
    std::function<void(OPDeviceSound*)> itemClickListener;
    std::function<void()> onItemPlayed;
};

class OPDeviceSoundComponent;

class OPDeviceSoundListComponent : public OPFairyComponent
{
public:
    CREATE_FUNC_WITH_ARGS(OPDeviceSoundListComponent)

    void setItems(const cocos2d::Vector<OPDeviceSound*>& items);
    void setItemClickListener(const std::function<void(OPDeviceSound*)>& itemClickListener);
    void stopPlayer();

    void setAudioSource(const std::string& audioSource);
    std::string getAudioSource() const { return _audioSource; }

protected:
    void onConstruct() override;
    void renderListItem(int index, fairygui::GObject* obj);
    virtual void onRenderItem(int index, OPDeviceSoundComponent* item) {};

    fairygui::GList* _itemsList;
    cocos2d::Vector<OPDeviceSound*> _items;
    std::function<void(OPDeviceSound*)> _itemClickListener;
    std::function<void()> _onItemPlayed;
    std::string _audioSource;

    void playItem(int index);

    void updateList();

    int _playingIndex = -1;
    std::unique_ptr<OPPlayer> _soundPlayer;

    void onEnter() override;
    void onExit() override;

public:
    virtual ~OPDeviceSoundListComponent();
};

#endif // OPALIBTEMPLATE_CLASSES_COMPONENTS_KITEDITOR_OPDEVICESOUNDLISTCOMPONENT_HPP_
