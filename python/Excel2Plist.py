# -*- coding: utf-8 -*--
'''
#=============================================================================
#     FileName: Excel2Plist.py
#         Desc: simple create plist file from excel
#       Author: bugcode
#        Email: bugcodingt@gmail.com
#     HomePage: bugcoding.net
#      Version: 0.0.1
#   LastChange: 2014-11-14 14:52:53
#=============================================================================
'''
#引入操作excel model
import xlrd
import sys
import os
import types
import getopt

class Excel2Plist(object):
    #excel 目录
    __excel_dirs = ""
    #plist 目录
    __plist_dirs = ""
    #标准题头xml
    __xml_title_ = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
    #docType 声明
    __doc_type_declare = "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
    #plist 前置标签
    __plist_pre_label = "<plist version=\"1.0\">"
    #plist 封闭标签
    __plist_closed_label = "</plist>"
    #字典前置与封闭标签
    __dict_pre_label = "<dict>"
    __dict_closed_label = "</dict>"
    #key
    __key_pre_label = "<key>"
    __key_closed_label = "</key>"
    #string
    __string_pre_label = "<string>"
    __string_closed_label = "</string>"
    
    
    #确定excel在哪个目录，plist放到哪个目录
    def __init__(self, _excel_dirs = "./excel", _plist_dirs = "./plist"):
        self.__excel_dirs = _excel_dirs
        self.__plist_dirs = _plist_dirs
    
    #操作单个excel文件，转换为plist文件
    def transExcel2Plist(self, _excel_name, _plist_name):
        #读取excel文件数据，_excel_name 指定的文件
        _excel_data = xlrd.open_workbook(_excel_name)
        #读取不到直接返回
        if _excel_data is None :
            return
        #获取一个工作表，默认获取第0个工作表
        _excel_table = _excel_data.sheets()[0]
        if _excel_table is None :
            return
        #创建plist文件
        _plist_fp = open(_plist_name, "w")
        if _plist_fp is None :
            return
        #写入公共信息
        _plist_fp.write(self.__xml_title_ + "\n" + self.__doc_type_declare + "\n" + self.__plist_pre_label + "\n")
        #写入字典前置标签
        _plist_fp.write("\t" + self.__dict_pre_label + "\n")
        
        
        #写入数据
        _excel_rows = _excel_table.nrows
        _excel_cols = _excel_table.ncols
        if _excel_rows == 0 or _excel_cols == 0:
            _plist_fp.close()
            os.remove(_plist_name)
            return
        #第一行数据是用作key的，所以总数据是读取出来的行从第一行开始
        
        for i in range(1, _excel_rows):
            
            #第一个key，根据类型来区分
            _key_info = _excel_table.row(i)[0].value
            if type(_key_info) == types.FloatType:
                _key_info = str(int(_key_info))
            else:
                _key_info = str(_key_info)
            _plist_fp.write("\t\t" + self.__key_pre_label + _key_info + self.__key_closed_label + "\n")
            #key所属下面是一个dict
            _plist_fp.write(self.__dict_pre_label + "\n")
            
            #加入每一列对应的数据
            for j in range(0, _excel_cols):
                #写入key值
                _plist_fp.write("\t\t\t" + self.__key_pre_label + (_excel_table.row(0)[j].value) + self.__key_closed_label + "\n")
                #写入string
                _plist_fp.write("\t\t\t" + self.__string_pre_label)
                #对应的数据信息，如果是中文，做encode处理
                _data_info = _excel_table.row(i)[j].value
                
                if type(_data_info) == types.FloatType:
                    _data_info = str(int(_data_info))
                else:
                    _data_info = _data_info.encode('utf-8')
                
                _plist_fp.write(_data_info)
                _plist_fp.write(self.__string_closed_label + "\n")
            
            _plist_fp.write(self.__dict_closed_label + "\n")
        
        
        #写完数据写入字符封闭标签
        _plist_fp.write("\t" + self.__dict_closed_label + "\n")
        #写入plist封闭标签
        _plist_fp.write(self.__plist_closed_label + "\n")
        _plist_fp.close()
    
    #设置目录
    def setSourceDir(self, _src_excel_dir, _dst_plist_dir):
        self.__excel_dirs = self.getpwd() + os.sep + _src_excel_dir + os.sep
        if not os.path.exists(self.__excel_dirs):
            print ("\033[1;31;40m%s\033[0m" % "[not found [excel] directory on current directory!]")
            sys.exit(0)
        self.__plist_dirs = self.getpwd() + os.sep + _dst_plist_dir + os.sep
        if not os.path.exists(self.__plist_dirs):
            os.makedirs(self.__plist_dirs)

    def getpwd(self):
        if os.getcwd() == sys.path[0]:  # 说明是直接运行py
            return os.getcwd()
        else:  # 说明是pyinstaller1.5打包成exe的文件在运行
            return sys.path[1]
    
    def check_extend_name(self, file_name, *extend_name):
        array = map(file_name.endswith, extend_name)
        return any(array)
    
    #遍历整个文件夹
    def traverseAllExcel2Plist(self):
        #遍历指定的excel文件
        cnt = 0
        for file in os.listdir(self.__excel_dirs):
            if not os.path.isdir(file):
                if self.check_extend_name(file, "xls", "xlsx"):
                    excel_name = self.__excel_dirs + os.sep + file
                    plist_name = self.__plist_dirs + os.sep + file.split(".")[0] + ".plist"
                    cnt += 1
                    self.transExcel2Plist(excel_name, plist_name)
                    print("\033[1;34;36m%s\033[0m \033[1;32;38m%s\033[0m" % ("transform " , "["+file+"]" + " >>> completed"))
        if cnt != 0:
            print ("Success [" + str(cnt) + "]" + " plist")
        else:
            print ("not found xls or xlsx file!")


excel2plist = Excel2Plist()
excel2plist.setSourceDir("excel", "plist")
#excel2plist.transExcel2Plist("nationList.xls", "nationList.plist")
excel2plist.traverseAllExcel2Plist()
