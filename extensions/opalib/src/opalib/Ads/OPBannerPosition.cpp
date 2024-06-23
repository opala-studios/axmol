//
//  OPBannerPosition.cpp
//  DrumsCocos-mobile
//
//  Created by Adilson Tavares on 02/07/19.
//

#include "OPBannerPosition.hpp"

const OPBannerPosition OPBannerPosition::TopLeft = OPBannerPosition(OPBannerPosition::Horizontal::Left, OPBannerPosition::Vertical::Top);
const OPBannerPosition OPBannerPosition::TopCenter = OPBannerPosition(OPBannerPosition::Horizontal::Center, OPBannerPosition::Vertical::Top);
const OPBannerPosition OPBannerPosition::TopRight = OPBannerPosition(OPBannerPosition::Horizontal::Right, OPBannerPosition::Vertical::Top);
const OPBannerPosition OPBannerPosition::BottomLeft = OPBannerPosition(OPBannerPosition::Horizontal::Left, OPBannerPosition::Vertical::Bottom);
const OPBannerPosition OPBannerPosition::BottomCenter = OPBannerPosition(OPBannerPosition::Horizontal::Center, OPBannerPosition::Vertical::Bottom);
const OPBannerPosition OPBannerPosition::BottomRight = OPBannerPosition(OPBannerPosition::Horizontal::Right, OPBannerPosition::Vertical::Bottom);

OPBannerPosition::OPBannerPosition(OPBannerPosition::Horizontal horizontal, OPBannerPosition::Vertical vertical)
{
    this->horizontal = horizontal;
    this->vertical = vertical;
}
