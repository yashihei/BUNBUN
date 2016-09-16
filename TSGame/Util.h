#pragma once
#include <algorithm>

template <typename Type> inline const Type& clamp(const Type& x, const Type& min, const Type& max) {
	return (x < min) ? min : ((max < x) ? max : x);
}
template <typename Type> inline Type wrap(const Type& x, const Type& min, const Type& max) {
	const Type n = (x - min) % (max - min);
	return (n >= 0) ? (n + min) : (n + max);
}
template <typename Container, typename Pred> inline void Erase_if(Container& c, Pred pred) {
	c.erase(std::remove_if(c.begin(), c.end(), pred), c.end());
}
template <typename Type> inline void OutputDebugValue(const Type& value) {
#ifdef _DEBUG
	OutputDebugString(std::to_string(value).c_str());
	OutputDebugString("\n");
#endif
}
