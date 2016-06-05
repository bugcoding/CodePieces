set nocompatible
source $VIMRUNTIME/vimrc_example.vim
source $VIMRUNTIME/mswin.vim

set diffexpr=MyDiff()
function! MyDiff()
let opt = '-a --binary '
if &diffopt =~ 'icase' | let opt = opt . '-i ' | endif
if &diffopt =~ 'iwhite' | let opt = opt . '-b ' | endif
let arg1 = v:fname_in
if arg1 =~ ' ' | let arg1 = '"' . arg1 . '"' | endif
let arg2 = v:fname_new
if arg2 =~ ' ' | let arg2 = '"' . arg2 . '"' | endif
let arg3 = v:fname_out
if arg3 =~ ' ' | let arg3 = '"' . arg3 . '"' | endif
let eq = ''
if $VIMRUNTIME =~ ' '
if &sh =~ '\<cmd'
  let cmd = '""' . $VIMRUNTIME . '\diff"'
  let eq = '"'
else
  let cmd = substitute($VIMRUNTIME, ' ', '" ', '') . '\diff"'
endif
else
let cmd = $VIMRUNTIME . '\diff'
endif
silent execute '!' . cmd . ' ' . opt . arg1 . ' ' . arg2 . ' > ' . arg3 . eq
endfunction



" OmniCppComplete
"----------------------OmniCppComplete_BEGIN_----------------------------------
let OmniCpp_ShowAccess = 1
"显示函数参数列表
let OmniCpp_ShowPrototypeInAbbr = 1
"输入.后自动补???
let OmniCpp_MayCompleteDot = 1
"输入->自动补全
let OmniCpp_MayCompleteArrow = 1
"输入::后自动补???
let OmniCpp_MayCompleteScope = 1
let OmniCpp_DefaultNamespaces = ["std", "_GLIBCXX_STD"]
let OmniCpp_SelectFirstItem = 2

au CursorMovedI,InsertLeave * if pumvisible() == 0|silent! pclose|endif

"补全菜单，不弹出预览窗口
""set completeopt=preview,menuone
set completeopt=menuone,menu,longest
"----------------------OmniCppComplete_END_------------------------------------

"设置tab键默认补???
"----------------------SuperTab_BEGIN_-----------------------------------------
""let g:SuperTabDefaultCompletionType = "<C-x><C-p>"
"----------------------SuperTab_END_-------------------------------------------


"winmanager 设置
"-------------winmanager 设置 _BEGIN_---------------------------------

"设置界面分割
""let g:winManagerWindowLayout = "BufExplorer,FileExplorer|TagList"
let g:winManagerWindowLayout="NERDTree|TagList,BufExplorer"

"设置winmanager的宽度，默认???25
let g:winManagerWidth = 26

"定义打开关闭winmanager按键
nmap <silent> <F8> :WMToggle<cr>

"在进入vim时自动打开winmanager
let g:AutoOpenWinManager = 0

"-------------winmanager 设置 _END_------------------------------------



"tags相关设置
"-------------tags相关设置_BEGIN_--------------------
"函数跳转
""set tags=tags;
set autochdir

"函数原型提示(文件打开按F10生成tags)
nmap <F10> <Esc>:!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q<CR>

"Tags开关快捷键
"map  <C-t> :TlistOpen<CR>
":map  <C-w> :TlistClose<CR>

"-------------tags相关设置_END_----------------------


"窗口相关设置
"------------窗口相关设置_BEGIN_----------------------
"窗口最大化
"autocmd guienter * simalt ~x

"gvim启动位置
winpos 120 6

"窗口大小
set lines=35 columns=110

"高亮当前???
"set cursorcolumn

"搜索高亮
"set hlsearch

"底部滚动???
set guioptions+=b

"多标???
":tabedit

"多标签快捷键操作
:map  <F6> :tabprevious<CR>
:map  <F7> :tabnext<CR>
:map  <F5> :tabnew<CR>
:map  <F4> :tabc<CR>i
:imap  <F6> <ESC>:tabprevious<CR>i
:imap  <F7> <ESC>:tabnext<CR>i
:imap  <F5> <ESC>:tabnew<CR>i
:imap  <F4> <ESC>:tabc<CR>i

