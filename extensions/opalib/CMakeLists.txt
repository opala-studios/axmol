set(target_name opalib)

#FILE(GLOB_RECURSE OPALIB_SOURCES *.h;*.cpp)

set (OPALIB_SOURCES
    src/opalib/Opalib.h
    src/opalib/Opalib.cpp
    src/opalib/Debug/OPDebug.hpp
    src/opalib/Debug/OPDebug.cpp
    src/opalib/Pooling/OPPooling.hpp
    src/opalib/Scene/OPBaseScene.hpp
    src/opalib/Scene/OPBaseScene.cpp
    src/opalib/Spawner/SpawnerGObject.hpp
    src/opalib/Spawner/SpawnerGObject.cpp
    src/opalib/Spawner/ParticleGObject.hpp
    src/opalib/Spawner/ParticleGObject.cpp
    src/opalib/Fairy/OPGRoot.hpp
    src/opalib/Fairy/OPGRoot.cpp
#    src/opalib/Helpers/OPString.cpp
#    src/opalib/Helpers/OPString.hpp
    src/opalib/Utils/OPFairyUtils.hpp
    src/opalib/Utils/OPFairyUtils.cpp
    src/opalib/Utils/ComponentPool.hpp
    src/opalib/Math/OPCurve.hpp
    src/opalib/Math/OPCurve.cpp

#    src/opalib/Notifications/OPLocalNotificationManager.hpp
#    src/opalib/Notifications/OPLocalNotificationManager.cpp
)

# Detectar plataforma iOS e adicionar arquivos específicos
if(IOS)
    list(APPEND OPALIB_SOURCES
            src/opalib/iOS/Opalib_iOS.mm
            )
elseif(APPLE)
    list(APPEND OPALIB_SOURCES
            src/opalib/iOS/Opalib_iOS.mm
            )
endif()

add_library(${target_name} STATIC ${OPALIB_SOURCES})

target_include_directories(
    ${target_name}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(
    ${target_name}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/opalib
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/opalib
)

setup_ax_extension_config(${target_name})
