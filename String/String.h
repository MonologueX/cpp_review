#pragma once 
#include <iostream>
#include <string.h>
#include <assert.h>
using namespace std;

class String
{
public:
    String(const char* str = "")
        : m_size(strlen(str))
        , m_capacity(m_size)
    {
        m_str = new char[m_size+1];
        strcpy(m_str, str);
    }

    void Swap(String& s)
    {
        swap(m_str, s.m_str);
        swap(m_size, s.m_size);
        swap(m_capacity, s.m_capacity);
    }

    String(const String& s)
        : m_str(nullptr)
    {
        String temp(s.m_str);
        Swap(temp);
    }

    String& operator=(String s)
    {
        Swap(s);
        return *this;
    }

    ///////////////////////////////////////////////
    //  接口
    ///////////////////////////////////////////////
    void Reserve(size_t n)
    {
        Expand(n);
    }
    void Resize(size_t n, char ch = '\0')
    {
        if (n < m_size)
        {
            m_size = n;
            m_str[m_size] = '\0';
        }
        else 
        {
            if (n > m_capacity)
            {
                Expand(n);
            }
            for (size_t i = m_size; i < n; i++)
            {
                m_str[i] = ch;
            }
            m_str[n] = '\0';
            m_size = n;
        }
    }

    void Expand(size_t n)
    {
        if (n > m_capacity)
        {
            char* temp = new char[n+1];
            strcpy(temp, m_str);
            delete[] m_str;
            m_str = temp;
            m_capacity = n;
        }
    }

    void PushBack(char ch)
    {
        //if (m_size == m_capacity)
        //{
        //    Expand(m_capacity*2);
        //}
        //m_str[m_size] = ch;
        //m_str[++m_size] = '\0';
        Insert(m_size, ch);
    }

    void Append(const char* str)
    {
        size_t len = strlen(str);
        if (m_size+len > m_capacity)
        {
            //size_t new_capacity = m_capacity*2;
            //while ( new_capacity < m_size+len )
            //{
            //    new_capacity *= 2;
            //}
            //Expand(new_capacity);
            Expand(m_size+len);
        }
        strcpy(m_str+m_size, str);
        m_size += len;
        //Insert(m_size, str);
    }

    void Insert(size_t pos, char ch)
    {
        assert(pos <= m_size);
        if (m_capacity == m_size)
        {
            Expand(m_capacity*2);
        }
        int end = m_size;
        while (end >= (int)pos)
        {
            m_str[end+1] = m_str[end];
            --end;
        }
        m_str[pos] = ch;
        ++m_size;
    }
    void Insert(size_t pos, const char* str)
    {
        assert(pos <= m_size);
        size_t len = strlen(str);
        if (m_size+len > m_capacity)
        {
            Expand(m_size+len);
        }
        int end = m_size;
        while (end >= (int)pos)
        {
            m_str[end+len] = m_str[end];
            --end;
        }
        strncpy(m_str+pos, str, len);
        m_size += len;
    }

    void PopBack()
    {
        assert(m_size>0);
        --m_size;
        m_str[m_size] = '\0';
    }

    void Erase(size_t pos, size_t len)
    {
        assert(pos<m_size);
        if (pos + len >= m_size)
        {
            m_str[pos] = '\0';
            m_size = pos;
        }
        else 
        {
            strcpy(m_str+pos, m_str+pos+len);
            m_size -= len;
        }
    }

    size_t Size()
    {
        return m_size;
    }
    size_t Capacity()
    {
        return m_capacity;
    }
    bool Empty()
    {
        return m_size == 0;
    }

    char& operator[](size_t pos)
    {
        return m_str[pos];
    }

    size_t Find(char ch)
    {
        for (size_t i = 0; i < m_size; i++)
        {
            if (m_str[i] == ch)
            {
                return i;
            }
        }
        return npos;
    }
    size_t Find(const char* sub) const
    {
        char* src = m_str;
        while (*src)
        {
            char* src_temp = src;
            const char* sub_temp = sub;
            while (*sub_temp && *src_temp == *sub_temp)
            {
                ++src_temp;
                ++sub_temp;
            }
            if (*sub_temp == '\0')
            {
                return src-m_str;
            }
            else 
            {
                ++src;
            }
        }
        return npos;
    }

