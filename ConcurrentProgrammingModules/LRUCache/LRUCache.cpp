#include "LRUCache.h"



LRUCache::LRUCache(int capacity)
	:_size(0), _capacity(capacity)
{
}

void LRUCache::Put(size_t id, const std::string& data)
{
	const auto iter = _pageMap.find(id);

	if(iter != _pageMap.end())
	{
		auto nodePtr = iter->second;
		nodePtr->_data = data;

		// Change Head
		_pageList.remove(nodePtr);
		_pageList.push_front(nodePtr);
		return;
	}

	// Size Check
	if(_size < _capacity)
	{
		//1. Remove Rear
		const auto rearNode = *_pageList.rbegin();
		EraseNode(rearNode);
	}

	const auto newNode = std::make_shared<Node>(id, data);
	_pageList.push_front(newNode);
	_pageMap[id] = newNode;
	_size++;
}

void LRUCache::Get(size_t id)
{
	const auto iter = _pageMap.find(id);
	if (iter == _pageMap.end())
		return;

	const auto & node = iter->second;
	std::string value = node->_data;

	_pageList.remove(node);
	_pageList.push_front(node);
}


void LRUCache::EraseNode(const std::shared_ptr<Node>& node)
{
	const auto iter = _pageMap.find(node->_id);

	if (iter == _pageMap.end())
		return;

	_pageMap.erase(iter);
	--_size;
}
