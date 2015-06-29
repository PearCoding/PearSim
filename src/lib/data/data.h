#pragma once

#include "config.h"
#include <limits>

template<typename T>
class PS_LIB_INLINE Data
{
public:
	class DataIterator;
	class DataAccessor;

	typedef T value_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef DataIterator iterator;
	typedef const DataIterator const_iterator;
	typedef ptrdiff_t difference_type;
	typedef size_t size_type;
	typedef std::vector<size_type> element_size_type;

private:
	size_type mLinearSize;
	std::vector<size_type> mSize;
	value_type* mData;

public:

	class DataAccessor
	{
	private:
		pointer mData;
		element_size_type mSize;
		mutable element_size_type mValues;

		DataAccessor(pointer data, const element_size_type& size) :
			mData(data), mSize(size)
		{
		}

		inline DataAccessor& operator [] (size_type x) const
		{
			Q_ASSERT(mValues.size() != mSize.size());

			mValues.push_back(x);//Reverse order
			return *this;
		}

		inline value_type operator() () const
		{
			Q_ASSERT(mValues.size() == mSize.size());

			size_t index = 0;

			for (size_t i = 0; i < mValues.size(); ++i)
			{
				index += mValues.at(i)*(i == 0 ? 1 : mSize.at(i - 1));
			}

			return mData[index];
		}

		inline DataAccessor& operator = (value_type val)
		{
			Q_ASSERT(mValues.size() == mSize.size());

			size_t index = 0;

			for (size_t i = 0; i < mValues.size(); ++i)
			{
				index += mValues.at(i)*(i == 0 ? 1 : mSize.at(i - 1));
			}

			mData[index] = val;
		}
	};

	//TODO: Can be std::random_access_iterator_tag
	class DataIterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
	{
		friend Data;
	private:
		pointer mData;
		size_type mPos;

	public:
		DataIterator() :
			mData(nullptr), mPos(0)
		{
		}

		DataIterator(pointer data) :
			mData(data), mPos(0)
		{
		}

		DataIterator(const DataIterator& itr) : mData(itr.mData), mPos(itr.mPos)
		{
		}

		inline DataIterator& operator++()
		{
			++mPos; return *this;
		}

		inline DataIterator operator++(int)
		{
			DataIterator tmp(*this); operator++(); return tmp;
		}

		inline DataIterator& operator--()
		{
			--mPos; return *this;
		}

		inline DataIterator operator--(int)
		{
			DataIterator tmp(*this); operator--(); return tmp;
		}

		inline bool operator == (const DataIterator& rhs) const
		{
			return mData == rhs.mData && mPos == rhs.mPos;
		}

		inline bool operator != (const DataIterator& rhs) const
		{
			return !(*this == rhs);
		}

		inline value_type operator*() const
		{
			return mData[mPos];
		}

		inline reference operator*()
		{
			return mData[mPos];
		}
	};

	Data(size_type w = 1) :
		Data(element_size_type({ w }))
	{
	}

	Data(size_type w, size_type h) :
		Data(element_size_type({ w, h }))
	{
	}

	Data(const element_size_type& size) :
		mSize(size)
	{
		Q_ASSERT(size.size() >= 1);

		mLinearSize = 1;

		for(size_type i : size)
		{
			Q_ASSERT(i > 0);
			mLinearSize *= i;
		}

		mData = new value_type[mLinearSize];
		memset(mData, 0, sizeof(value_type)*mLinearSize);
	}

	~Data()
	{
		delete [] mData;
	}

	inline size_type size() const
	{
		return mLinearSize;
	}

	inline size_type linearSize() const
	{
		return mLinearSize;
	}

	inline element_size_type elementSize() const
	{
		return mSize;
	}

	inline size_type dimension() const
	{
		return mSize.size();
	}

	inline bool isSingle() const
	{
		return mLinearSize == 1;
	}

	inline bool isVector() const
	{
		return mSize.size() == 1 && mSize.at(0) > 1;
	}

	inline bool isMultidimensional() const
	{
		return mSize.size() > 1;
	}

	inline pointer ptr()
	{
		return mData;
	}

	inline const_pointer ptr() const
	{
		return mData;
	}
	
	inline DataAccessor operator [] (size_type x) const
	{
		return DataAccessor(mData, mSize)[x];
	}

	inline value_type at(const element_size_type& indexes) const
	{
		size_type index = 0;

		for (size_type i = 0; i < indexes.size(); ++i)
		{
			index += indexes.at(i)*(i == 0 ? 1 : mSize.at(i - 1));
		}

		return mData[index];
	}

	inline value_type at (size_type i) const
	{
		return mData[i];
	}

	inline void set(const element_size_type& indexes, const_reference val)
	{
		size_type index = 0;

		for (size_type i = 0; i < indexes.size(); ++i)
		{
			index += indexes.at(i)*(i == 0 ? 1 : mSize.at(i - 1));
		}
		
		mData[index] = val;
	}

	inline void set(size_type i, const_reference val)
	{
		mData[i] = val;
	}

	inline value_type operator () () const
	{
		Q_ASSERT(isSingle());
		return mData[0];
	}

	inline value_type max() const
	{
		value_type m = std::numeric_limits<value_type>::min();
		for(value_type t : *this)
		{
			m = t > m ? t : m;
		}
		return m;
	}

	inline value_type min() const
	{
		value_type m = std::numeric_limits<value_type>::max();
		for(value_type t : *this)
		{
			m = t < m ? t : m;
		}
		return m;
	}

	inline iterator begin()
	{
		return iterator(mData);
	}

	inline const_iterator begin() const
	{
		return const_iterator(mData);
	}

	inline const_iterator cbegin() const
	{
		return begin();
	}

	inline iterator end()
	{
		iterator itr(mData);
		itr.mPos = mLinearSize;
		return itr;
	}

	inline const_iterator end() const
	{
		iterator itr(mData);
		itr.mPos = mLinearSize;
		return itr;
	}

	inline const_iterator cend() const
	{
		return end();
	}
};

typedef Data<float> FloatData;
typedef Data<int> IntData;
