#include "BlockModelData.h"

// Gets the position with the bits layed out in the format: xxxxxyyyyyyyyzzzzz00000000000000 (5x - 8y - 5z - 0)
unsigned int getPositionBits(const unsigned int& x, const unsigned int& y, const unsigned int& z) {
    return (x << 27) + (y << 19) + (z << 14);
}

// Gets the block data layed out in the format: positionX(5 bits), positionY(8 bits), positionZ(5 bits), blockSide(3 bits), textureId(9 bits), textureOffsetIndex(2) bits
// position.x needs to be between 0 and 15, position.y between 0 and 255, position.z between 0 and 15
std::array<unsigned int, 6> BlockModelData::getBlockData(const BlockSide& cubeSide, const Vector3& position, const int& blockId) {
    BlockId id = (BlockId)blockId;
    const Block& block = BlockManager::getBlock(id);

    unsigned int x = position.x, y = position.y, z = position.z;
    unsigned int sideBytes = (unsigned int)cubeSide << 11;
    unsigned int textureIdBits = block.getTextureId(cubeSide) << 2;
    unsigned int sideAndTextureBits = sideBytes | textureIdBits;
    switch(cubeSide) {
    case BlockSide::Front:
        return { getPositionBits(x    , y    , z + 1) | sideAndTextureBits | 0,
                 getPositionBits(x + 1, y    , z + 1) | sideAndTextureBits | 1,
                 getPositionBits(x + 1, y + 1, z + 1) | sideAndTextureBits | 2,
                 getPositionBits(x + 1, y + 1, z + 1) | sideAndTextureBits | 2,
                 getPositionBits(x    , y + 1, z + 1) | sideAndTextureBits | 3,
                 getPositionBits(x    , y    , z + 1) | sideAndTextureBits | 0,
        };
    case BlockSide::Back:
        return { getPositionBits(x + 1, y    , z    ) | sideAndTextureBits | 0,
                 getPositionBits(x    , y    , z    ) | sideAndTextureBits | 1,
                 getPositionBits(x    , y + 1, z    ) | sideAndTextureBits | 2,
                 getPositionBits(x    , y + 1, z    ) | sideAndTextureBits | 2,
                 getPositionBits(x + 1, y + 1, z    ) | sideAndTextureBits | 3,
                 getPositionBits(x + 1, y    , z    ) | sideAndTextureBits | 0,
        };
    case BlockSide::Left:
        return { getPositionBits(x    , y    , z    ) | sideAndTextureBits | 0,
                 getPositionBits(x    , y    , z + 1) | sideAndTextureBits | 1,
                 getPositionBits(x    , y + 1, z + 1) | sideAndTextureBits | 2,
                 getPositionBits(x    , y + 1, z + 1) | sideAndTextureBits | 2,
                 getPositionBits(x    , y + 1, z    ) | sideAndTextureBits | 3,
                 getPositionBits(x    , y    , z    ) | sideAndTextureBits | 0,
        };
    case BlockSide::Right:
        return { getPositionBits(x + 1, y    , z + 1) | sideAndTextureBits | 0,
                 getPositionBits(x + 1, y    , z    ) | sideAndTextureBits | 1,
                 getPositionBits(x + 1, y + 1, z    ) | sideAndTextureBits | 2,
                 getPositionBits(x + 1, y + 1, z    ) | sideAndTextureBits | 2,
                 getPositionBits(x + 1, y + 1, z + 1) | sideAndTextureBits | 3,
                 getPositionBits(x + 1, y    , z + 1) | sideAndTextureBits | 0,
        };
    case BlockSide::Top:
        return { getPositionBits(x    , y + 1, z + 1) | sideAndTextureBits | 0,
                 getPositionBits(x + 1, y + 1, z + 1) | sideAndTextureBits | 1,
                 getPositionBits(x + 1, y + 1, z    ) | sideAndTextureBits | 2,
                 getPositionBits(x + 1, y + 1, z    ) | sideAndTextureBits | 2,
                 getPositionBits(x    , y + 1, z    ) | sideAndTextureBits | 3,
                 getPositionBits(x    , y + 1, z + 1) | sideAndTextureBits | 0,
        };
    case BlockSide::Down:
        return { getPositionBits(x    , y    , z    ) | sideAndTextureBits | 0,
                 getPositionBits(x + 1, y    , z    ) | sideAndTextureBits | 1,
                 getPositionBits(x + 1, y    , z + 1) | sideAndTextureBits | 2,
                 getPositionBits(x + 1, y    , z + 1) | sideAndTextureBits | 2,
                 getPositionBits(x    , y    , z + 1) | sideAndTextureBits | 3,
                 getPositionBits(x    , y    , z    ) | sideAndTextureBits | 0,
        };
    }
    return {0,0,0,0,0,0};
}

