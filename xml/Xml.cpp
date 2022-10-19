#include "Xml.h"
#include "Parser.h"
#include <fstream>
#include <sstream>
using namespace std;
using namespace pan::xml;

// 变量类
// 构造函数
Value::Value()
{

}

Value::Value(bool value)
{
    *this = value;
}

Value::Value(int value)
{
    *this = value;
}

Value::Value(double value)
{
    *this = value;
}

Value::Value(const char * value) : m_value(value)
{

}

Value::Value(const string & value) : m_value(value)
{

}

// 析构函数
Value::~Value()
{
    
}

// 重载=运算符
Value & Value::operator=(bool value)
{
    m_value = value ? "true" : "false";
    return *this;
}

Value & Value::operator=(int value)
{
    stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value & Value::operator=(double value)
{
    stringstream ss;
    ss << value;
    m_value = ss.str();
    return *this;
}

Value & Value::operator=(const char * value)
{
    m_value = value;
    return *this;
}

Value & Value::operator=(const string & value)
{
    m_value = value;
    return *this;
}

Value & Value::operator=(const Value & value)
{
    m_value = value.m_value;
    return *this;
}

// 重载==和!=运算符
bool Value::operator==(const Value & value)
{
    return m_value == value.m_value;
}

bool Value::operator!=(const Value & value)
{
    return m_value != value.m_value;
}

// 类型转换运算符
Value::operator bool()
{
    if (m_value == "true")
        return true;
    else if (m_value == "false")
        return false;

    return false;
}

Value::operator int()
{
    // 将字符串转换为整型
    return std::atoi(m_value.c_str());
}

Value::operator double()
{
    // 将字符串转换为浮点型
    return std::atof(m_value.c_str());
}

Value::operator string()
{
    return m_value;
}

// 构造函数
Xml::Xml() : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr)
{
}

Xml::Xml(const char * name) : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr)
{
    m_name = new string(name);
}

Xml::Xml(const string & name) : m_name(nullptr), m_text(nullptr), m_attrs(nullptr), m_child(nullptr)
{
    m_name = new string(name);
}

// 拷贝构造函数
Xml::Xml(const Xml & other)
{
    // 浅拷贝，直接复制指针地址
    m_name = other.m_name;
    m_text = other.m_text;
    m_attrs = other.m_attrs;
    m_child = other.m_child;
}

// 释放内存
void Xml::clear()
{
    // 节点名称
    if (m_name != nullptr)
    {
        delete m_name;
        m_name = nullptr;
    }

    // 节点内容
    if (m_text != nullptr)
    {
        delete m_text;
        m_text = nullptr;
    }

    // 节点属性
    if (m_attrs != nullptr)
    {
        delete m_attrs;
        m_attrs = nullptr;
    }

    // 子节点，需要考虑：字节点里面的子节点
    if (m_child != nullptr)
    {
        for (auto it = m_child->begin(); it != m_child->end(); it++)
        {
            it->clear();
        }

        delete m_child;
        m_child = nullptr;
    }
}

// 获得节点名称
string Xml::name() const
{
    if (m_name == nullptr)
    {
        return "";
    }

    return *m_name;
}

// 设置节点名称
void Xml::name(const string & name)
{
    if (m_name != nullptr)
    {
        delete m_name;
        m_name = nullptr;
    }

    m_name = new string(name);
}

// 获得节点内容
string Xml::text() const
{
    if (m_text == nullptr)
    {
        return "";
    }

    return *m_text;
}

// 设置节点内容
void Xml::text(const string & text)
{
    if (m_text != nullptr)
    {
        delete m_text;
        m_text = nullptr;
    }

    m_text = new string(text);
}

// 获得节点属性
Value & Xml::attr(const string & key)
{
    if (m_attrs == nullptr)
    {
        m_attrs = new std::map<string, Value>();
    }

    return (*m_attrs)[key];
}

// 设置节点属性
void Xml::attr(const string & key, const Value & val)
{
    if (m_attrs == nullptr)
    {
        m_attrs = new std::map<string, Value>();
    }

    (*m_attrs)[key] = val;
}

