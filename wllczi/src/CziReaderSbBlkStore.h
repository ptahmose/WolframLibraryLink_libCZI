#pragma once

#include <cstdint>
#include <unordered_map>
#include "inc_libCzi.h"

class CziReaderSubBlockStore
{
private:
    std::unordered_map<std::uint32_t, std::shared_ptr<libCZI::ISubBlock>> sbBlkMap;
    std::uint32_t lastHandle;
public:
    CziReaderSubBlockStore();
    std::uint32_t AddSubBlock(std::shared_ptr<libCZI::ISubBlock> sbBlk);
    std::shared_ptr<libCZI::ISubBlock> GetForHandle(std::uint32_t h);
    bool RemoveSubBlock(std::uint32_t h);
private:
    std::uint32_t GetNewHandle();
};