
#coding=utf-8

import os.path
import zipfile
import shutil
import ctypes,sys

# 标准输入，输出，错误流
STD_INPUT_HANDLE = -10
STD_OUTPUT_HANDLE = -11
STD_ERROR_HANDLE = -12

# 蓝色
FOREGROUND_BLUE = 0x09
# 绿色
FOREGROUND_GREEN = 0x0a
# 天蓝色
FOREGROUND_SKYBLUE = 0x0b
# 红色
FOREGROUND_RED = 0x0c

# 输出流handler
std_out_handle = ctypes.windll.kernel32.GetStdHandle(STD_OUTPUT_HANDLE)

# 压缩指定文件夹下文件到指定zip文件�?
def zip_dir(dirname,zipfilename):
    filelist = []
    if os.path.isfile(dirname):
        filelist.append(dirname)
    else :
        for root, dirs, files in os.walk(dirname):
            for name in files:
                filelist.append(root + "/" + name)

    zf = zipfile.ZipFile(zipfilename, "w", zipfile.zlib.DEFLATED)
    for tar in filelist:
        arcname = tar[len(dirname):]
        #print arcname
        zf.write(tar,arcname)
    zf.close()


# 解压指定zip文件到指定文件夹�?
def unzip_file(zipfilename, unziptodir):
    if not os.path.exists(unziptodir): os.makedirs(unziptodir, 0777)
    zfobj = zipfile.ZipFile(zipfilename)
    for name in zfobj.namelist():
        name = name.replace('\\','/')

        if name.endswith('/'):
            os.makedirs(unziptodir + "/" + name, 0777)
        else:
            ext_filename = unziptodir + "/" + name
            ext_dir = os.path.dirname(ext_filename)
            if not os.path.exists(ext_dir) : os.makedirs(ext_dir, 0777)
            outfile = open(ext_filename, 'wb')
            outfile.write(zfobj.read(name))
            outfile.close()

# 遍历指定文件夹下的zip文件
def traverse_dir(dirname):
    cnt = 0
    for lists in os.listdir(dirname):
        path = os.path.join(dirname, lists)
        #print path
        if lists.endswith('.zip'):
            temp_dir = path[:-4]
            temp_dir_name = lists[:-4]
            # 解压到与zip文件名相同的临时文件夹中
            unzip_file(path, temp_dir)
            if os.path.exists(temp_dir):
                lib_dir = temp_dir + "/root/lib"
                if os.path.exists(lib_dir):
                    for f in os.listdir(lib_dir):
                        if f != "armeabi":
                            final_path = lib_dir + "/" + f
                            print("== deleting " + final_path + "==")
                            shutil.rmtree(final_path)

            # 重新压缩这个目录
            if os.path.exists(path):
                os.remove(path)
                zip_dir(temp_dir_name, temp_dir_name + ".zip")
                #print("\033[1;34;36m%s\033[0m \033[1;31;40m%s\033[0m \033[1;32;38m%s\033[0m " % ("Repack " , "[ "+temp_dir_name+".zip ]", ">>> Completed"))
                print("Repack " + "[ " + temp_dir_name + ".zip ] successful")
                cnt += 1
                if os.path.exists(temp_dir):
                    shutil.rmtree(temp_dir)
    #print ("TOTAL REPACK \033[1;31;40m%s\033[0m ZIP FILES" % str(cnt))
    print("Total repack " + str(cnt) + " zip files")

if __name__ == '__main__':
    traverse_dir(r'./')
    os.system("pause")
