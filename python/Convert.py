#coding=utf-8
'''
#=============================================================================
#     FileName: Convert.py
#         Desc: 
#       Author: bugcode
#        Email: bugcodingt@163.com
#     HomePage: 
#      Version: 0.0.1
#   LastChange: 2014-05-29 16:15:31
#=============================================================================
'''
import sys
import os
import getopt

#png到etc pvr到png
class Convert(object):
    __pic_dirs = ''
    __pic_ext = []
    __which_convert = ""

    def check_extend_name(self, file_name, *extend_name):
        array = map(file_name.endswith, extend_name)

        return any(array)

    def get_command(self, f_name):
        command = ""
        #pvr.ccz convert to png
        if self.__which_convert == "pvr2png":
            new_file_name = f_name[:-7] + ("png")
            command = order = "TexturePacker "+f_name+" --sheet "+new_file_name + " --texture-format png --disable-rotation --trim-mode Trim --allow-free-size"
        #png convert to pkm
        if self.__which_convert == "png2pkm":
            path = os.path.split(f_name)[0]
            command = "etcpack "+ f_name + " " + path +" -c etc1 -f RGB -aa"
        
        return command


    def trans_file(self, file_name):
        if os.path.isfile(file_name):
            #excute python
            command = self.get_command(file_name)
            print "[debug]: " + command
            cmd = command.split(" ")[0]
            if self.check_cmd_in_path(cmd) == 1 or self.check_cmd_in_path(cmd) == 2:
                os.system(command)
            else:
                print ("command [" + cmd + "] not in system path")
                sys.exit(0)
            
    def list_file_and_trans(self, file_name):
        if os.path.isdir(file_name):
            files = os.listdir(file_name)
            for l in files:
                #build path for current file
                new_file_path = file_name + os.sep + l

                if os.path.isdir(new_file_path):
                    self.list_file_and_trans(new_file_path)
                else:
                    if self.check_extend_name(new_file_path, self.__pic_ext):
                        self.trans_file(new_file_path)


    def get_dirs(self):
        print "__pic_dirs: ", self.__pic_dirs
        return self.__pic_dirs

    def compute_all_dir(self, *file_groups):
        for file in file_groups:
            print file
            self.list_file_and_trans(file)


    def in_sys_path(self, command):
        cmdlist = []
        xtpath = os.getenv('PATH')
        xtpathlist = xtpath.split(':')
        for directory in xtpathlist:
            x = directory + '/' + command
            cmdlist.append(x)
        #print cmdlist
        for cmd in cmdlist:
            #os.access判断文件是否有可执行权限
            if os.access(cmd, os.X_OK):
                #有执行权限返回值为2
                return 2

    def check_cmd_in_path(self, command):
        #判断命令参数为/usr/bin/python这种的。
        if command.startswith('/'):
            if os.access(command, os.X_OK):
                return 1  
        else:
            return self.in_sys_path(command)     

    def get_convert_opt(self):
        
        try:
            opts, args = getopt.getopt(sys.argv[1:], "hc:d:", ["help", "convert" ,"dir"])
            if (opts == []):
                print ("[Convert Usage]: Convert.py -d ~/six/pics -c pvr2png")
                sys.exit(0)

                
            for opt, arg in opts:
                if opt in ("-h", "--help"):
                    #print usage
                    print ("[Convert Usage]: Convert.py -d ~/six/pics -c pvr2png")
                if opt in ("-d", "--dir"):
                    self.__pic_dirs = arg
                if opt in ("-c", "--convert"):
                    self.__which_convert = arg
                    if self.__which_convert == "pvr2png":
                        self.__pic_ext = "pvr.ccz"
                    elif self.__which_convert == "png2pkm":
                        self.__pic_ext = "png"
                    else:
                        print ("[debug]: param error")
                        sys.exit(0)
        except getopt.GetoptError:
            #output usage
            print ("[Convert Usage]: Convert.py -d ~/six/pics -c pvr2png")
        

convert = Convert()
convert.get_convert_opt()
convert.compute_all_dir(convert.get_dirs())


