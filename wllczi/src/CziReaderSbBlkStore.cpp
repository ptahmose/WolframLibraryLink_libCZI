#include "CziReaderSbBlkStore.h"

#include <utility>

CziReaderSubBlockStore::CziReaderSubBlockStore() : lastHandle(1000)
{
}

std::uint32_t CziReaderSubBlockStore::AddSubBlock(std::shared_ptr<libCZI::ISubBlock> sbBlk)
{
    auto h = this->GetNewHandle();
    this->sbBlkMap[h] = std::move(sbBlk);
    return h;
}

std::shared_ptr<libCZI::ISubBlock> CziReaderSubBlockStore::GetForHandle(std::uint32_t h)
{
    const auto it = this->sbBlkMap.find(h);
    if (it == this->sbBlkMap.cend())
    {
        return std::shared_ptr<libCZI::ISubBlock>();
    }

    return it->second;
}

bool CziReaderSubBlockStore::RemoveSubBlock(std::uint32_t h)
{
    const auto it = this->sbBlkMap.find(h);
    if (it == this->sbBlkMap.cend())
    {
        return false;
    }

    this->sbBlkMap.erase(it);
    return true;
}

std::uint32_t CziReaderSubBlockStore::GetNewHandle()
{
    return ++this->lastHandle;
}