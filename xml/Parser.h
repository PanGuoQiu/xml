#pragma once

#include "Xml.h"
#include <string>
using namespace std;

namespace pan
{
    namespace xml
    {
        // 解析器类
        class Parser
        {
            public:
                // 构造函数
                Parser();

                // 载入xml文件和在xml字符串
                bool load_file(const string &filename);
                bool load_string(const string &str);

                // 解析xml文件
                Xml parse();

            private:
                void skip_white_space();                                // 跳过空格
                bool parse_declaration();                               // 解析声明
                bool parse_comment();                                   // 解析注释
                Xml  parse_element();                                   // 解析节点

                string parse_element_name();                            // 解析节点名称
                string parse_element_text();                            // 解析节点内容
                string parse_element_attr_key();                        // 解析节点属性键
                string parse_element_attr_val();                        // 解析节点属性值

            private:
                string m_str;                                           // xml文件读取到m_str中
                int m_idx;                                              // 下标、即指向m_str第idx的位置
        };
    }
}
