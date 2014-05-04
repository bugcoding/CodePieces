#coding=utf-8
import xlrd
import sys
import types

reload(sys)
sys.setdefaultencoding("utf-8")
xlrd.Book.encoding = "gbk"




excel_data = xlrd.open_workbook("pieces.xls")
data_table = excel_data.sheets()[0]

#第一行标头
title = data_table.row_values(0)
cols = data_table.ncols
rows = data_table.nrows

#前置内容(xml标准)
#改为从文件读取文件头
xml_declare = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
pre_xml_content = "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
plist_version = "<plist version=\"1.0\">\n<dict>"



fp = open("./excel.xml", "w")

fp.write(xml_declare)
fp.write(pre_xml_content)
fp.write(plist_version)

for i in range(1, rows):
    datas = data_table.row_values(i)
    fp.write("\n<key>" + str(int(datas[0])) + "</key>"+ "\n<dict>")

    for j in range(0, cols):
        key = "\n\t<key>" + title[j] + "</key>\n"

        #如果读取的带有小数点,这里处理抹掉
        if (type(datas[j]) is types.FloatType):
            datas[j] = int(datas[j])

        string = "\t<string>" + str(datas[j]) + "</string>"
        fp.write(key)
        fp.write(string)
    fp.write("\n</dict>")

fp.write("\n</dict>\n</plist>")