// 序列化
string Xml::str() const
{
    if (m_name == nullptr)
    {
        return "";
    }

    stringstream os;
    // 节点头部
    os << "<";
    os << *m_name;
    // 判断节点是否有属性
    if (m_attrs != nullptr)
    {
        for (auto it = m_attrs->begin(); it != m_attrs->end(); it++)
        {
            os << " " << it->first << "=\"" << (string)(it->second) << "\"";
        }
    }
    os << ">";

    // 判断是否有子节点
    if (m_child != nullptr)
    {
        for (auto it = m_child->begin(); it != m_child->end(); it++)
        {
            os << it->str();
        }
    }

    // 判断节点是否有内容
    if (m_text != nullptr)
    {
        os << *m_text;
    }

    // 节点尾部
    os << "</" << *m_name << ">";

    // 返回序列化后的结束
    return os.str();
}

// 添加子节点
void Xml::append(const Xml & child)
{
    // 判断子节点是否为空
    if (m_child == nullptr)
    {
        // 如果为空，则new一个
        m_child = new std::list<Xml>();
    }

    m_child->push_back(child);
}

// 通过下标索引的方式获得子节点
Xml & Xml::operator[](int index)
{
    if (index < 0)
    {
        throw std::logic_error("index less than zero");
    }

    if (m_child == nullptr)
    {
        m_child = new std::list<Xml>();
    }

    // 遍历下标，找到index对应Xml
    int size = m_child->size();
    if (index >= 0 && index < size)
    {
        auto it = m_child->begin();
        for (int i = 0; i < index; i++)
        {
            it++;
        }

        return (*it);
    }

    // index > size，则需要扩容
    if (index >= size)
    {
        for (int i = size; i <= index; i++)
        {
            m_child->push_back(Xml());
        }
    }
    // 返回最后一个
    return m_child->back();
}

// 根据节点名，返回节点
Xml & Xml::operator[](const char * name)
{
    return (*this)[string(name)];
}

Xml & Xml::operator[](const string & name)
{
    // 判断子节点是否为空
    if (m_child == nullptr)
    {
        // 如果为空，则创建
        m_child = new std::list<Xml>();
    }

    // 遍历子节点
    for (auto it = m_child->begin(); it != m_child->end(); it++)
    {
        if (it->name() == name)
        {
            return (*it);
        }
    }

    // 遍历后，没找到，则创建
    m_child->push_back(Xml(name));
    return m_child->back();
}

// 通过下标删除子节点
void Xml::remove(int index)
{
    if (m_child == nullptr)
    {
        return;
    }

    int size = m_child->size();
    if (index < 0 || index >= size)
    {
        return;
    }

    auto it = m_child->begin();
    for (int i = 0; i < index; i++)
    {
        it++;
    }

    // 释放内存
    it->clear();
    m_child->erase(it);
}

// 通过节点名称来删除子节点
void Xml::remove(const char *name)
{
    this->remove(string(name));
}

void Xml::remove(const string & name)
{
    if (m_child == nullptr)
    {
        return;
    }

    // 删除所有名称相同的节点
    for (auto it = m_child->begin(); it != m_child->end(); )
    {
        if (it->name() == name)
        {
            // 释放内存，并返回下一个迭代器节点
            it->clear();
            it = m_child->erase(it);
        }
        else
        {
            it++;
        }
    }
}

// 重载=运算符
Xml & Xml::operator=(const Xml & other)
{
    clear();
    m_name = other.m_name;
    m_text = other.m_text;
    m_attrs = other.m_attrs;
    m_child = other.m_child;

    return *this;
}

// 载入xml文件
bool Xml::load(const string &filename)
{
    Parser p;
    if (!p.load_file(filename))
    {
        return false;
    }

    *this = p.parse();
    return true;
}

// 保存为xml文件
bool Xml::save(const string &filename)
{
    ofstream fout(filename);
    if (fout.fail())
    {
        return false;
    }

    fout << str();
    fout.close();
    return true;
}

// xml字符串解析
bool Xml::parse(const string &str)
{
    Parser p;
    if (!p.load_string(str))
    {
        return false;
    }

    *this = p.parse();
    return true;
}
