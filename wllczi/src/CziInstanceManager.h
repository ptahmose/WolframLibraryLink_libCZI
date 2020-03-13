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

    /// Gets the instance associated with the specified id.
    /// If the specified key is not present, then an out_of_range-exception is thrown.
    /// \param id The identifier.
    /// \returns The instance.
    std::shared_ptr<CziReader> GetInstance(mint id);
};