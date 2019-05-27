#pragma once

#include "../ConcurrentProgrammingModules/CommonHeader.h"

/*
 * LRUCache is not Thread-Safe.
 * in now, it's an experience function. 
 */

struct Node
{
	size_t _id;
	std::string _data;
	Node* _prevNode = nullptr;
	Node* _nextNode = nullptr;

	Node(size_t id, const std::string& data)
		:_id(id), _data(data)
	{
	}
};

class LRUCache
{
private:
	size_t _capacity;
	size_t _size;
	std::list<std::shared_ptr<Node>> _pageList;
	std::map<size_t, std::shared_ptr<Node>> _pageMap;

public:
	LRUCache(int capacity);
	~LRUCache() = default;

	void Put(size_t id, const std::string& data);
	void Get(size_t id);

private:
	void EraseNode(const std::shared_ptr<Node>& node);
};

