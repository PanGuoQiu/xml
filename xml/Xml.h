#pragma once

#include <string>
#include <map>
#include <list>
using namespace std;

namespace pan
{
    namespace xml
    {
        // 变量类
        class Value
        {
            public:
                // 构造函数和析构函数
                Value();
                Value(bool value);
                Value(int value);
                Value(double value);
                Value(const char * value);
                Value(const string & value);
                ~Value();

                // 重载=运算符
                Value & operator=(bool value);
                Value & operator=(int value);
                Value & operator=(double value);
                Value & operator=(const char * value);
                Value & operator=(const string & value);
                Value & operator=(const Value & value);

                // 重载==和!=运算符
                bool operator==(const Value & other);
                bool operator!=(const Value & other);

                // 类型转换函数
                operator bool();
                operator int();
                operator double();
                operator string();

            private:
                string m_value;
        };

        // xml结构与C++的转换类
        class Xml
        {
            public:
                // 构造函数
                Xml();
                Xml(const char * name);
                Xml(const string & name);

                // 拷贝构造函数
                Xml(const Xml & other);

                // 释放内存
                void clear();

                // 获得节点名称和设置节点名称
                string name() const;
                void   name(const string & name);

                // 获得节点内容和设置节点内容
                string text() const;
                void   text(const string & text);

                // 获得节点属性和设置节点属性
                Value & attr(const string & key);
                void   attr(const string & key, const Value & val);

                // 序列化
                string str() const;

                // 添加子节点
                void append(const Xml & child);

                // 通过下标索引获得子节点
                Xml & operator[](int index);
                // 根据节点名称，返回节点
                Xml & operator[](const char * name);
                Xml & operator[](const string & name);

                // 删除节点
                void remove(int idnex);                         // 通过下标的方式删除子节点
                void remove(const char * name);
                void remove(const string & name);

                // 重载=运算符
                Xml & operator=(const Xml & other);

                // 定义一个迭代器，遍历子节点
                typedef std::list<Xml>::iterator iterator;
                iterator begin()
                {
                    return m_child->begin();
                }
                iterator end()
                {
                    return m_child->end();
                }
                iterator erase(iterator it)
                {
                    it->clear();
                    return m_child->erase(it);
                }

                // 获得子节点的个数
                int size() const
                {
                    return m_child->size();
                }

                // 载入xml文件
                bool load(const string &filename);              // 载入xml文件
                bool save(const string &filename);              // 保存xml文件
                bool parse(const string &str);                  // 解析xml字符串

            private:
                string * m_name;                                // 节点名称
                string * m_text;                                // 节点内容
                std::map<string, Value> * m_attrs;              // 节点属性
                std::list<Xml> * m_child;                       // 子节点
        };
    }
}