## Lua Note##
<!--March 9, 2015 10:49 PM--Lua.md-->


**Hell World**

1. 直接在命令行运行(前提lua已加入环境变量中):
> \>\> print("Hello, World!")
2. print语句写在文件中，如hello.lua, print("Hello, World!")保存
在文件所在目录打开命令行，然后再执行命令:
> lua hello.lua





**注释**
- 单行注释，二个连续的中划线"--"，一行内二个中划线后的内容就被认为是注释内容
```lua
-- single line comment in lua
```
- 多行注释，以"--[["开头，然后以"]]"结尾的部分都会被认为是多行注释(块注释)
```lua
--[[
muilti line comment in lua
by bugcode
]]
```

**变量定义**
1. __全局变量__
在lua中因为是脚本语言，不需要声明，需要变量的时候直接定义使用(就是需要的时候拿过来用)
不需要类型关键字等信息，定义的这些变量都是全局变量。
```lua
--未加local标识符，下面定义的都是全局变量
var_def_num = 12 --定义了一个数值变量
var_def_str = 'string' --定义了一个字符串
var_def_bool = false --定义了一个布尔类型
```
2. __局部变量__
如果你想定义一下局部变量，需要在变量前加"local"关键字。
```lua
local var_def_loc_num = 20 --定义了一个局部变量
```


**各种类型**

1. __nil__
nil表示没有使用过的变量，一个使用过的全局变量赋值为nil即表示删除了这个变量。
nil是值，也是一个类型，可以直接给一个变量赋值为nil。
```lua
var_global = nil
```

2. __布尔类型__
false与true，只有false与nil表示false，其他的所有都表示为true(包括数值0与空字符串"")
也可以直接给变量赋值false或者true。
```lua
boolean = false
```

3. __数值类型__
在lua中数值只有一种，就是c语言中的double，并不会有浮点数的效率问题，下面的赋值都是合法的
```lua
var_int = 12
var_float = 20.01
var_math = 1.34e12
var_math_neg = 0.12e-12
```
4. __字符串__
_**定义与使用**_
lua中比较常用的一种数据类型，可以处理_量_很大的字符串，定义的方式就是使用单引号或者双引号括起来都可以。
```lua
var_string_dou_qt = "string in lua with double quot"
var_string_sig_qt = "string in lua with single quot"
```
在lua中也如c语言中，支持一些转义字符，如换行，响铃之类，如下：
**符号** | **含义**
---|---
\a | 响铃 
\b | 退格
\f | 换页
\n | 换行
\r | 回车
\t | 横向制表
\v | 纵向制表
\\ | 转义
\" | 双引号
\' | 单引号
\[ | 右中括号
\] | 右中括号
lua中也支持使用\xxx的方式表示字符(x为一个十进制的数字)，如\049就是数字1，\98就是小写字母b。
有些情况就是使用一些大段的字符串，而且显示的时候需要换行，这样就会在单行的字符串中出现多个\n之类的转义字符出现
可能美观上不太好看，lua提供了另一个字符串的表示方法，就是用"[["与"]]"将要显示或者处理的字符串括起来，那么二个[[与]]
之间的字符串就会是保持原样。
```lua
var_string_in_bracket = [[
keep origion style string in lua
that use double square bracket
]]
```
先提一下字符的连接是".."使用二个句点连接字符串:
```lua
var_str = "prefix_string"
print(var_str .. "_middle_string" .. "_suffix_string")
```
输出的就是_prefix_string_middle_string_suffix_string_。
lua中的字符串是定义后是不可修改的(类似于c语言中的字符串字面量)，但是可以用一个新的字符串变量接收修改后的字符串。
这里的字符串也是不需要手动分配内存与释放内存的，都是lua自动分配与释放的。
```lua
var_str = "string for lua"
--这里并没有改动var_str的内容，而是返回了一个新的字符串，被str_res接收
var_res = string.gsub(var_str, "lua", "python")
print("var_res: " .. var_res)
print("var_str: " .. var_str)
```
_**字符串与数值的互转**_
lua会自动的在字符串与数值类型间相互转换，比如对一个字符串进行数值上才有的操作时，这时字符串就会转为数值
会在期望一个数值的地方将字符串转为数值，如果期望数值的地方不是数值或者转不成数值就会报错。
同理，在期望一个字符串的地方遇到了数值，就会把数值转换成字符串。
```lua
-- 此时"25"会转换为数值25，var_rst就是37
var_rst = "25" + 12
print("12" * "4")--输出48
print("12" * "str")--报错，期望str是一个数值然后与12做乘法，但是str无法转换成数值，报错！
print(100 .. 200) -- 100200，数字后的连接符号前要有空格，不然可能会被解释错。
```

5. __函数__




















