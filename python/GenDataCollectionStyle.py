#!/usr/bin/python
# encoding: utf-8
__author__ = 'bugcode'

import sys

class GenInitDefine(object):
    # 源字符串
    __src_str = ""
    # 最终生成的字符串
    __dst_str = ""
    # 分隔符
    __str_sep = " "
    # 可选，默认是数组形式
    __gen_style = "Array"

    # 每种语言对应不同的数组前后的括号
    __lang_array_style = {"Swift":"[  ]", "Cpp":"{  }", "Python":"[  ]"}

    def __init__(self, src_str = "", str_sep = " ", gen_style = "Array"):
        self.__src_str = src_str
        self.__str_sep = str_sep
        self.__gen_style = gen_style
    
    def getDestStr(self):
        return self.__dst_str

    # 根据不同的语言设置获取前缀括号与后缀括号
    def getLangPrefix(self, lang):
        if not self.__lang_array_style[lang]:
            return None
        return self.__lang_array_style[lang].split()[0]

    # 获取后缀括号
    def getLangSuffix(self, lang):
        if not self.__lang_array_style[lang]:
            return None
        return self.__lang_array_style[lang].split()[1]

    # 获取所有已有定义语言的名称
    def getAllLangName(self):
        return self.__lang_array_style.keys()
		
    # 生成最终所有语言的数组形式的列表
    def genAllArrayStyleDefine(self):
        names = self.getAllLangName()
        if names == None:
            return
        dict = {}
        for index in range(len(names)):
            # 生成所有已定义语言的格式化形式
            name = names[index]
            title = name + "[" + self.__gen_style + "]"
            genRes = self.genArrayStyle(name)
            dict[name] = genRes
        return dict

    # 为方便扩展，这里对每个不同语言使用不同的函数进行生成
    def genArrayStyle(self, lang):
        tmpList = self.__src_str.split(self.__str_sep)
        # 获取前缀括号
        prefix = self.getLangPrefix(lang)
        # 获取后缀括号
        suffix = self.getLangSuffix(lang)
        if not prefix or not suffix:
            return None

        # 生成Swift形式的数组 ["1", "2"]
        final_str = prefix
        for index in range(len(tmpList)):
            comma = "" if (index == len(tmpList) - 1) else ", "
            final_str += "\"" + tmpList[index] + "\"" + comma 

        final_str += suffix	
        return final_str
        

# 命令行参数都齐全的情况下
if len(sys.argv) == 2:
    #print("argv1 = " + sys.argv[1] + " argv2 = " + sys.argv[2])
    gid = GenInitDefine(sys.argv[1], " ")
elif len(sys.argv) == 3:
    gid = GenInitDefine(sys.argv[1], sys.argv[2])
            
print(gid.genAllArrayStyleDefine())
