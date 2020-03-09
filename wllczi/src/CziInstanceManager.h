#pragma once

#include <unordered_map>
#include <memory>
#include "CziReader.h"

class CziReaderManager
{
private:
    std::unordered_map<int, std::shared_ptr<CziReader>> map;
public:
    static CziReaderManager Instance;

    void AddInstance(int id);
    void RemoveInstance(int id);
    std::shared_ptr<CziReader> GetInstance(int id);
};