"设定光标离窗口上下边??? 7 行时窗口自动滚动
set scrolloff=7

"命令行高???
set cmdheight=1

"显示命令???
set showcmd

"缩进的宽???
set tabstop=4

"自动缩进宽度
set shiftwidth=4

"隐藏工具???
"set guioptions-=T

"隐藏菜单???
"set guioptions-=m

""菜单栏和工具栏的动态显示和隐藏
set guioptions-=m
set guioptions-=T
map <silent> <F2> :if &guioptions =~# 'm' <Bar>
    \set guioptions-=m <bar>
\else <Bar>
    \set guioptions+=m <Bar>
\endif<CR>

"隐藏底部滚动???
set guioptions-=b

"设置按BackSpace的时候可以一次删除掉4个空???
set softtabstop=4

set expandtab

"python文件的缩进设???
autocmd FileType python setlocal tabstop=4 shiftwidth=4 softtabstop=4 textwidth=79

"命令行增强模???
set wildmenu

"行距
set linespace=0

"------------窗口相关设置_END_------------------------


"特定编程语言选项
"------------------特定语言_BEGIN_--------------------------


"------------------特定语言_END_----------------------------


"其他相关
"-------------------其他_BEGIN_---------------------

"光标可一直向左移???
set whichwrap=b,s,<,>,[,]

"防止特殊符号无法正常显示
set ambiwidth=double

"设置菜单显示为英???
set langmenu=en_US
let $LANG = 'en_US'
source $VIMRUNTIME/delmenu.vim
source $VIMRUNTIME/menu.vim

"编码检???
set fileencodings=ucs-bom,utf-8,cp936,gb18030,big5,euc-jp,euc-kr,latin1
""set termencoding=utf-8
set encoding=utf-8
set fencs=cp936,utf-8,gbk,ucs-bom,gb18030,gb2312

"临时文件
set nobackup
set noswapfile
set nowritebackup

"插件自动检???
filetype plugin on
filetype plugin indent on

"文件类型自动检???
filetype on

"vim主题, 根据不同的文件使用不同的主题??? python的主题更好看一???
function g:SetColorscheme()
if &filetype == "python"
    colorscheme	gummybears
else
    colorscheme	desertEx
    set cursorline
        hi cursorline guibg=#333333
endif
endfunc
""autocmd FileType * call g:SetColorscheme()
colorscheme	desertEx


"不同系统下的字体选择
if has('win32')
""set guifont=Meslo\ LG\ S\ for\ Powerline:h11:cANSI
set guifont=Menlo\ for\ Powerline:h11:cANSI
elseif has('unix')
let &guifont="Monospace 10"
endif

"关闭兼容模式
set nocompatible

" 每行超过80个的字符用下划线标示
au BufRead,BufNewFile *.asm,*.c,*.cpp,*.java,*.cs,*.sh,*.lua,*.pl,*.pm,*.py,*.rb,*.hs,*.vim 2match Underlined /.\%80v/

" 更改Leader???","
let g:mapleader = ","

"语法高亮开???
syntax enable

"txtbrowser插件
syntax on "打开txt语法高亮
au BufEnter *.txt setlocal ft=txt

"保存.vimrc文件后会自动调用新的.vimrc
autocmd! bufwritepost .vimrc source D:\Program\ Files\Vim\_vimrc

" 不要闪烁
set vb t_vb=
set novb
"去掉按ENTER的那个提???
command! -nargs=1 Silent
\ | execute ':silent !'.
\ | execute ':redraw!'

"使用统一的dos格式
set fileformats=dos

" 可以在buffer的任何地方使用鼠标（类似office中在工作区双击鼠标定位）
set mouse=a
set selection=exclusive
set selectmode=mouse,key

"日历
map ca :Calendar<cr>

"自动缩进
set autoindent
"智能缩进
set smartindent
"C风格缩进
set cindent
"光标所在位置也被选择
set selection=inclusive

