#include "CziInstanceManager.h"

using namespace std;

/*static*/CziReaderManager CziReaderManager::Instance = CziReaderManager();

void CziReaderManager::AddInstance(mint id)
{
    this->map.insert(make_pair(id, make_shared< CziReader>()));
}

void CziReaderManager::RemoveInstance(mint id)
{
    auto noOfElementsRemoved = this->map.erase(id);
}

std::shared_ptr<CziReader> CziReaderManager::GetInstance(mint id)
{
    return this->map.at(id);
}
