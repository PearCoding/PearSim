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
		element_size_type mSize;
		value_type* mData;
		size_type mRefs;
	};

	InternalData* mRef;

public:

	class DataAccessor
	{
		friend Data<T>;
	private:
		Data<T>* mData;
		mutable element_size_type mValues;

		DataAccessor(Data<T>* data) :
			mData(data)
		{
		}

	public:
		inline DataAccessor& operator [] (size_type x)
		{
			Q_ASSERT(mValues.size() != mData->size().size());

			mValues.push_back(x);//Reverse order
			return *this;
		}

		//inline const DataAccessor& operator [] (size_type x) const
		//{
		//	Q_ASSERT(mValues.size() != mData->size().size());

		//	mValues.push_back(x);//Reverse order
		//	return *this;
		//}

		inline value_type operator() () const
		{
			Q_ASSERT(mValues.size() == mData->size().size());
			return mData->at(mValues);
		}

		inline DataAccessor& operator = (value_type val)
		{
			Q_ASSERT(mValues.size() == mData->size().size());
			mData->set(mValues, val);
			return *this;
		}
	};

	//TODO: Can be std::random_access_iterator_tag
	class DataIterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
	{
		friend Data<T>;
	private:
		Data<T>* mData;
		mutable size_type mPos;

	public:
		DataIterator() :
			mData(nullptr), mPos(0)
		{
		}

		DataIterator(Data<T>* data) :
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

		inline const DataIterator& operator++() const
		{
			++mPos; return *this;
		}

		inline DataIterator operator++(int) const
		{
			DataIterator tmp(*this); operator++(); return tmp;
		}

		inline DataIterator& operator--()
		{
			--mPos; return *this;
		}

		inline const DataIterator& operator--() const
		{
			--mPos; return *this;
		}

		inline DataIterator operator--(int) const
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
			return mData->at(mPos);
		}
	};

	// Constructors
	Data(size_type w = 1, size_type h = 0, size_type d = 0) :
		Data({ w, h, d})
	{
	}

	Data(std::initializer_list<size_type> size) :
		Data(element_size_type(size))
	{
	}

	Data(const element_size_type& size) :
		mRef(nullptr)
	{
		Q_ASSERT(size.size() >= 1);

		mRef = new InternalData;
		mRef->mRefs = 1;
		mRef->mLinearSize = 1;
		mRef->mSize = size;

		for (element_size_type::iterator it = mRef->mSize.begin();
			it != mRef->mSize.end();
			)
		{
			if (*it != 0)
			{
				mRef->mLinearSize *= *it;
				++it;
			}
			else
			{
				it = mRef->mSize.erase(it);

				if (it == mRef->mSize.end())
				{
					break;
				}
			}
		}

		mRef->mSize.shrink_to_fit();

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

	inline void trim()
	{
		element_size_type newSize;

		for (element_size_type::iterator it = mRef->mSize.begin();
			it != mRef->mSize.end();
			++it)
		{
			if (*it != 1)
			{
				newSize.push_back(*it);
			}
		}

		if (newSize.empty())
		{
			newSize.push_back(1);
		}

		if (newSize.size() != mRef->mSize.size())
		{
			makeSingle();
			mRef->mSize = newSize;
		}
	}

	// Access
	inline pointer ptr()//Attention: No reference handling!
	{
		return mRef->mData;
	}

	inline const_pointer ptr() const
	{
		return mRef->mData;
	}

	inline DataAccessor operator [] (size_type x)
	{
		return DataAccessor(this)[x];
	}

	inline value_type at(const element_size_type& indexes) const
	{
		return mRef->mData[toLinear(indexes)];
	}

	inline value_type at (size_type i) const
	{
		return mRef->mData[i];
	}

	inline void set(const element_size_type& indexes, const_reference val)
	{		
		makeSingle();
		mRef->mData[toLinear(indexes)] = val;
	}

	inline void set(size_type i, const_reference val)
	{
		makeSingle();
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
		return iterator(this);
	}

	inline const_iterator begin() const
	{
		return const_iterator(const_cast<Data<T>*>(this));
	}

	inline const_iterator cbegin() const
	{
		return begin();
	}

	inline iterator end()
	{
		iterator itr(this);
		itr.mPos = mRef->mLinearSize;
		return itr;
	}

	inline const_iterator end() const
	{
		iterator itr(const_cast<Data<T>*>(this));
		itr.mPos = mRef->mLinearSize;
		return itr;
	}

	inline const_iterator cend() const
	{
		return end();
	}

	// Union/Merge/Split
	inline Data<value_type> split(const element_size_type& start, const element_size_type& end) const
	{
		Q_ASSERT(start.size() == end.size());

		element_size_type diff;
		for (size_type i = 0; i < start.size(); ++i)
		{
			size_type d = end.at(i) - start.at(i) + 1;
			Q_ASSERT(d > 0);
			diff.push_back(d);
		}

		Data<value_type> newData(diff);
		for (size_type i = 0; i < newData.linearSize(); ++i)
		{
			element_size_type newIndexes = newData.toMulti(i);
			element_size_type oldIndexes = newIndexes;

			for (size_type j = 0; j < start.size(); ++j)
			{
				oldIndexes[j] += start.at(j);
			}

			newData.set(i, at(oldIndexes));
		}

		return newData;
	}

	// Access utils
	inline size_type toLinear(const element_size_type& indexes) const
	{
		size_type index = 0;
		for (size_type i = 0; i < indexes.size(); ++i)
		{
			index += indexes.at(i)*(i == 0 ? 1 : mRef->mSize.at(i - 1));
		}
		return index;
	}

	inline element_size_type toMulti(size_type index) const
	{
		element_size_type indexes;
		for (size_type i = 0; i < mRef->mSize.size(); ++i)
		{
			indexes.push_back(index % mRef->mSize.at(i));
			index /= mRef->mSize.at(i);
		}
		return indexes;
	}

	void makeSingle()
	{
		if (mRef->mRefs > 1)
		{
			mRef->mRefs--;

			InternalData* newRef = new InternalData;
			newRef->mLinearSize = mRef->mLinearSize;
			newRef->mSize = mRef->mSize;
			newRef->mRefs = 1;

			newRef->mData = new value_type[newRef->mLinearSize];
			memcpy(newRef->mData, mRef->mData, sizeof(value_type)*newRef->mLinearSize);

			mRef = newRef;
		}
	}
};

typedef Data<float> FloatData;
typedef Data<int> IntData;