"十六进制查看
augroup Binary
au!
au BufReadPost *.* if &bin | %!xxd
au BufReadPost *.* set ft=xxd | endif
augroup END

"去掉press ENTER 提示
command! -nargs=1 Silent
\ | execute ':silent !'.<q-args>
\ | execute ':redraw!'


"快速对齐映???
vnoremap <silent> <Enter> :EasyAlign<cr>

"-------------------其他_END_-----------------------


""Vim-airline
"-------------------Vim-Airline_BEGIN_-----------------------
"分隔???
let g:airline_right_sep = ''
"显示时间
let g:airline_section_z = '%{strftime("%c")}'

"使用powerline patchfont
let g:airline_powerline_fonts = 1

set fillchars+=stl:\ ,stlnc:\
"vim-airline主题
let g:airline_theme="powerlineish"

let g:airline#extensions#tabline#enabled = 0
"-------------------Vim-Airline_END_-----------------------


"行号相关设置
"-----------------行号设置_BEGIN_----------------
"显示行号
set nu

"行号颜色
hi LineNr guifg=#555555 gui=none

"行号显示宽度
set nuw=1
"-----------------行号设置_END_------------------

"代码提示辅助相关
"-----------------代码辅助提示_BEGIN_---------------------
" 设置代码提示窗口的颜色（默认为粉红色???
highlight Pmenu ctermbg=13 guibg=#80c040 guifg=Black
highlight PmenuSel ctermbg=7 guibg=#a06000 guifg=White
highlight PmenuSbar ctermbg=7 guibg=Black
highlight PmenuThumb guibg=Black

"代码行折???
"set foldenable " 开始折???
"set foldmethod=syntax " 设置语法折叠
"set foldcolumn=0 " 设置折叠区域的宽???
"setlocal foldlevel=1 " 设置折叠层数???
"set foldclose=all " 设置为自动关闭折???
"nnoremap <space> @=((foldclosed(line('.')) < 0) ? 'zc' : 'zo')<CR> " 用空格键来开关折???

" 多行注释时样子更好看
let NERDCompactSexyComs=1

"删除多余行尾空格
nmap  :%s= *$==

"使得注释换行时自动加上前导的空格和星???
set formatoptions=tcqro

" 自动补全括号，包括大括号(此处配合auto_pairs插件)
:inoremap { {<CR>}<ESC>O
:inoremap } <c-r>=ClosePair('}')<CR>
:inoremap [ []<ESC>i
:inoremap ] <c-r>=ClosePair(']')<CR>
:inoremap " ""<ESC>i
:inoremap ' ''<ESC>i

function! ClosePair(char)
if getline('.')[col('.') - 1] == a:char
  return "\<Right>"
else
  return a:char
endif
endf

"<F4>添加作者信???
let g:vimrc_author='bugcode'
let g:vimrc_email='bugcoding@gmail.com'
let g:vimrc_homepage='http://bugcode.com.cn'

nmap <F4> :AuthorInfoDetect<cr>

"调用AStyle程序，进行代码美???
function! CodeFormat()
      "取得当前光标所在行???
      let lineNum = line(".")
      "C源程???
      if &filetype == 'c'
             "执行调用外部程序的命???
              exec "%! astyle -A1Lfpjk3NS\<CR>"
      "H头文???(文件类型识别为cpp)，CPP源程???
      elseif &filetype == 'cpp'
             "执行调用外部程序的命???
             exec "%! astyle -A1Lfpjk3NS\<CR>"
      "JAVA源程???
      elseif &filetype == 'java'
             "执行调用外部程序的命???
             exec "%! astyle -A1Lfpjk3NS\<CR>"
      else
             "提示信息
             echo "不支???".&filetype."文件类型???"
      endif
      "返回先前光标所在行
      exec lineNum
endfunc

"映射代码美化函数到Shift+f快捷???
map <C-S-f> <Esc>:call CodeFormat()<CR>

"单词中间不能断行
set lbr
"对亚洲语言支持
set fo+=mB

