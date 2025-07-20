#pragma once

class NameManager
{
public:
	
	string AddOrGetNewNameIfDuplicated(const string& name);
	bool HasName(const string& name);
	void RemoveName(string name);

private:
	NameManager() {}

	static shared_ptr<NameManager> CreateNameManager();

	friend class Renderer;

	unordered_set<string> _nameSet;
	unordered_map<string, uint32> _duplicatedMap;
};

