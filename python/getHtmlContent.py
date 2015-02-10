'''
#=============================================================================
#     FileName: getHtml.py
#         Desc: read series html file from internet and save to local
#       Author: zx
#        Email: zx_start@163.com
#     HomePage: bugcode.net 
#      Version: 0.0.1
#   LastChange: 2014-09-22 16:58:35
#=============================================================================
'''
import urllib

def getPageConent(url):
    page = urllib.urlopen(url)
    htmlCnt = page.read()
    
    #replace some css file path to point local
    htmlCnt = htmlCnt.replace("/static/styles/", "./")
    htmlCnt = htmlCnt.replace("/static/scripts/", "./")
    htmlCnt = htmlCnt.replace("/chapters/", "./")

    #read extra css file for saving real html file
    fp = open("extra", "r")
    css = fp.read()
    htmlCnt = htmlCnt.replace("</head>", css)
    fp.close()

    return htmlCnt

def writePageCntToFile(htmlCnt, fileName):
    fp = open(fileName, "w")
    fp.writelines(htmlCnt)
    fp.close()

def constructPageLink(chapters):
    chapStr = str(chapters)
    if len(chapStr) < 2:
        chapStr = "0" + chapStr + ".html"
    else:
        chapStr += ".html"
    
    return "http://learnvimscriptthehardway.onefloweroneworld.com/chapters/" + chapStr

for i in range(0, 57):
    writePageCntToFile(getPageConent(constructPageLink(i)), \
            (len(str(i)) < 2 and "0" + str(i) or str(i)) + ".html")

