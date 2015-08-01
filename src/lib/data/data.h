#pragma once

#include "config.h"
#include <limits>

/**
 * @brief	A general multidimensional data container.
 *
 * @author	PearCoding
 * @date	31.07.2015
 *
 * @tparam	T	Base value type.
 */

template<typename T>
class PS_LIB_INLINE Data
{
public:
	class DataIterator;
	class DataAccessor;

	/**
	 * @brief	Defines an alias representing type of the value.
	 */
	typedef T value_type;

	/**
	 * @brief	Defines an alias representing the reference.
	 */
	typedef value_type& reference;

	/**
	 * @brief	Defines an alias representing the constant reference.
	 */
	typedef const value_type& const_reference;

	/**
	 * @brief	Defines an alias representing a pointer to value_type.
	 */
	typedef value_type* pointer;

	/**
	 * @brief	Defines an alias representing a constant pointer to value_type.
	 */
	typedef const value_type* const_pointer;

	/**
	 * @brief	Defines an alias representing the iterator.
	 */
	typedef DataIterator iterator;

	/**
	 * @brief	Defines an alias representing the constant iterator.
	 */
	typedef const DataIterator const_iterator;

	/**
	 * @brief	Defines an alias representing type of the difference.
	 */
	typedef ptrdiff_t difference_type;

	/**
	 * @brief	Defines an alias representing type of the size.
	 */
	typedef size_t size_type;

	/**
	 * @brief	Defines an alias representing type of the element size.
	 */
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

	/**
	 * @brief	A proxy for data access and manipulation.
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 */
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

		/**
		 * @brief	Index operator which crawls one deeper into the dimension.
		 *
		 * @author	PearCoding
		 * @date	31.07.2015
		 *
		 * @param	x	The index at the current dimension.
		 *
		 * @return	DataAccessor which can be used to gain access and crawl deeper.
		 */
		inline DataAccessor& operator [] (size_type x)
		{
			Q_ASSERT(mValues.size() != mData->elementSize().size());

			mValues.push_back(x);//Reverse order
			return *this;
		}

		//inline const DataAccessor& operator [] (size_type x) const
		//{
		//	Q_ASSERT(mValues.size() != mData->elementSize().size());

		//	mValues.push_back(x);//Reverse order
		//	return *this;
		//}

		/**
		 * @brief	Return value from the current index.
		 *
		 * @author	PearCoding
		 * @date	31.07.2015
		 *
		 * @attention	Index should be at the last dimension!
		 * 				
		 * @return	The value at the current index.
		 */
		inline value_type operator() () const
		{
			Q_ASSERT(mValues.size() == mData->elementSize().size());
			return mData->at(mValues);
		}

		/**
		 * @brief	Assign value to the current index.
		 *
		 * @author	PearCoding
		 * @date	31.07.2015
		 *
		 * @attention	Index should be at the last dimension!
		 * 				
		 * @param	val	The new value.
		 * @return	This object.
		 */

