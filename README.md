# TJudger ( NowVersion 1.0 β )
---
This is a Judger for onlinejudge and loco test.  
#Usage
##For Python:
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
	}
Then you will get result from ``RES = TJudger.run(CFG)``.  
The result is also a dict:

	RES = {
		'status': 'Wrong Answer', # run status
		'compile_info': '', # if compile fail, it will show compile error message. Otherwise, it's empty.
		'use_memory': 7728L, # the total memory program used
		'in': '2 3\n', # input detail
		'use_time': 0L, # the total time program used ( usr + sys )
		'score': 0L, # the score program can get(top: 100)
		'ans': '5  ', # answer detail
		'out': '' # output detail
	}

For detail, check ``test/test.py``.  
##For C/C++:
**Check if you have installed ``libseccomp-dev``**  
If not, you need to install it with``sudo apt-get install libseccomp-dev``.  
Header:  
``#include "xxxx/src/judger.cpp"``
Then initialize Config and Result with:

	char* argv[] = {"xxx", "xxx", ...};
	Config CFG(language, soure file name, in file name, out file name, ans file name, time limit(ms), memery limit(MB), argv);
	Result RES;
Then run:  
``run(CFG, RES)``  
The result is a struct defined in ``src/judger.h``, you can visit all things in RES via:

	RES.score // the score program can get(top: 100)
	RES.compile_info // if compile fail, it will show compile error message. Otherwise, it's empty.
	RES.status // run status
	RES.use_time // the total time program used ( usr + sys )
	RES.use_memory // # the total memory program used
	RES.in // input detail
	RES.out // output detail
	RES.ans // answer detail
 
For detail, check ``test/test.cpp``.  

##Status detail
	System Error: Config is not correct
	Judger Error: Judge system goes wrongly
	Accept: Run successfully and get the correct output
	Wrong Answer: Run successfully but get the wrong output
	Dangerous System Call: The program is killed because of runing with dangerous system call
	Runing Error: The program is killed because of error like segmentfault
	Compie Error: Your program sourse cannot be compiled correctly
	Time Limit Exceed: The Time your program costs is too large
	Memory Limit Exceed: The memory your program costs is too large
	Output Limit Exceed: The output of your program is too large
	Run Successfully: Run successfully but something goes wrong....
	with spj:
	Compile Special Judge Error: Your special judge source cannot be compiled correctly
	Run Special Judge Error: Your speical judger gose wrongly
	Partly Correct: Special judger find that your answer is not completely wrong
	

( Special judge mod is untested. It will coming soon )


##[Development Records](http://xtt.lcybox.com/wp-content/themes/XTT_A/article_tmp/sandbox.php)
