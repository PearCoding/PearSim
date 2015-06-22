#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <vector>

template <typename T>
class DataContainer
{
public:
    class DataAccess
    {

    };

    DataContainer()
    {

    }

    ~DataContainer()
    {

    }

    unsigned int entries() const
    {
        return mContent.size();
    }

    void add(const T& t)
    {
        mContent.push_back(t);
    }

    void set(unsigned int i, const T& t)
    {
        mContent[i] = t;
    }

    const T& get(unsigned int i) const
    {
        return mContent.at(i);
    }

    const T& operator [] (unsigned int i) const
    {
        return mContent[i];
    }

private:
    DataContainer(const DataContainer<T>& t) = delete;
    DataContainer& operator = (const DataContainer<T>& t) = delete;

    std::vector<T> mContent;
};

#endif // DATACONTAINER_H
