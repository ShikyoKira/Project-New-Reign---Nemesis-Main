#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <deque>

#include "utilities/line.h"


template <class T>
using Vec = std::vector<T>;

template <class T>
using Deq = std::deque<T>;

template <class T>
using SPtr = std::shared_ptr<T>;

template <class T>
using UPtr = std::unique_ptr<T>;

template <class T>
using WPtr = std::weak_ptr<T>;

template <class T>
using USet = std::unordered_set<T>;

template <class T, class P = std::less<T>>
using Set = std::set<T, P>;

template <class K, class V>
using UMap = std::unordered_map<K, V>;

template <class K, class V, class P = std::less<K>>
using Map = std::map<K, V, P>;

template <class T>
using VecSPtr = Vec<SPtr<T>>;

using VecChar  = Vec<char>;
using VecStr   = Vec<std::string>;
using VecWstr  = Vec<std::wstring>;
using VecNstr  = Vec<nemesis::Line>;
using VecNwstr = Vec<nemesis::Wline>;

using USetStr  = USet<std::string>;
using USetWstr = USet<std::wstring>;

using SetStr  = Set<std::string>;
using SetWstr = Set<std::wstring>;

using ID = UMap<std::string, int>;

using uint = unsigned int;

using UMapStr2 = UMap<std::string, std::string>;
using UMapSetStr = UMap<std::string, Set<std::string>>;
