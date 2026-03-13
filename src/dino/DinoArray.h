#pragma once
#include "dino/xdino.h"

#define DINOARRAY_AS_STDVECTOR 0

#if DINOARRAY_AS_STDVECTOR
template<typename T>
class DinoArray {
    std::vector<T> data;
public:
    DinoArray(size_t size = 0, size_t capacity = 0)
    {
        data.reserve(capacity);
        data.resize(size);
    }

    // Implémenter le reste DinoArray
};
#else

template <typename T>
class DinoArray
{
public:
    DinoArray(size_t size = 0, size_t capacity = 0) : currentSize(size), capacity(capacity)
    {
        if (size > capacity)
            capacity = size;
        if (capacity >= 1)
            m_data = (T*)XDino_MemAlloc(capacity * sizeof(T), "DinoArray");
        else
            m_data = nullptr;
        for (size_t i = 0; i < size; ++i)
            new ((void*)&m_data[i]) T();
    }

    DinoArray(const DinoArray& other) : currentSize(other.currentSize), capacity(other.capacity)
    {
        if (capacity > 0) 
        {
            m_data = (T*)XDino_MemAlloc(sizeof(T) * capacity, "DinoArray");
            if (m_data && other.m_data)
                memcpy(m_data, other.m_data, currentSize * sizeof(T));
        }
        else
        {
            m_data = nullptr;
        }
    }

    ~DinoArray()
    {
        Clear();
    }

    void AddBack(T point)
    {
        if (currentSize >= capacity)
        {
            size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;

            T* newData = (T*)XDino_MemAlloc(sizeof(T) * newCapacity, "DinoArray");

            if (newData != nullptr)
            {
                if (m_data != nullptr)
                    memcpy(newData, m_data, currentSize * sizeof(T));
                XDino_MemFree(m_data);
                m_data = newData;
                capacity = newCapacity;
            }
            else
            {
                return;
            }
        }

        new ((void*)&m_data[currentSize]) T(point);
        currentSize++;
    }
    
    size_t GetSize() const
    {
        return currentSize;
    }

    size_t GetCapacity()
    {
        return capacity;
    }
    
    T& operator[](size_t index) { return m_data[index]; }

    const T& operator[](size_t index) const
    {
        return m_data[index];
    }

    DinoArray& operator=(const DinoArray& other)
    {
        if (this == &other) return *this;
    
        Clear();
    
        currentSize = other.currentSize;
        capacity = other.capacity;
    
        if (capacity > 0)
        {
            m_data = (T*)XDino_MemAlloc(sizeof(T) * capacity, "DinoArray");
            if (m_data && other.m_data)
                memcpy(m_data, other.m_data, currentSize * sizeof(T));
        }
        else
        {
            m_data = nullptr;
        }
    
        return *this;
    }
    
    void RemoveAt(size_t index)
    {
        if (currentSize == 0) return;
        if (index >= currentSize) return;

        for (size_t i = index; i < currentSize - 1; i++) {
            m_data[i].~T();
            m_data[i] = m_data[i + 1];
        }
        currentSize--;
    }

    
    void Clear()
    {
        for (size_t i = 0; i < currentSize; i++)
            m_data[i].~T();
        XDino_MemFree(m_data);
        m_data = nullptr;

        currentSize = 0;
        capacity = 0;
    }
    
    std::string ToString()
    {
        std::string s = "";

        for (size_t i = 0; i < currentSize; i++)
        {
            s += + "{ " +std::to_string(m_data[i].x) + " | " + std::to_string(m_data[i].y) + " }";
            s+= "\n";
        }

        return s;
    }

    T* data() { return m_data; }
    T* begin() { return m_data; }
    T* end() { return m_data + currentSize; }

private:
    T* m_data;
    size_t currentSize;
    size_t capacity;
};
#endif