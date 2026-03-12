#pragma once
#include "dino/xdino.h"

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
        XDino_MemFree(m_data);
    }

    void AddBack(T point)
    {
        if constexpr (std::is_pointer_v<T>)
            if (point == nullptr)
                XDINO_BREAKPOINT();
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

        m_data[currentSize] = point;
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
    
    void RemoveAt(size_t index)
    {
        if (currentSize == 0) return;
        if (index >= currentSize) return;

        T* newData = nullptr;
    
        if (currentSize - 1 > 0)
        {
            newData = (T*)XDino_MemAlloc(sizeof(T) * (currentSize - 1), "DinoArray");

            if (newData != nullptr)
            {
                size_t newIndex = 0;
                for (size_t i = 0; i < currentSize; i++)
                {
                    if (i == index) continue;
                    newData[newIndex] = m_data[i];
                    newIndex++;
                }
            }
        }

        XDino_MemFree(m_data);
        m_data = newData;
        currentSize--;
        capacity = currentSize;
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
