//
// Created by Adilson Tavares on 09/07/20.
//

#include "OPPermissions.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
OPPermissions::SEL_PermissionRequestHandler OPPermissions::readExternalStorageHandler = nullptr;
OPPermissions::SEL_PermissionRequestHandler OPPermissions::writeExternalStorageHandler = nullptr;
#endif

OPPermissions::SEL_PermissionRequestHandler OPPermissions::microphoneHandler = nullptr;