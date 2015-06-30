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
	struct InternalData
	{
		size_type mLinearSize;
		std::vector<size_type> mSize;
		value_type* mData;
		size_type mRefs;
	};

	InternalData* mRef;

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
		mRef(nullptr)
	{
		Q_ASSERT(size.size() >= 1);

		mRef = new InternalData;
		mRef->mRefs = 1;
		mRef->mLinearSize = 1;

		for(size_type i : size)
		{
			Q_ASSERT(i > 0);
			mRef->mLinearSize *= i;
		}

		mRef->mData = new value_type[mRef->mLinearSize];
		memset(mRef->mData, 0, sizeof(value_type)*mRef->mLinearSize);
	}

	Data(const Data& data) :
		mRef(data.mRef)
	{
		mRef->mRefs++;
	}

	~Data()
	{
		mRef->mRefs--;

		if (mRef->mRefs == 0)
		{
			delete [] mRef->mData;
			delete mRef;
		}
	}

	inline Data<value_type>& operator = (const Data& data)
	{
		mRef->mRefs--;

		if (mRef->mRefs == 0)
		{
			delete[] mRef->mData;
			delete mRef;
		}

		mRef = data.mRef;
		mRef->mRefs++;

		return *this;
	}

	// Size
	inline size_type size() const
	{
		return mRef->mLinearSize;
	}

	inline size_type linearSize() const
	{
		return mRef->mLinearSize;
	}

	inline element_size_type elementSize() const
	{
		return mRef->mSize;
	}

	inline size_type dimension() const
	{
		return mRef->mSize.size();
	}

	inline bool isSingle() const
	{
		return mRef->mLinearSize == 1;
	}

	inline bool isVector() const
	{
		return mRef->mSize.size() == 1 && mRef->mSize.at(0) > 1;
	}

	inline bool isMultidimensional() const
	{
		return mRef->mSize.size() > 1;
	}

	// Access
	inline pointer ptr()
	{
		return mRef->mData;
	}

	inline const_pointer ptr() const
	{
		return mRef->mData;
	}
	
	inline DataAccessor operator [] (size_type x) const
	{
		return DataAccessor(mRef->mData, mRef->mSize)[x];
	}

	inline value_type at(const element_size_type& indexes) const
	{
		size_type index = 0;

		for (size_type i = 0; i < indexes.size(); ++i)
		{
			index += indexes.at(i)*(i == 0 ? 1 : mRef->mSize.at(i - 1));
		}

		return mRef->mData[index];
	}

	inline value_type at (size_type i) const
	{
		return mRef->mData[i];
	}

	inline void set(const element_size_type& indexes, const_reference val)
	{
		size_type index = 0;

		for (size_type i = 0; i < indexes.size(); ++i)
		{
			index += indexes.at(i)*(i == 0 ? 1 : mRef->mSize.at(i - 1));
		}
		
		mRef->mData[index] = val;
	}

	inline void set(size_type i, const_reference val)
	{
		mRef->mData[i] = val;
	}

	inline value_type operator () () const
	{
		Q_ASSERT(isSingle());
		return mRef->mData[0];
	}

	// Utility
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

	// Iterators
	inline iterator begin()
	{
		return iterator(mRef->mData);
	}

	inline const_iterator begin() const
	{
		return const_iterator(mRef->mData);
	}

	inline const_iterator cbegin() const
	{
		return begin();
	}

	inline iterator end()
	{
		iterator itr(mRef->mData);
		itr.mPos = mRef->mLinearSize;
		return itr;
	}

	inline const_iterator end() const
	{
		iterator itr(mRef->mData);
		itr.mPos = mRef->mLinearSize;
		return itr;
	}

	inline const_iterator cend() const
	{
		return end();
	}

	// Union/Merge/Split
	inline Data<value_type> split(const element_size_type& start, const element_size_type& end)
	{
		Q_ASSERT(start.size() == end.size());

		element_size_type diff;
		for (size_type i = 0; i < start.size(); ++i)
		{
			size_type d = end.at(i) - start.at(i);
			Q_ASSERT(d != 0);

			diff.push_back(d);
		}

		Data<value_type> newData(diff);

		return newData;
	}
};

typedef Data<float> FloatData;
typedef Data<int> IntData;
