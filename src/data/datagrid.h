#ifndef DATAGRID_H
#define DATAGRID_H

#include <QtGlobal>
#include <limits>

template<typename T>
class DataGrid
{
public:
    template<typename T2>
    class DataAccessor
    {
    private:
        int mX;
        int mWidth;
        T2* mData;

    public:
        inline T2* operator [] (unsigned int y) const
        {
            return mData[mX+y*mWidth];
        }
    };

    DataGrid(unsigned int w, unsigned int h) :
        mWidth(w), mHeight(h)
    {
        Q_ASSERT(w != 0 && h != 0);

        mData = new T[mWidth*mHeight];
        memset(mData, 0, sizeof(T)*mWidth*mHeight);
    }

    ~DataGrid()
    {
        delete [] mData;
    }

    inline unsigned int width() const
    {
        return mWidth;
    }

    inline unsigned int height() const
    {
        return mHeight;
    }

    inline unsigned int area() const
    {
        return mWidth*mHeight;
    }

    inline bool isQuad() const
    {
        return mWidth == mHeight;
    }

    inline T* ptr()
    {
        return mData;
    }

    inline const T* ptr() const
    {
        return mData;
    }

    inline DataAccessor<T> operator [] (unsigned int x)
    {
        DataAccessor<T> acc;
        acc.mX = x;
        acc.mData = mData;
        acc.mWidth = mWidth;
        return a;
    }

    inline DataAccessor<const T> operator [] (unsigned int x) const
    {
        DataAccessor<const T> acc;
        acc.mX = x;
        acc.mData = mData;
        acc.mWidth = mWidth;
        return acc;
    }

    inline T at(unsigned int x, unsigned int y) const
    {
        return mData[x + y*mWidth];
    }

    inline void set(unsigned int x, unsigned int y, T val)
    {
        mData[x + y*mWidth] = val;
    }

    inline T max() const
    {
        T m = std::numeric_limits<T>::min();
        for(unsigned int i = 0; i < mWidth; ++i)
        {
            for(unsigned int j = 0; j < mHeight; ++j)
            {
                T t = mData[i + j*mWidth];
                m = t > m ? t : m;
            }
        }
        return m;
    }

    inline T min() const
    {
        T m = std::numeric_limits<T>::max();
        for(unsigned int i = 0; i < mWidth; ++i)
        {
            for(unsigned int j = 0; j < mHeight; ++j)
            {
                T t = mData[i + j*mWidth];
                m = t < m ? t : m;
            }
        }
        return m;
    }

private:
    unsigned int mWidth;
    unsigned int mHeight;
    T* mData;
};

typedef DataGrid<float> FloatDataGrid;
typedef DataGrid<int> IntDataGrid;

#endif // DATAGRID_H
