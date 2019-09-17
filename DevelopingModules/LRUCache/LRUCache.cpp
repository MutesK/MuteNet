#include "LRUCache.h"


template <typename Data>
LRUCache<Data>::LRUCache(int capacity)
	:_size(0), _capacity(capacity)
{
}

template <typename Data>
void LRUCache<Data>::Put(size_t id, const Data& data)
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

template <typename Data>
void LRUCache<Data>::Get(size_t id)
{
	const auto iter = _pageMap.find(id);
	if (iter == _pageMap.end())
		return;

	const auto & node = iter->second;
	Data value = node->_data;

	_pageList.remove(node);
	_pageList.push_front(node);
}

template <typename Data>
void LRUCache<Data>::EraseNode(const std::shared_ptr<Node<Data>>& node)
{
	const auto iter = _pageMap.find(node->_id);

	if (iter == _pageMap.end())
		return;

	_pageMap.erase(iter);
	--_size;
}
