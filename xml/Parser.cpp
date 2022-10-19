#include "Parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
using namespace pan::xml;

// 构造函数
Parser::Parser() : m_str(""), m_idx(0)
{
}

// 载入xml文件
bool Parser::load_file(const string & filename)
{
    // 判断读取文件是否失败
    ifstream fin(filename);
    if (fin.fail())
    {
        return false;
    }

    // 读取xml文件到缓存中
    stringstream ss;
    ss << fin.rdbuf();
    m_str = ss.str();
    m_idx = 0;

    return true;
}

// 载入xml字符串
bool Parser::load_string(const string &str)
{
    m_str = str;
    m_idx = 0;

    return true;
}

// 跳过空格
void Parser::skip_white_space()
{
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
    {
        m_idx++;
    }
}

// 解析器
Xml Parser::parse()
{
    // 解析声明
    skip_white_space();
    if (m_str.compare(m_idx, 5, "<?xml") == 0)
    {
        if (!parse_declaration())
        {
            throw std::logic_error("parse declaration error");
        }
    }

    // 解析注释
    skip_white_space();
    while (m_str.compare(m_idx, 4, "<!--") == 0)
    {
        if (!parse_comment())
        {
            throw std::logic_error("parse comment error");
        }
        skip_white_space();
    }

    // 解析节点
    if (m_str[m_idx] == '<' && (isalpha(m_str[m_idx + 1]) || m_str[m_idx + 1] == '_'))
    {
        return parse_element();
    }

    throw std::logic_error("parse element error");
}

// 解析声明
bool Parser::parse_declaration()
{
    // 判断声明的开头
    if (m_str.compare(m_idx, 5, "<?xml") != 0)
    {
        return false;
    }

    m_idx += 5;
    // 寻找声明的结尾，判断是否超出最大值
    size_t pos = m_str.find("?>", m_idx);
    if (pos == std::string::npos)
    {
        return false;
    }

    m_idx = pos + 2;
    return true;
}

// 解析注释
bool Parser::parse_comment()
{
    // 判断注释的开头
    if (m_str.compare(m_idx, 4, "<!--") != 0)
    {
        return false;
    }

    // 寻找注释的结尾，并判断是否超出最大值
    m_idx += 4;
    size_t pos = m_str.find("-->", m_idx);
    if (pos == std::string::npos)
    {
        return false;
    }

    m_idx = pos + 3;
    return true;
}

// 解析节点
Xml Parser::parse_element()
{
    Xml elem;

    // 节点的<括号
    m_idx++; 
    skip_white_space();

    // 节点名称
    const string &name = parse_element_name();
    elem.name(name);

    // 节点属性
    while (m_str[m_idx] != '\0')
    {
        // 判断节点是否为结尾 />
        skip_white_space();
        if (m_str[m_idx] == '/')
        {
            if (m_str[m_idx + 1] == '>')
            {
                m_idx += 2;
                break;
            }
            else
            {
                throw std::logic_error("xml empty element is error");
            }
        }
        else if (m_str[m_idx] == '>')                                   // 判断是否为 >
        {
            m_idx++;
            // 获得节点的内容
            string text = parse_element_text();
            if (text != "")
            {
                elem.text(text);
            }
        }
        else if (m_str[m_idx] == '<')                                   // 节点结尾的 <
        {
            if (m_str[m_idx + 1] == '/')
            {
                // find the end tag
                string end_tag = "</" + name + ">";
                size_t pos = m_str.find(end_tag, m_idx);
                if (pos == std::string::npos)
                {
                    throw std::logic_error("xml element " + name + " end tag not found");
                }

                m_idx = (pos + end_tag.size());
                break;
            }
            else if (m_str.compare(m_idx, 4, "<!--") == 0)
            {
                // 判断是为注释
                if (!parse_comment())
                {
                    throw std::logic_error("xml comment is error");
                }
            }
            else
            {
                // parse child element
                elem.append(parse_element());
            }
        }
        else
        {
            // 解析节点的属性
            // 键
            string key = parse_element_attr_key();
            skip_white_space();
            if (m_str[m_idx] != '=')
            {
                throw std::logic_error("xml element attr is error" + key);
            }

            // 值
            m_idx++;
            skip_white_space();
            string val = parse_element_attr_val();
            elem.attr(key, val);
        }
    }

    return elem;

    /*
    {
        
        else if (m_str[m_idx] == '>')
        {
            m_idx++;
            string text = parse_element_text();
            if (text != "")
            {
                elem.text(text);
            }
        }
        else if (m_str[m_idx] == '<')
        {
            if (m_str[m_idx+1] == '/')
            {
                // find the end tag
                string end_tag = "</" + name + ">";
                size_t pos = m_str.find(end_tag, m_idx);
                if (pos == std::string::npos)
                {
                    throw std::logic_error("xml element " + name + " end tag not found");
                }
                m_idx = (pos + end_tag.size());
                break;
            }
            else if (m_str.compare(m_idx, 4, "<!--") == 0)
            {
                // parse xml comment
                if (!parse_comment())
                {
                    throw std::logic_error("xml comment is error");
                }
            }
            else
            {
                // parse child element
                elem.append(parse_element());
            }
        }
        else
        {
            // parse elem's attr
            string key = parse_element_attr_key();
            skip_white_space();
            if (m_str[m_idx] != '=')
            {
                throw std::logic_error("xml element attr is error" + key);
            }
            m_idx++;
            skip_white_space();
            string val = parse_element_attr_val();
            elem.attr(key, val);
        }
    }
    return elem;*/
}

// 解析节点名称
string Parser::parse_element_name()
{
    int pos = m_idx;
    // 判断字符是否为字母，即判断名称的开头字符的类型
    if (isalpha(m_str[m_idx]) || (m_str[m_idx] == '_'))
    {
        m_idx++;
        while (isalnum(m_str[m_idx]) || (m_str[m_idx] == '_') || (m_str[m_idx] == '-') || (m_str[m_idx] == ':') || (m_str[m_idx] == '.'))
        {
            m_idx++;
        }
    }

    return m_str.substr(pos, m_idx - pos);
}

// 解析节点内容
string Parser::parse_element_text()
{
    int pos = m_idx;
    while (m_str[m_idx] != '<')
    {
        m_idx++;
    }

    return m_str.substr(pos, m_idx - pos);
}

// 解析节点属性键
string Parser::parse_element_attr_key()
{
    int pos = m_idx;
    if (isalpha(m_str[m_idx]) || (m_str[m_idx] == '_'))
    {
        m_idx++;
        while (isalnum(m_str[m_idx]) || (m_str[m_idx] == '_') || (m_str[m_idx] == '-') || (m_str[m_idx] == ':'))
        {
            m_idx++;
        }
    }

    return m_str.substr(pos, m_idx - pos);
}

// 解析节点属性值
string Parser::parse_element_attr_val()
{
    if (m_str[m_idx] != '"')
    {
        throw std::logic_error("xml element attr value should be in double quotes");
    }

    m_idx++;
    int pos = m_idx;
    while (m_str[m_idx] != '"')
    {
        m_idx++;
    }

    m_idx++;
    return m_str.substr(pos, m_idx - pos - 1);
}
