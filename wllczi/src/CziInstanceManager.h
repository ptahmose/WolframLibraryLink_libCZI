#pragma once

#include <WolframLibrary.h>
#include <unordered_map>
#include <memory>
#include "CziReader.h"

class CziReaderManager
{
private:
    std::unordered_map<mint, std::shared_ptr<CziReader>> map;
public:
    static CziReaderManager Instance;

    void AddInstance(mint id);
    void RemoveInstance(mint id);
    std::shared_ptr<CziReader> GetInstance(mint id);
};