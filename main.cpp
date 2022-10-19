#include <iostream>
#include <vector>
#include "Xml.h"
using namespace pan::xml;

int main()
{
    // 添加
    Xml growlie;
    growlie.load("../growlie.xml");
    std::cout << growlie.str() << std::endl;

    // 保存
    string tempSave = growlie.str();
    growlie.save("../growlie1.xml");

    return 0;
}