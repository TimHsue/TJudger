# TJudger 1.0
一只可以本地评测或用在OJ上的评测系统
#使用方法
##Python:
###普通版本:
**在终端输入 ``sudo python setup.py install``**  
这将帮你安装该评测机的所需要的 python-dev 和 libseccomp-dev 扩展，所以如果出现询问是否安装，选Y。  
使用时请在头部加入 :  
``import TJudger``  
我们可以通过一个python的dict类型变量来描述设置选项。  
举个栗子:

	CFG = {
		'language':'C++', # 源文件语言
		'source_name':'lll.cpp', # 源文件名
		'in_file':'lll.in', # 输入文件名
		'out_file':'lll.out', # 输出文件名
		'ans_file':'lll.ans', # 答案文件名
		'time_limit':1000, # 时间限制(ms)
		'memory_limit':64, # 内存限制(MiB)
		'compile option':['-O2', '-lm', '-DONLINE_JUDGE'] # 编译选项
		['special_judge':'spj.cpp', # spj文件名]
		['spj_language':'C++' # spj语言]
	}
然后你可以通过运行代码 ``RES = TJudger.run(CFG)`` 来得到特评测结果。  
结果也是一个python的dict类型变量:

	RES = {
		'status': 'Wrong Answer', # 运行结果
		'compile_info': '', # 如果编译错误，这里将显示编译错误信息
		'use_memory': 7728L, # 内存占用峰值
		'in': '2 3\n', # 输入文件内容
		'use_time': 0L, # 程序所占用的用户态时间
		'score': 0L, # 该程序所得分数(上限为100)
		'ans': '5  ', # 答案文件内容
		'out': '' # 输出文件内容
	}

你可以在 ``test/test.py`` 和 ``test/test_spj.py``中了解更多细节。  
###精确版本:
由于 ``fork()``可能会导致不可靠的内存使用峰值，所以我们直接采用类似于``os.system("./runner")``的命令，运行一个与当前进程不共享内存的新程序，以达到减少误差的目的(当然，即时采用这种方法，误差在内存占用非常小的时候依然存在)。  
如果你想使用精确版本的话，需要进行以下安装:  
**在终端输入 ``sudo python Exsetup.py install``**  
**拷贝运行之后编译出的程序(名为 ``runner``)到你评测程序所在的目录**  
使用时请在头部加入:  
**``import ExTJudger``**  
**注意，这里不能 ``import TJudger``**  
与普通版本不同的是，你需要通过 ``RES = ExTJudger.run(CFG) `` 来获取结果。  
其余部分都与普通版本相同.  
你可以在 ``test/Extest.py`` 和 ``test/Extest_spj.py``(在此目录下应该还有一个名为 ``runner`` 的程序)来了解更多细节。  
##C/C++:
###普通版本:
**首先确认你已经安装了 ``libseccomp-dev``**  
如果没有的话，在终端输入如下指令来安装``sudo apt-get install libseccomp-dev``.  
需要包含头文件:  
``#include "xxxx/src/judger.c"``  
**在编译你的评测程序的时候，需要在编译选项中加入``-lseccomp``**  
通过如下方式初始化设置和结果:  

	char* argv[] = {"xxx", "xxx", ...};
	Config CFG = {
		language, soure file name, in file name, out filename, ans file name, 
		time limit(ms), memery limit(MiB), argv[, special judger name][, special judger language]};
	Result RES;
然后通过代码 ``RES = run(&CFG)`` 来获取运行结果  
结果是一个在 ``src/judger.h`` 中定义的结构体, 你可以通过如下方式来访问到该结构体的所有成员:

	RES.score // 该程序所得分数(上限为100)
	RES.compile_info // 如果编译错误，这里将显示编译错误信息
	RES.status // 运行结果
	RES.use_time // 程序所占用的用户态时间
	RES.use_memory // 内存占用峰值
	RES.in // 输入文件内容
	RES.out // 输出文件内容
	RES.ans // 答案文件内容
 
你可以在 ``test/test.c`` 和 ``test/test_spj.c``中了解更多细节。  
###精确版本:
**编译的时候除了加入 ``-lseccomp`` 选项之外只需要再加入 ``-DEXACT_MOD`` 选项即可**  
#关于评测插件 ( spj ) :
所有用``[]``圈起来的内容是spj相关设置，假如没有spj的话，可以直接留空。  
为了系统的安全，spj同样有沙盒以及时间空间的限制，假如你觉得这样会造成诸多不便的话，可以在``src/judger.c``中修改相关设置。  
主评测将通过argv来向spj通信，argv的前三项依次是，输入文件名，输出文件名和答案文件名，你可以在自己的spj中直接读取这些内容。  
主评测将通过获取spj的输出来得到该测试点能得到的分数，spj需要在最后通过标准输出流输出一个0~100之间的整数来告诉主评测用户的得分，系统只会截取spj输出的前8个字符，且截取到最后一次出现的连续整数，还会将该数对100取膜。

#状态信息 ( status ) :
	System Error: 设置错误或其他问题(请查阅TJudger.log获取详细错误信息及代码)
	Judger Error: 主评测系统故障
	Accepted: 成功运行并得到了正确的结果
	Wrong Answer: 成功运行但到得了错误的结果
	Dangerous System Call: 程序因为危险的系统调用被终止
	Runtime Error: 程序因为类似于segmentfault的错误而被系统终止，即运行错误
	Compile Error: 源代码无法被正确编译
	Time Limit Exceed: 超过时间限制
	Memory Limit Exceed: 超过内存限制
	Output Limit Exceed: 程序输出过大
	Run Successfully: 运行成功但是不可预知的错误发生了..
	在有spj的情况下还会有如下状态:
	Compile Special Judge Error: spj无法被正确编译
	Run Special Judge Error: spj运行错误
	Partly Correct: 程序输出被spj认为部分正确 ( 一般此时该点得分不为满分100分 ) 
	

( spj还未经过正规测试，不久后将继续更新 )  
**( 普通版本的内存检测是基于ru_minflt(缺页)的，这可能会导致检测到的内存比实际使用大 )**  
**( 精确版本的内存检测是基于ru_maxrss的，比较准确，请放心使用 )**


##[开发记录 ( 持续更新中 ) ](http://xtt.lcybox.com/wp-content/themes/XTT_A/article_tmp/sandbox.php)

( 由于本人英语水平所限 ( 一年多没上英语课了.. ) ，且现行英文版本为当时一时兴起所做，可能水平比机翻高一些，但是仅供参考，关于语法和语句及用词的相关问题，除非个人咨询，不接受任何建议，感谢理解 )

# TJudger 1.0
This is a Judger for onlinejudge and local test.  
#Usage
##For Python:
###Normal Edition:
**Run ``sudo python setup.py install``**  
It will help you install python-dev and libseccomp-dev which is in need for judger.  
Import module:  
``import TJudger``  
There should be a dict to describe config.  
For instance:

	CFG = {
		'language':'C++', # compile language
		'source_name':'lll.cpp', # source file name
		'in_file':'lll.in', # input file name
		'out_file':'lll.out', # output file name
		'ans_file':'lll.ans', # answer file name
		'time_limit':1000, # time limit(ms)
		'memory_limit':64, # memory limit(MB)
		'compile option':['-O2', '-lm', '-DONLINE_JUDGE'] # compile options
		['special_judge':'spj.cpp', # special judger name]
		['spj_language':'C++' # special judger language]
	}
Then you will get result from ``RES = TJudger.run(CFG)``.  
The result is also a dict:

	RES = {
		'status': 'Wrong Answer', # run status
		'compile_info': '', # if compile fail, it will show compile error message. Otherwise, it's empty.
		'use_memory': 7728L, # the total memory program used
		'in': '2 3\n', # input detail
		'use_time': 0L, # the user time program used
		'score': 0L, # the score program can get(top: 100)
		'ans': '5  ', # answer detail
		'out': '' # output detail
	}

For details, check ``test/test.py`` and ``test/test_spj.py``.  
###Exact Edition:
Because of ``fork()`` may lead to unreliable memory using in python when actual memory using is very small, using ``os.system("./runner")`` to run judger in a new program in Exact Edition does do better in memory using detection instead of running in same program by API.  
In order to use Exact Edition:  
**Run ``sudo python Exsetup.py install``**  
**Then copy ``runner`` which is be compiled after Run ``sudo python Exsetup.py install`` to your judger root directory**  
Import module:  
**``import ExTJudger`` insead of ``import TJudger``**  
You can get result from ``RES = ExTJudger.run(CFG) `` instead.  
The other thing is the same as Normal Edition.  
For details, check ``test/Extest.py`` and ``test/Extest_spj.py``( there should also be a ``runner`` ) .
##For C/C++:
###Normal Edition:
**Check if you have installed ``libseccomp-dev``**  
If not, you need to install it with``sudo apt-get install libseccomp-dev``.  
**You should compile your juger source with ``-lseccomp``**  
Header:  
``#include "xxxx/src/judger.c"``
Then initialize Config and Result with:

	char* argv[] = {"xxx", "xxx", ...};
	Config CFG = {
		language, soure file name, in file name, out filename, ans file name, 
		time limit(ms), memery limit(MiB), argv[, special judger name][, special judger language]};
	Result RES;
Then run:  
``RES = run(&CFG)``  
The result is a struct defined in ``src/judger.h``, you can view all things in RES by:

	RES.score // the score program can get(top: 100)
	RES.compile_info // if compile fail, it will show compile error message. Otherwise, it's empty.
	RES.status // run status
	RES.use_time // the user time program used
	RES.use_memory // the total memory program used
	RES.in // input detail
	RES.out // output detail
	RES.ans // answer detail
 
For details, check ``test/test.c`` and ``test/test_spj.py``.  
###Exact Edition:
**You should compile your juger source with ``-lseccomp`` and ``-DEXACT_MOD``**  
#About Special Judge:
The code in ``[]`` is in need for the judgement with special judger. And if you dont have special judger, you can ignore this option and leave it blank.  
Special judger also runs in sandbox and have time/memory limit for system safety. You can change this option in ``src/judger.c``.  
Your special judger should code with ``int main(int argc, char *argv[])``. Because main judger pass the message (in file, out file, ans file) to special judger by argv in order:  
in_file = argv[0], out_file = argv[1], ans_file = argv[2]  
And your special judger should print the score that program can get in stdout. The score needs to be in the range of 0 ~ 100. Otherwise it may lead to judger error. Main judger will and only will read 8 characters in your special judger output and cut out by last num can get.
#Status details
	System Error: Config is not correct
	Judger Error: Judge system goes wrongly
	Accepted: Run successfully and get the correct output
	Wrong Answer: Run successfully but get the wrong output
	Dangerous System Call: The program is killed because of runing with dangerous system call
	Runtime Error: The program is killed because of error like segmentfault
	Compile Error: Your program sourse cannot be compiled correctly
	Time Limit Exceed: The Time your program costs is too large
	Memory Limit Exceed: The memory your program costs is too large
	Output Limit Exceed: The output of your program is too large
	Run Successfully: Run successfully but something goes wrong....
	with spj:
	Compile Special Judge Error: Your special judge source cannot be compiled correctly
	Run Special Judge Error: Your speical judger gose wrongly
	Partly Correct: Special judger find that your answer is not completely wrong
	

( Special judge mod is untested. It will be coming soon )  
**( Normal Edition memory using is based on ru_minflt. That may lead to the memory using which is detected is bigger than which is actually used )**  
**( Exact Edition memory using is based on ru_maxrss )**


##[Development Records](http://xtt.lcybox.com/wp-content/themes/XTT_A/article_tmp/sandbox.php)
