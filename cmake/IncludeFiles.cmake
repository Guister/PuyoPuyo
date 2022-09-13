set (SourceFolder ${CMAKE_SOURCE_DIR}/src)
set (GameObjectsFolder ${SourceFolder}/gameObjects)
set (UtilsFolder ${SourceFolder}/utils)


set(Includes
        ${SourceFolder}/main.cpp
        ${SourceFolder}/WindowRenderer.cpp
        ${SourceFolder}/AudioMixer.cpp
        ${SourceFolder}/GameManager.cpp
        ${GameObjectsFolder}/GameObject.cpp
        ${GameObjectsFolder}/Tile.cpp
        ${GameObjectsFolder}/Ball.cpp
        ${GameObjectsFolder}/Block.cpp
        ${GameObjectsFolder}/Grid.cpp
        ${UtilsFolder}/Color.hpp
        ${UtilsFolder}/Direction.hpp
        ${UtilsFolder}/FileSystem.cpp
        )