"统计所有字???
command! -nargs=0 CountChars         :%s/./&/gn|noh

"统计当前所有单词数
command! -nargs=0 CountWords         :%s/\i\+/&/gn|noh
"-----------------代码辅助提示_END_-----------------------

"状态栏相关
"-----------------状态栏相关_BEGIN_------------------------------------------


"状态条显示
"set statusline=%F%m%r%w\ [FORMAT=%{&ff}]\[TYPE=%Y]\[POS=%l,%v]\[ASC=%b]\[HEX=0X%B]\

"显示状态栏
set laststatus=2
"底栏时间
"set statusline+=[%{strftime(\"%m-%d\ %H:%M:%S\")}]

"设置状态栏根据不同状态显示不同颜???
"function!! InsertStatuslineColor(mode)
"    if a:mode == 'i'
"        hi statusline guibg=Yellow
"    elseif a:mode == 'r'
"        hi statusline guibg=Green
"    else
"        hi statusline guibg=Blue
"    endif
"endfunction
"au InsertEnter * call InsertStatuslineColor(v:insertmode)
"au InsertLeave * hi statusline guibg=#187dfb guifg=#333333
"hi statusline guibg=yellow

"状态栏颜色
"hi statusline gui=bold,inverse guifg=#4b4f41 guibg=#4bca48

"垂直分割
hi VertSplit guibg=#31312D guifg=#526A83 ctermfg=Black ctermbg=Black term=none cterm=none gui=none
"状态栏
"hi StatusLineNC guibg=#31312D guifg=#526A83 ctermfg=White ctermbg=Black term=none cterm=none gui=none
"折叠???
hi FoldColumn guibg=#31312D guifg=#526A83 ctermfg=Black ctermbg=darkgreen term=none cterm=none gui=none

"当前行高???
set cursorline
hi cursorline guibg=#333333
""hi CursorLine   gui=underline guibg=#202020 guifg=NONE

""查看某列是否在一列上
map ,ch :call SetColorColumn()<CR>
function! SetColorColumn()
let col_num = virtcol(".")
let cc_list = split(&cc, ',')
if count(cc_list, string(col_num)) <= 0
    execute "set cc+=".col_num
else
    execute "set cc-=".col_num
endif
endfunction

"显示缩进???
"map ,in :call EchoIndentLine()<CR>
function! EchoIndentLine()
se list
"se lcs=tab:>-
set lcs=tab:\|\
hi SpecialKey guifg=#333333
endfunction


"-----------------状态栏相关_END_---------------------------------------


"编译文件
"--------------------编译文件_BEGIN_-------------------------------------
" 编译C源文???
"func! CompileGcc()
"    exec "w"
"    let compilecmd="!gcc -Wall -pedantic -std=c99 "
"    let compileflag="-o %<"
"    exec compilecmd." % ".compileflag
"endfunc
"
"" 编译C++源文???
"func! CompileCpp()
"    exec "w"
"    let compilecmd="!g++ -Wall -pedantic -std=c++98 "
"    let compileflag="-o %<"
"    exec compilecmd." % ".compileflag
"endfunc
"
"" 编译Java源文???
"func! CompileJava()
"    exec "w"
"    exec "!javac %"
"endfunc
"
"" 根据文件类型自动选择相应的编译函???
"func! CompileCode()
"        exec "w"
"     if &filetype == "c"
"         exec "call CompileGcc()"
"     elseif &filetype == "cpp"
"         exec "call CompileCpp()"
"     elseif &filetype == "java"
"         exec "call CompileJava()"
"     elseif &filetype == "perl"
"         exec "call RunPerl()"
"     elseif &filetype == "python"
"         exec "call RunPython()"
"     elseif &filetype == "ruby"
"         exec "call RunRuby()
"     endif
"endfunc
"
"
"" 运行可执行文???
"func! RunResult()
"     exec "w"
"     if &filetype == "c"
"         exec "! %<"
"     elseif &filetype == "cpp"
"         exec "! %<"
"     elseif &filetype == "java"
"         exec "!java %<"
"     elseif &filetype == "perl"
"         exec "!perl %<.pl"
"     elseif &filetype == "python"
"         exec "!python %<.py"
"     endif
"endfunc
"
""定义Debug函数，用来调试程???
func! Debug()
exec "w"
if &filetype == 'c'
    exec "!gcc -g % -o %<.exe"
    exec "!gdb %<.exe"
