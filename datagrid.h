#ifndef DATAGRID_H
#define DATAGRID_H

template<typename T>
class DataGrid
{
public:
    DataGrid(unsigned int w, unsigned int h) :
        mWidth(w), mHeight(h)
    {
        assert(w != 0 && h != 0);

        mData = new T[mWidth*mHeight];
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
        return mWidth = mHeight;
    }

    inline T* ptr()
    {
        return mData;
    }

    inline const T* ptr() const
    {
        return mData;
    }

    inline T* operator [] (unsigned int row)
    {
        return mData + row*mWidth;
    }

    inline const T* operator [] (unsigned int row) const
    {
        return mData + row*mWidth;
    }

private:
    unsigned int mWidth;
    unsigned int mHeight;
    T* mData;
};

typedef DataGrid<float> FloatDataGrid;
typedef DataGrid<int> IntDataGrid;

#endif // DATAGRID_H
