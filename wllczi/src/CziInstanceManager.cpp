#include "CziInstanceManager.h"
#include "dbgprint.h"

using namespace std;

/*static*/CziReaderManager CziReaderManager::Instance = CziReaderManager();

void CziReaderManager::AddInstance(mint id)
{
    VDBGPRINT((CDbg::Level::Trace, "CziReaderManager::AddInstance: add instance %" MINTFMT ".", id));
    this->map.insert(make_pair(id, make_shared< CziReader>()));
}

void CziReaderManager::RemoveInstance(mint id)
{
    VDBGPRINT((CDbg::Level::Trace, "CziReaderManager::RemoveInstance: remove instance %" MINTFMT ".", id));
    auto noOfElementsRemoved = this->map.erase(id);
}

std::shared_ptr<CziReader> CziReaderManager::GetInstance(mint id)
{
    return this->map.at(id);
}
