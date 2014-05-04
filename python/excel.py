#coding=utf-8
import xlrd
import sys
import types
import getopt

#设置字符编码
reload(sys)
sys.setdefaultencoding("utf-8")
xlrd.Book.encoding = "gbk"

class ExcelToPlist(object):
    __rows = 0;
    __cols = 0;
    __input_file_name = ""
    __output_file_name = ""
    __xml_head = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    __xml_doc = "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
    __plist_ver = "<plist version=\"1.0\">\n<dict>"
    __config_file = "xml_config.conf"

    #从配置文件中读取xml前置信息，plist版本信息，及xml_doc信息
    def read_xml_config(self, config_file):
        pass
    def read_xls_file(self, which_sheet = 0):
        if not self.__input_file_name:
            print("Fatal Error: need a xls file name, please check usage.")
            sys.exit(-1)
        excel_data = xlrd.open_workbook(self.__input_file_name)
        self.__sheet_contents = excel_data.sheets()[which_sheet]
        #读取行列数与标头信息
        if self.__sheet_contents:
            self.__rows = self.__sheet_contents.nrows
            self.__cols = self.__sheet_contents.ncols
            self.__title_data = self.__sheet_contents.row_values(0)
    
    #写入plist中
    def write_to_plist(self):
        if not self.__output_file_name:
            self.__output_file_name = \
            self.__input_file_name[0:self.__input_file_name.find('.') + 1] + "plist"

        fp = open(self.__output_file_name, "w")

        try:

            fp.writelines([self.__xml_head, self.__xml_doc, self.__plist_ver])
            for i in range(1, self.__rows):
                datas = self.__sheet_contents.row_values(i)
                fp.write("\n<key>" + str(int(datas[0])) + "</key>"+ "\n<dict>")

                for j in range(0, self.__cols):
                    key = "\n\t<key>" + self.__title_data[j] + "</key>\n"
                    #如果读取的带有小数点,这里处理抹掉
                    if (type(datas[j]) is types.FloatType):
                        datas[j] = int(datas[j])

                    string = "\t<string>" + str(datas[j]) + "</string>"
                    fp.writelines([key, string])
                fp.write("\n</dict>")

            fp.write("\n</dict>\n</plist>")
            print "Write data to [%s] success.[%d] rows insert." % (self.__output_file_name, self.__rows - 1)
 
        finally:
            fp.close()
    #读取命令行参数 
    def get_command_opt(self):
        opt, args = getopt.getopt(sys.argv[1:], 'hi:o:')
        for op, arg in opt:
            if op in ('-i'):
                self.__input_file_name = arg
            if op in ('-o'):
                self.__output_file_name = arg
            if op in ('-h'):
                print ("Like this: python excel.py -i xxx.xls -o xxx.plist\n"
                        "-i: input file name [xls file]\n"
                        "-o: output file name [plist file]\n")
                sys.exit(-1)

plist = ExcelToPlist()
plist.get_command_opt()
plist.read_xls_file()
plist.write_to_plist()