		inline DataAccessor& operator = (value_type val)
		{
			Q_ASSERT(mValues.size() == mData->elementSize().size());
			mData->set(mValues, val);
			return *this;
		}
	};


	/**
	 * @brief	The general iterator class for Data.
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 * 			
	 * @todo	Can be std::random_access_iterator_tag
	 */
	class DataIterator : public std::iterator<std::bidirectional_iterator_tag, value_type>
	{
		friend Data<T>;
	private:
		Data<T>* mData;
		mutable size_type mPos;

	public:

		/**
		 * @brief	Creates iterator and points at first element.
		 *
		 * @author	PearCoding
		 * @date	31.07.2015
		 *
		 * @param	data	Underlying data instance
		 */

		DataIterator(Data<T>* data) :
			mData(data), mPos(0)
		{
		}

		/**
		 * @brief	Copy constructor.
		 *
		 * @author	PearCoding
		 * @date	31.07.2015
		 *
		 * @param	itr	The itr.
		 */
		DataIterator(const DataIterator& itr) :
			mData(itr.mData), mPos(itr.mPos)
		{
		}

		/**
		 * @brief	Walking to next element.
		 *
		 * @author	PearCoding
		 * @date	31.07.2015
		 *
		 * @return	This object.
		 */
		inline DataIterator& operator++()
		{
			++mPos; return *this;
		}

		/**
		* @brief	Walking to next element.
		*
		* @author	PearCoding
		* @date		31.07.2015
		*
		* @return	This object.
		*/
		inline const DataIterator& operator++() const
		{
			++mPos; return *this;
		}

		/**
		* @brief	Walking to next element.
		*
		* @author	PearCoding
		* @date		31.07.2015
		*
		* @return	Copy of iterator before the increment.
		*/
		inline DataIterator operator++(int) const
		{
			DataIterator tmp(*this); operator++(); return tmp;
		}

		/**
		* @brief	Walking to previous element.
		*
		* @author	PearCoding
		* @date		31.07.2015
		*
		* @return	This object.
		*/
		inline DataIterator& operator--()
		{
			--mPos; return *this;
		}

		/**
		* @brief	Walking to previous element.
		*
		* @author	PearCoding
		* @date		31.07.2015
		*
		* @return	This object.
		*/
		inline const DataIterator& operator--() const
		{
			--mPos; return *this;
		}

		/**
		* @brief	Walking to previous element.
		*
		* @author	PearCoding
		* @date		31.07.2015
		*
		* @return	Copy of iterator before the increment.
		*/
		inline DataIterator operator--(int) const
		{
			DataIterator tmp(*this); operator--(); return tmp;
		}

		/**
		 * @brief	Equality operator.
		 *
		 * @author	PearCoding
		 * @date	31.07.2015
		 *
		 * @param	rhs	The right hand side iterator.
		 *
		 * @return	true if the iterators are considered equivalent.
		 */
		inline bool operator == (const DataIterator& rhs) const
		{
			return mData == rhs.mData && mPos == rhs.mPos;
		}

		/**
		 * @brief	Inequality operator.
		 *
		 * @author	PearCoding
		 * @date	31.07.2015
		 *
		 * @param	rhs	The right hand side iterator.
		 *
		 * @return	true if the iterators are not considered equivalent.
		 */
		inline bool operator != (const DataIterator& rhs) const
		{
			return !(*this == rhs);
		}

		/**
		 * @brief	Dereferencing operator
		 *
		 * @author	PearCoding
		 * @date	31.07.2015
		 *
		 * @return	The value of the current pointed element.
		 */
		inline value_type operator*() const
		{
			return mData->at(mPos);
		}
	};

	/**
	 * @brief	Creates object with given dimensional sizes (up to 3D)
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @param	w	The size of the first dimension.
	 * @param	h	The size of the second dimension.
	 * @param	d	The size of the third dimension.
	 */
	Data(size_type w = 1, size_type h = 0, size_type d = 0) :
		Data({ w, h, d})
	{
	}

	/**
	 * @brief	Creates object with given dimensional size list
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @param	size	List of sizes (per dimension).
	 * 					Elements equal zero are skipped.
	* 					One element must be non-zero.
	 */
	Data(std::initializer_list<size_type> size) :
		Data(element_size_type(size))
	{
	}

	/**
	* @brief	Creates object with given dimensional size list
	*
	* @author	PearCoding
	* @date		31.07.2015
	*
	* @param	size	List of sizes (per dimension).
	* 					Elements equal zero are skipped.
	* 					One element must be non-zero.
	*/
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

	/**
	 * @brief	Copy constructor. Based on Copy on Write design.
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @param	data	The referenced object
	 */
	Data(const Data& data) :
		mRef(data.mRef)
	{
		mRef->mRefs++;
	}

	/**
	 * @brief	Destructor.
	 * 			
	 * Dereference data and deletes it when needed.
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 */
	~Data()
	{
		mRef->mRefs--;

		if (mRef->mRefs == 0)
		{
			delete [] mRef->mData;
			delete mRef;
		}
	}

	/**
	 * @brief	Assignment operator.
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @param	data	The data.
	 *
	 * @return	This object.
	 */
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

	/** @name Structure
	 *  Operations only working with the structure of the data container.
	 */
	///@{
	
	/**
	 * @brief	Returns the linear size of the container.
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @return	Linear size of the container.
	 * @see		linearSize()
	 */
	inline size_type size() const
	{
		return mRef->mLinearSize;
	}

	/**
	* @brief	Returns the linear size of the container.
	*
	* @author	PearCoding
	* @date		31.07.2015
	*
	* @return	Linear size of the container.
	* @see		size()
	*/
	inline size_type linearSize() const
	{
		return mRef->mLinearSize;
	}

	/**
	 * @brief	Returns the list of size per dimension
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @return	List of sizes
	 */

	inline element_size_type elementSize() const
	{
		return mRef->mSize;
	}

	/**
	 * @brief	Gets the dimension of the container.
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @return	The dimension.
	 */
	inline size_type dimension() const
	{
		return mRef->mSize.size();
	}

	/**
	 * @brief	Checks if container contains only one single element.
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @return	true if only one element, false if not.
	 */
	inline bool isSingle() const
	{
		return mRef->mLinearSize == 1;
	}

	/**
	 * @brief	Checks if the object is vector sized.
	 * 			
	 * Vector structure is defined as dimension() == 1 and size() > 1
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @return	true if has vector structure, false if not.
	 */
	inline bool isVector() const
	{
		return mRef->mSize.size() == 1 && mRef->mSize.at(0) > 1;
	}

	/**
	 * @brief	Checks if the object is multidimensional
	 *
	 * Multidimensional is dimension() > 1. 
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @return	true if multidimensional, false if not.
	 */
	inline bool isMultidimensional() const
	{
		return mRef->mSize.size() > 1;
	}

	/**
	 * @brief	Checks if 'ref' has the same structure.
	 *
	 * Same structure is achieved when 'ref' has same dimension
	 * and same size. 
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @param	ref	The reference data container.
	 *
	 * @return	true if same structure, false if not.
	 */
	inline bool hasSameStructure(const Data<T>& ref) const
	{
		return ref.dimension() == dimension() &&
			ref.linearSize() == linearSize();
	}

	/**
	 * @brief	Removes single sized dimensions.
	 *  
	 *  Dimensions which are single sized will be removed. Object can not be empty after this
	 *  operation.
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 */
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
	///@}

	/** @name Access
	 *  Operations working on elements of the container and their values.
	 */
	///@{
	
	/**
	 * @brief	Returns pointer to internal data array.
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @attention	No reference handling provided. Use makeSingle() when needed.
	 *
	 * @return	A pointer to internal data array.
	 * 			
	 * @see makeSingle()
	 */
	inline pointer ptr()
	{
		return mRef->mData;
	}

	/**
	 * @brief	Returns const pointer to internal data array.
	 *
	 * @author	PearCoding
	 * @date	31.07.2015
	 *
	 * @return	A const pointer to internal data array.
	 */
	inline const_pointer ptr() const
	{
		return mRef->mData;
	}

	/**
	 * @brief	Returns DataAccessor for the index at the first dimension.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @param	x	Index at the first dimension.
	 *
	 * @return	DataAccessor with first dimension set.
	 */
	inline DataAccessor operator [] (size_type x)
	{
		return DataAccessor(this)[x];
	}

	/**
	 * @brief	Returns value from full list of indexes.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @param	indexes	The given indexes per dimension.
	 *
	 * @return	Value at the given index.
	 */
	inline value_type at(const element_size_type& indexes) const
	{
		return mRef->mData[toLinear(indexes)];
	}

	/**
	 * @brief	Returns value from linear index.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @param	i	Linear index.
	 *
	 * @return	Value at the given index.
	 */
	inline value_type at (size_type i) const
	{
		return mRef->mData[i];
	}

	/**
	 * @brief	Sets value at given index list.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @param	indexes	The given indexes per dimension.
	 * @param	val	   	The new value.
	 */
	inline void set(const element_size_type& indexes, const_reference val)
	{		
		makeSingle();
		mRef->mData[toLinear(indexes)] = val;
	}

	/**
	 * @brief	Sets value at given index.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @param	i  	Linear index.
	 * @param	val	The new value.
	 */
	inline void set(size_type i, const_reference val)
	{
		makeSingle();
		mRef->mData[i] = val;
	}

	/**
	 * @brief	Returns value when single structured.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @attention	Data has to be single structured.
	 * @return	Value at index 0.
	 * @see		isSingle()
	 */
	inline value_type operator () () const
	{
		Q_ASSERT(isSingle());
		return mRef->mData[0];
	}
	///@}

	/** @name Utility
	 *  Some utility operations.
	 */

	///@{.
	/**
	 * @brief	Determines the maximum value.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @return	The maximum value.
	 */
	inline value_type max() const
	{
		value_type m = std::numeric_limits<value_type>::min();
		for(value_type t : *this)
		{
			m = t > m ? t : m;
		}
		return m;
	}

	/**
	 * @brief	Determines the minimum value.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @return	The minimum value.
	 */
	inline value_type min() const
	{
		value_type m = std::numeric_limits<value_type>::max();
		for(value_type t : *this)
		{
			m = t < m ? t : m;
		}
		return m;
	}

	/**
	 * @brief	Calculates the absolute values.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @return	New data container with all values >= 0.
	 */

	inline Data<value_type> abs() const
	{
		Data<value_type> newData = *this;
		for (size_type i = 0; i < newData.linearSize(); ++i)
		{
			newData.set(i, newData.at(i) < 0 ? -newData.at(i) : newData.at(i));
		}
		return newData;
	}
	///@}

	/** @name Iterators
	 *  Operations only working with iterators.
	 */
	///@{
	
	/**
	 * @brief	Return iterator to beginning
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @return	An iterator pointing at the first element.
	 */
	inline iterator begin()
	{
		return iterator(this);
	}

	/**
	 * @brief	Return const_iterator to beginning.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @return	An const_iterator pointing at the first element.
	 */
	inline const_iterator begin() const
	{
		return const_iterator(const_cast<Data<T>*>(this));
	}

	/**
	 * @brief	Return const_iterator to beginning.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @return	An const_iterator pointing at the first element.
	 */
	inline const_iterator cbegin() const
	{
		return begin();
	}

	/**
	 * @brief	Return iterator to end.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @return	An iterator pointing after the last element.
	 */
	inline iterator end()
	{
		iterator itr(this);
		itr.mPos = mRef->mLinearSize;
		return itr;
	}

	/**
	* @brief	Return const_iterator to end.
	*
	* @author	PearCoding
	* @date		02.08.2015
	*
	* @return	An const_iterator pointing after the last element.
	*/
	inline const_iterator end() const
	{
		iterator itr(const_cast<Data<T>*>(this));
		itr.mPos = mRef->mLinearSize;
		return itr;
	}

	/**
	* @brief	Return const_iterator to end.
	*
	* @author	PearCoding
	* @date		02.08.2015
	*
	* @return	An const_iterator pointing after the last element.
	*/
	inline const_iterator cend() const
	{
		return end();
	}

	///@}

	/** @name Set-Operations
	 *  Math based set operations.
	 */
	///@{
	
	/**
	 * @brief	Returns container only containing elements from start index to end index.
	 *
	 * The new data container can contain single sized dimensions. Use trim() when needed. 
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @param	start	The start index.
	 * @param	end  	The end index.
	 *
	 * @return	New data container;
	 * @see		trim()
	 */
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

	///@}

	/** @name Index
	 *  Operations working with the index scheme of the data container.
	 */
	///@{
	
	/**
	 * @brief	Converts a list of indexes per dimension to a linear index.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @param	indexes	The indexes per dimension.
	 *
	 * @return	Linear index.
	 */
	inline size_type toLinear(const element_size_type& indexes) const
	{
		size_type index = 0;
		for (size_type i = 0; i < indexes.size(); ++i)
		{
			index += indexes.at(i)*(i == 0 ? 1 : mRef->mSize.at(i - 1));
		}
		return index;
	}

	/**
	 * @brief	Converts a linear index to a list of indexes per dimension.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 *
	 * @param	index	Linear index.
	 *
	 * @return	The indexes per dimension.
	 */
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

	///@}

	/** @name Referencing
	*  Operations working with the referencing mechanism.
	*/
	///@{
	
	/**
	 * @brief	Forces the data container to deattach from other references and allocates his own space.
	 *
	 * @author	PearCoding
	 * @date	02.08.2015
	 */
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
	///@}
};

/**
 * @brief	Defines an alias based on float as base value type.
 */
typedef Data<float> FloatData;

/**
* @brief	Defines an alias based on double as base value type.
*/
typedef Data<double> DoubleData;

/**
 * @brief	Defines an alias based on int as base value type.
 */
typedef Data<int> IntData;
