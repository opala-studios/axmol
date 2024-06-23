//
//  OPSynthItem.cpp
//  opalib-ios
//
//  Created by Arthur Motelevicz on 02/11/20.
//  Copyright © 2020 Opala Studios. All rights reserved.
//

#include "OPSynthItem.hpp"


OPSynthItem::OPSynthItem(const std::string& filePath,
            int note,
            int page,
            int group,
            bool loop):
            _filePath(filePath),
            _note(note),
            _page(page),
            _group(group),
            _loop(loop)
{
    
}

bool OPSynthItem::shouldHandleGroup()
{
    return _group != -1;
}
void OPSynthItem::setFilePath(const std::string& path)
{
    _filePath = path;
}

const std::string& OPSynthItem::getFilePath() const
{
    return _filePath;
}

void OPSynthItem::setNote(int note)
{
    _note = note;
}

int OPSynthItem::getNote() const
{
    return _note;
}

void OPSynthItem::setPage(int id)
{
    _page = id;
}

int OPSynthItem::getPage() const
{
    return _page;
}

int OPSynthItem::getId()
{
    return _note + _page*100;
}

int OPSynthItem::getGroup() const
{
    return _group;
}

void OPSynthItem::setGroup(int group)
{
    _group = group;
}

//handy function
int OPSynthItem::getIdFromNoteAndPage(int note, int page)
{
    return note + page*100;
}

bool OPSynthItem::isLoop() const {
    return _loop;
}

void OPSynthItem::setLoop(bool loop) {
    _loop = loop;
}