std::array<unsigned int, 6> BlockModelData::getFoliageData(const BlockSide& cubeSide, const Vector3& position, const int& blockId) {
    BlockId id = (BlockId)blockId;
    const Block& block = BlockManager::getBlock(id);

    unsigned int x = position.x, y = position.y, z = position.z;
    unsigned int sideBytes = (unsigned int)cubeSide << 11;
    unsigned int textureIdBits = block.getTextureId(cubeSide) << 2;
    unsigned int sideAndTextureBits = sideBytes | textureIdBits;
    switch(cubeSide) {
    case BlockSide::Front:
        return { getPositionBits(x    , y    , z + 1) | sideAndTextureBits | 0,
                 getPositionBits(x + 1, y    , z    ) | sideAndTextureBits | 1,
                 getPositionBits(x + 1, y + 1, z    ) | sideAndTextureBits | 2,
                 getPositionBits(x + 1, y + 1, z    ) | sideAndTextureBits | 2,
                 getPositionBits(x    , y + 1, z + 1) | sideAndTextureBits | 3,
                 getPositionBits(x    , y    , z + 1) | sideAndTextureBits | 0,
        };
    case BlockSide::Back:
        return { getPositionBits(x + 1, y    , z    ) | sideAndTextureBits | 0,
                 getPositionBits(x    , y    , z + 1) | sideAndTextureBits | 1,
                 getPositionBits(x    , y + 1, z + 1) | sideAndTextureBits | 2,
                 getPositionBits(x    , y + 1, z + 1) | sideAndTextureBits | 2,
                 getPositionBits(x + 1, y + 1, z    ) | sideAndTextureBits | 3,
                 getPositionBits(x + 1, y    , z    ) | sideAndTextureBits | 0,
        };
    case BlockSide::Left:
        return { getPositionBits(x    , y    , z    ) | sideAndTextureBits | 0,
                 getPositionBits(x + 1, y    , z + 1) | sideAndTextureBits | 1,
                 getPositionBits(x + 1, y + 1, z + 1) | sideAndTextureBits | 2,
                 getPositionBits(x + 1, y + 1, z + 1) | sideAndTextureBits | 2,
                 getPositionBits(x    , y + 1, z    ) | sideAndTextureBits | 3,
                 getPositionBits(x    , y    , z    ) | sideAndTextureBits | 0,
        };
    case BlockSide::Right:
        return { getPositionBits(x + 1, y    , z + 1) | sideAndTextureBits | 0,
                 getPositionBits(x    , y    , z    ) | sideAndTextureBits | 1,
                 getPositionBits(x    , y + 1, z    ) | sideAndTextureBits | 2,
                 getPositionBits(x    , y + 1, z    ) | sideAndTextureBits | 2,
                 getPositionBits(x + 1, y + 1, z + 1) | sideAndTextureBits | 3,
                 getPositionBits(x + 1, y    , z + 1) | sideAndTextureBits | 0,
        };
    case BlockSide::Top:
        return { 0,0,0,0,0,0
        };
    case BlockSide::Down:
        return { 0,0,0,0,0,0
        };
    }
    return {0,0,0,0,0,0};
}
