#pragma once

#include "data.h"

// Collection of element based operations!

template<typename T>
inline Data<T> operator + (const Data<T>& v1, const Data<T>& v2)
{
	Q_ASSERT(v1.hasSameStructure(v2));
	Data<T> ret = v1;
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		ret.set(i, v1.at(i) + v2.at(i));
	}
	return ret;
}

template<typename T>
inline Data<T> operator + (const Data<T>& v1, const T& v2)
{
	Data<T> ret = v1;
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		ret.set(i, v1.at(i) + v2);
	}
	return ret;
}

template<typename T>
inline Data<T> operator - (const Data<T>& v1, const Data<T>& v2)
{
	Q_ASSERT(v1.hasSameStructure(v2));
	Data<T> ret = v1;
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		ret.set(i, v1.at(i) - v2.at(i));
	}
	return ret;
}

template<typename T>
inline Data<T> operator - (const Data<T>& v1, const T& v2)
{
	Data<T> ret = v1;
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		ret.set(i, v1.at(i) - v2);
	}
	return ret;
}

template<typename T>
inline Data<T> operator * (const Data<T>& v1, const Data<T>& v2)
{
	Q_ASSERT(v1.hasSameStructure(v2));
	Data<T> ret = v1;
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		ret.set(i, v1.at(i) * v2.at(i));
	}
	return ret;
}

template<typename T>
inline Data<T> operator * (const Data<T>& v1, const T& v2)
{
	Data<T> ret = v1;
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		ret.set(i, v1.at(i) * v2);
	}
	return ret;
}

template<typename T>
inline Data<T> operator / (const Data<T>& v1, const Data<T>& v2)
{
	Q_ASSERT(v1.hasSameStructure(v2));
	Data<T> ret = v1;
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		ret.set(i, v1.at(i) / v2.at(i));
	}
	return ret;
}

template<typename T>
inline Data<T> operator / (const Data<T>& v1, const T& v2)
{
	Q_ASSERT(v2 != 0);
	Data<T> ret = v1;
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		ret.set(i, v1.at(i) / v2);
	}
	return ret;
}

template<typename T>
inline bool operator == (const Data<T>& v1, const Data<T>& v2)
{
	Q_ASSERT(v1.hasSameStructure(v2));
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		if (v1.at(i) != v2.at(i))
		{
			return false;
		}
	}
	return true;
}

template<typename T>
inline bool operator == (const Data<T>& v1, const T& v2)
{
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		if (v1.at(i) != v2)
		{
			return false;
		}
	}
	return true;
}

template<typename T>
inline bool operator != (const Data<T>& v1, const Data<T>& v2)
{
	return !(v1 == v2);
}

template<typename T>
inline bool operator != (const Data<T>& v1, const T& v2)
{
	return !(v1 == v2);
}

template<typename T>
inline bool operator >= (const Data<T>& v1, const Data<T>& v2)
{
	Q_ASSERT(v1.hasSameStructure(v2));
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		if (v1.at(i) < v2.at(i))
		{
			return false;
		}
	}
	return true;
}

template<typename T>
inline bool operator >= (const Data<T>& v1, const T& v2)
{
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		if (v1.at(i) < v2)
		{
			return false;
		}
	}
	return true;
}

template<typename T>
inline bool operator <= (const Data<T>& v1, const Data<T>& v2)
{
	Q_ASSERT(v1.hasSameStructure(v2));
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		if (v1.at(i) > v2.at(i))
		{
			return false;
		}
	}
	return true;
}

template<typename T>
inline bool operator <= (const Data<T>& v1, const T& v2)
{
	for (Data<T>::size_type i = 0; i < v1.size(); ++i)
	{
		if (v1.at(i) > v2)
		{
			return false;
		}
	}
	return true;
}

template<typename T>
inline bool operator < (const Data<T>& v1, const Data<T>& v2)
{
	return !(v1 >= v2);
}

template<typename T>
inline bool operator < (const Data<T>& v1, const T& v2)
{
	return !(v1 >= v2);
}

template<typename T>
inline bool operator > (const Data<T>& v1, const Data<T>& v2)
{
	return !(v1 <= v2);
}

template<typename T>
inline bool operator > (const Data<T>& v1, const T& v2)
{
	return !(v1 <= v2);
}