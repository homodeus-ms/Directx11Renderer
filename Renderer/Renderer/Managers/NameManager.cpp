#include "pch.h"
#include "NameManager.h"

string NameManager::AddOrGetNewNameIfDuplicated(const string& name)
{
    if (!HasName(name))
    {
        _nameSet.insert(name);
        return name;
    }

    string newName = name;

    if (_duplicatedMap.contains(name))
    {
        _duplicatedMap[name]++;
    }
    else
    {
        _duplicatedMap.insert({ name, 1 });
    }

    uint32 num = _duplicatedMap[name];
    newName += to_string(num);
    _nameSet.insert(newName);

    return newName;
}

bool NameManager::HasName(const string& name)
{
    return _nameSet.contains(name);
}

void NameManager::RemoveName(string name)
{
    if (!_duplicatedMap.empty() && _duplicatedMap.contains(name))
    {
        _duplicatedMap[name]--;
        if (_duplicatedMap[name] == 0)
            _duplicatedMap.erase(name);
    }

    if (!_nameSet.empty())
    {
        auto findIt = _nameSet.find(name);
        if (findIt != _nameSet.end())
            _nameSet.erase(findIt);
    }
}

shared_ptr<NameManager> NameManager::CreateNameManager()
{
    return std::shared_ptr<NameManager>(new NameManager());
}
