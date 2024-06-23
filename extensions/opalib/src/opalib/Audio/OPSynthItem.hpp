//
//  OPSynthItem.hpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 02/11/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#ifndef OPSynthItem_hpp
#define OPSynthItem_hpp

#include <JuceHeader.h>
#include "OPSampler.h"

class OPSynthItem
{
private:
    std::string _filePath;
    int _note; // min 1 max 128
    int _page; // min 1 max 16
    int _group;
    bool _loop{false};
    
public:
    OPSynthItem(const std::string& filePath, int note, int page = 1, int group = -1, bool loop = false);
    
    bool shouldHandleGroup();

    void setFilePath(const std::string& path);

    const std::string& getFilePath() const;

    void setNote(int note);
    int getNote() const;

    void setPage(int id);
    int getPage() const;

    int getId();

    int getGroup() const;
    void setGroup(int group);

    bool isLoop() const;
    void setLoop(bool loop);

    static int getIdFromNoteAndPage(int note, int page);
};

#endif /* OPSynthItem_hpp */
