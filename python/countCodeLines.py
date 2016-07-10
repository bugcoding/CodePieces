import sys
import os
import getopt

# ͳ��Ŀ¼��������

class CountFileLines(object):
    __code_lines = 0
    __code_dirs = ''
    __code_ext = []
    #����׺��
    def check_extend_name(self, file_name, *extend_name):
        array = map(file_name.endswith, extend_name)

        return any(array) 

    #���㵥���ļ�������
    def count_file_lines(self, file_name):
        if os.path.isfile(file_name):
            file_handle = open(file_name, "r")
            file_contents = file_handle.readlines()
            file_handle.close()

            for line in file_contents:
                if line.strip():
                    self.__code_lines += 1


    #�г�һ��Ŀ¼�µ������ļ�
    def list_file_and_count(self, file_name):
        if os.path.isdir(file_name):
            files = os.listdir(file_name)
            for l in files:
                new_file_path = file_name + os.sep + l

                if os.path.isdir(new_file_path):
                    self.list_file_and_count(new_file_path)
                else:
                    if self.check_extend_name(new_file_path, self.__code_ext):
                        self.count_file_lines(new_file_path)

    def get_lines(self):
        return self.__code_lines

    def get_dirs(self):
        return self.__code_dirs
    
    #���㴫������е�Ŀ¼
    def compute_all_dir(self, *file_groups):
        for file in file_groups:
            #print file
            self.list_file_and_count(file)

    #��ȡ�����в���
    def get_some_opt(self):
        opt, args = getopt.getopt(sys.argv[1:], 'd:e:')
        for op, arg in opt:
            if op in ('-d'):
                self.__code_dirs = arg
            if op in ('-e'):
                self.__code_ext = arg


# python count_lines.py -d /your_path/ -e code_file_ext_name
cfl = CountFileLines()
cfl.get_some_opt()
cfl.compute_all_dir(cfl.get_dirs())
print cfl.get_lines()
