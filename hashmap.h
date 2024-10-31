#ifndef HASHMAP_H
#define HASHMAP_H

#include <string>
#include <algorithm>
#include <list>
#include <vector>

using namespace std;

template <typename T>
class HashMap
{
public:

	HashMap(double max_load = 0.75); 
	~HashMap() = default; 

	int size() const {return m_associations;}

	void insert(const string& key, const T& value);

	T& operator[](const string& key);
	
	const T* find(const string& key) const;
	
	T* find(const string& key) {
		const auto& hm = *this;
		return const_cast<T*>(hm.find(key));
	}

private:
	struct HashNode {
		string m_key;
		T m_value;
		HashNode(const string& key, const T& val) : m_key(key), m_value(val) {}
	};

	vector<list< HashNode>> buckets;
	int m_associations = 0;
	double m_load;

	int hashFunction(const string& key) const {
		return (hash<string>()(key) % buckets.size());
	}

	void expandHashMap();
};

template <typename T>
HashMap<T>::HashMap(double max_load) : m_load(max_load) {
	buckets.resize(10);
}


template <typename T>
void HashMap<T>::insert(const string& key, const T& value) {

	int index = hashFunction(key);
	auto& curr = buckets[index];

	for (auto& item : curr) {
		if (item.m_key == key) {
			item.m_value = value;
			return;
		}
	}

	curr.push_back(HashNode(key, value)); 
	m_associations++; 

	if ((static_cast<double>(m_associations) / static_cast<double>(buckets.size())) > m_load) {
		expandHashMap();
	}
}

template <typename T>
T& HashMap<T>::operator[](const string& key) {
	int index = hashFunction(key);
	auto& curr = buckets[index];

	for (auto& item : curr) {
		if (item.m_key == key) {
			return item.m_value;
		}
	}

	
	insert(key, T());

	return *find(key);
}


template <typename T>
const T* HashMap<T>::find(const string& key) const {
	int index = hashFunction(key);
	const auto& curr = buckets[index];

	for (const auto& item : curr) {
		if (item.m_key == key) {
			return &item.m_value;
		}
	}
	return nullptr;
}

template <typename T>
void HashMap<T>::expandHashMap() {
	vector<list<HashNode>> biggerBuckets;

	biggerBuckets.resize(buckets.size() * 2); 

	for (auto& curr : buckets) {
		for (auto& item : curr) {
			int index = (hash<string>()(item.m_key) % biggerBuckets.size()); 
			biggerBuckets[index].push_back(item); 
		}
	}
	buckets = move(biggerBuckets); 
}

#endif