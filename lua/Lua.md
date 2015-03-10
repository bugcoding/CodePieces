## Lua Note##
<!--March 9, 2015 10:49 PM--Lua.md-->


**Hell World**

1. 直接在命令行运行(前提lua已加入环境变量中):
> \>\> print("Hello, World!")
2. print语句写在文件中，如hello.lua, print("Hello, World!")保存
在文件所在目录打开命令行，然后再执行命令:
> lua hello.lua
3. lua还提供一个e选项可以直接在命令行中执行lua代码:
> lua -e "print('Hello, World!')"
4. lua也可以像c语言一样支持命令行参数，然后在lua中捕获这些参数值
参数值都存在一个arg的全局表中
```lua
-- 直接输出包括脚本文件名称在内的所有参数
for i = 0, table.getn(arg) do
	   print(arg[i])
end
```
> lua test.lua arg1 arg2 arg3




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
> 可以通过lua的内置函数type来得知每个变量的类型，type函数返回一个字符串
```lua
print(type("string"))--string
print(type(type))--function
print(type(nil))--nil
```

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
在lua中函数也是一个类型，也就是表示，函数在lua可以当做参数，可以赋值给变量，可以做为函数的返回值
定义方法，function关键字，end结束
```lua
function lua_function(param)
	--todo
end
```
6. __Userdata和Threads__
userdata是c中的定义的新数据类型，它可以直接将c中的数据放在lua的变量中，并且userdata在lua中除了赋值与相等操作并没有其他预定
义好的操作，threads是协同的时候需要用到的。


**表过式**

1. __算术运算符__
**符号** | **作用**
---|---
\+ | 加法
\- | 减法
\* | 乘法
/  | 除法
^  | 乘方
对于减号，如果是二元运算(有二个操作数)，表示减法操作，如果一元操作(只有一个操作数)表示取负数
这些运算符的操作数都是实数，也就是上文类型中的数值类型

2. __关系运算符__
**符号** | **作用**
---|---
\> | 大于
<  | 小于
\>=| 大于等于
<= | 小于等于
== | 等于
~= | 不等于
除了_不等于_其他都与c语言相同，进行相等比较的时候，在lua中比较二个操作数的值，如果值类型不同，那么lua
认为二个值不同，对于userdata，table，function三种数据，lua则是通过比较引用来确定二个对象是否相等
也就是说，单单内部的值相等是不够的，要二个操作数表示的是同一个对象，才相等。
> "10" == 10 这是不同类型的二个值，这个表达式生成false
> 10 > "2" 类似这种数值与字符串的大小比较，lua会直接报错

3. __逻辑运算符__
**符号** | **作用**
---|---
and | 与
or  | 或
not | 非
上文布尔类型的时候说过，lua中除了nil和false是false，其他都是true，逻辑运算也就与c语言相同了
有二个技巧可以用，因为在lua没有三元运算符，可以这样实现
```lua
-- x ? y : z
(x and y) or z
```
还有一种常用的变量非nil测试并赋值
```lua
x = x or val
--等价于
if x == nil then
	  x = val
end
```

4. __连接运算__
字符串的时候提到过了，二个句点，用于连接二个字符串，如果操作数是数值类型，则会把数值转换为字符串
但是数值使用连接符号的时候空格是必须的，以防解释错连接符作用

<!--chap1 above -->