elseif &filetype == 'cpp'
    exec "!g++ -g % -o %<.exe"
    exec "!gdb %<.exe"
endif
endfunc
"结束定义Debug


" Shift + C 一键保存、编???
""map <S-c> :call CompileCode()<CR><ESC>
"imap  <ESC>:call CompileCode()<CR><ESC>
"vmap  <ESC>:call CompileCode()<CR><ESC>

" Shift + R 一键保存、运???
"p <S-r> :call RunResult()<CR><ESC>
"imap <ESC>:call RunResult()<CR><ESC>
"vmap <ESC>:call RunResult()<CR><ESC>

"Shift + D GDB调试
map <S-d> :call Debug()<CR><ESC>


"快速运行php
function! RunPHP()
  exec "w"
  let runcmd="!php -q %"
  exec runcmd
endfunction
nmap <M-a> :call RunPHP() <CR><ESC>

""编译运行python,lua
function! RunScriptFile()
exec "w"
if &filetype == "py"
    let runcmd="!python %<.py"
elseif &filetype == "lua"
    let runcmd="!lua %<.lua"
endif
exec runcmd
endfunction

nmap <M-r> :call RunScriptFile() <CR><ESC>

"直接编译本文件并生成目标文件，并马上与libpng静态库链接生成可执行文???
func! CompilePng()
exec "w"
    if &filetype == "c"
        let linkcmd = "!gcc -Wall % -o %< -lpng -lz"
    elseif &filetype == "cpp"
        let linkcmd = "!g++ -Wall % -o %< -lpng -lz"
        exec linkcmd
    endif
endfunc

map <M-s> :call CompilePng() <CR><ESC>

"运行与当前文件名相同的可执行文件
func! RunSameFile()
    let file_name = expand("%:r")
    if (has("win32"))
        let exe_file_name = file_name .".exe"
    else
        let exe_file_name = file_name
    endif
    if filereadable(exe_file_name)
        let runcmd = "!%<"
        exec runcmd
    else
        echo exe_file_name." not exist!"
    endif
endfunc

map <S-r> :call RunSameFile() <CR><ESC>


"--------------------编译文件_END_---------------------------------------


"--------------------quickfix窗口相关_BEGIN_------------------------------------

"--------------------quickfix窗口相关_END_--------------------------------------



"-------------------Powerline_BEGIN_-----------------------------

""let g:Powerline_symbols = 'unicode'
""call Pl#Theme#ReplaceSegment('scrollpercent', 'charcode')
""call Pl#Theme#InsertSegment('time', 'after', 'lineinfo')
"call Pl#Theme#ReplaceSegment('filetype', 'filesize')
""call Pl#Theme#InsertSegment('currhigroup', 'after', 'filename')

"-------------------Powerline_END_-------------------------------


"-------------------PCLint_BEGIN_------------------------------------------------------
set errorformat=%f(%l):\ %t%*[^0-9]%n:\ %m

set makeprg=d:\pclint\LINT-NT.exe\ -u\ d:\PCLint\std.lnt\ d:\PCLint\env-vc6.lnt\ \"%\"

"-------------------PCLint_END_--------------------------------------------------------

""代码阅读
""--------------------------------------rainbow config_BEGIN---------------------------------
let g:rainbow_active = 0
let g:rainbow_operators = 2
""--------------------------------------rainbow config_END---------------------------------

"快捷键打开vimrc和快速载入vimrc
"--------------------------------------loading vimrc_BEGIN---------------------------------
map <silent> <leader>sv : so $vim/_vimrc<CR>
map <silent> <leader>se : e $vim/_vimrc<CR>
""--------------------------------------loading vimrc_END-----------------------------------
