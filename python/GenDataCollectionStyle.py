#!/usr/bin/python
# encoding: utf-8
__author__ = 'bugcode'

import sys
import re
from workflow import Workflow, ICON_WEB, web

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
        return final_str.decode("utf8")


def main(wf):
    dict = {}
    ## 命令行参数都齐全的情况下
    if len(sys.argv) == 2:
        ##print("argv1 = " + sys.argv[1] + " argv2 = " + sys.argv[2])
        cmd = sys.argv[1].strip().split("->")
        cmd2 = " "
        if len(cmd) == 2:
            cmd2 = " " if cmd[1].strip() == "" else cmd[1].strip()

        gid = GenInitDefine(cmd[0].strip(), cmd2)
        dict = gid.genAllArrayStyleDefine()
        
    
    cnt = 1
    keys = dict.keys()
    for index in range(len(keys)):
        #print(keys[index])
        wf.add_item(keys[index], dict[keys[index]], arg=dict[keys[index]], uid=str(cnt), valid=True)
        cnt += 1
    # 将最终结果以xml格式反馈给控制台或者Alfred
    wf.send_feedback()


if __name__ == '__main__':
    wf = Workflow()
    sys.exit(wf.run(main))

