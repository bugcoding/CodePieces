#!/usr/bin/python
# encoding: utf-8
__author__ = 'bugcode'

str = "子、鼠、丑、牛、虎、兔、龙、蛇、马、羊、猴、鸡"

class GenInitDefine(object):
    # 源字符串
    __src_str = ""
    # 最终生成的字符串
    __dst_str = ""
    # 分隔符
    __str_sep = " "
    # 可选，默认是数组形式
    __gen_style = "array"

    def __init__(self, src_str = "", str_sep = " ", gen_style = "array"):
        self.__src_str = src_str
        self.__str_sep = str_sep
        self.__gen_style = gen_style
    
    def getDestStr(self):
        return self.__dst_str

    def genSwiftStyle(self):
        tmpList = self.__src_str.split(self.__str_sep)
        # 生成Swift形式的数组 ["1", "2"]
        self.__dst_str = "["
        for index in range(len(tmpList)):
            comma = "" if (index == len(tmpList) - 1) else ", "
            self.__dst_str += "\"" + tmpList[index] + "\"" + comma 

        self.__dst_str += "]"
        

gid = GenInitDefine(str, "、")
gid.genSwiftStyle()
print(gid.getDestStr())
            