    String Substr(size_t pos, size_t n) const
    {
        String s;
        assert(pos > 0 && pos < m_size);
        assert(n>0);
        for (size_t i = 0; i < n && i < m_size; i++)
        {
            s += m_str[pos+i];
        }
        return s;
    }

    bool operator<(const String& s) const
    {
        const char* str1 = m_str;
        const char* str2 = s.m_str;
        while (*str1 && *str2)
        {
            if (*str1 < *str2)
            {
                return true;
            }
            else if (*str1 > *str2)
            {
                return false;
            }
            else 
            {
                ++str1;
                ++str2;
            }
        }
        if (*str1 == '\0' && *str2 != '\0')
        {
            return true;
        }
        return false;
    }
    bool operator<=(const String& s) const 
    {
        return *this < s || *this == s;
    }
    bool operator>(const String& s) const 
    {
        return !(*this <= s);
    }
    bool operator>=(const String& s) const 
    {
        return !(*this < s);
    }
    bool operator==(const String& s) const 
    {
        const char* str1 = m_str;
        const char* str2 = s.m_str;
        while (*str1 && *str2)
        {
            if (*str1 != *str2)
            {
                return false;
            }
            else 
            {
                ++str1;
                ++str2;
            }
        }
        if (*str1 == '\0' && *str2 == '\0')
        {
            return true;
        }
        return false;
    }
    bool operator!=(const String& s) const 
    {
        return !(*this == s);
    }
    String& operator+=(const String& s)
    {
        this->Append(s.m_str);
        // *this += s.m_str;
        return *this;
    }
    String& operator+=(const char* str)
    {
        this->Append(str);
        return *this;
    }
    String& operator+=(char c)
    {
        this->PushBack(c);
        return *this;
    }
    String operator+(const char* str)
    {
        String ret(*this);
        ret.Append(str);
        return ret;
    }
    String operator+(const String s)
    {
        return *this + s.m_str;
    }

    ~String()
    {
        if (m_str)
        {
            delete[] m_str;
            m_str = nullptr;
        }
    }

    char* c_str()
    {
        return m_str;
    }
private:
    char* m_str;
    size_t m_size;
    size_t m_capacity;
public:
    static size_t npos;
};
size_t String::npos = -1;

void TestString()
{
    String s1("Hello");
    cout << s1.c_str() << endl;
    //s1.PushBack(' ');
    //s1.Insert(5, ' ');
    //s1.Append("world");
    //s1.Insert(0, 'h');
    //s1 += " World";
    //cout << s1.c_str() << endl;
    size_t pos = s1.Find("llo");
    if (pos != String::npos)
    {
        cout << "找到了: " << pos << endl;
    }
    String str = s1.Substr(1, 2);
    cout << str.c_str() << endl;
}

#if 0
class String
{
public:
    String(const char* str)
        : m_str(new char[strlen(str)+1])
    {
        strcpy(m_str, str);
    }

    String(const String& s)
        : m_str(new char[strlen(s.m_str)+1])
    {
        strcpy(m_str, s.m_str);
    }

    String& operator=(const String& s)
    {
        if (this != &s)
        {
            char* temp = new char[strlen(s.m_str)+1];
            strcpy(temp, s.m_str);
            delete [] m_str;
            m_str = temp;
            return *this;
        }
        return *this;
    }
    ~String()
    {
        if (m_str)
        {
            delete[] m_str;
            m_str = nullptr;
        }
    }

    char* c_str()
    {
        return m_str;
    }
private:
    char* m_str;
};
#endif

#if 0
class String
{
public:
    String(const char* str)
        : m_str(new char[strlen(str)+1])
    {
        strcpy(m_str, str);
    }

    String(const String& s)
        : m_str(nullptr)
    {
        String temp(s.m_str);
        swap(m_str, temp.m_str);
    }

    // 缺点: 自己给自己赋值
    String& operator=(String s)
    {
        swap(this->m_str, s.m_str);
        return *this;
    }
    //String& operator=(const String& s)
    //{
    //    if (this != &s)
    //    {
    //        String temp(s.m_str);
    //        swap(this->m_str, temp.m_str);
    //    }
    //    return *this;
    //}

    ~String()
    {
        if (m_str)
        {
            delete[] m_str;
            m_str = nullptr;
        }
    }

    char* c_str()
    {
        return m_str;
    }
private:
    char* m_str;
};
#endif
