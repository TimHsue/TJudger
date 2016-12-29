# TJudger ( NowVersion 1.0)
TJudger is a judger designed for online judge system, which is also available for local test purpose.
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

For details, check ``test/test.c``.  
###Exact Edition:
**You should compile your juger source with ``-lseccomp`` and ``-DEXACT_MOD``**  
#About Special Judge:
The code in ``[]`` is in need for the judgement with special judger. Just leave it blank if there is no special judge.
Special judger also runs in sandbox and have time/memory limit for system safety. You can tweak this in ``src/judger.c``.  
The main function of the special judger should be declared like ``int main(int argc, char *argv[])``.Because the main judger passes the file content to special judger by `argv` in order (in = argv[0], out = argv[1], ans = argv[2]).  
Your special judger should print the score of the candidate in `stdout`. The score should be in the range of 0 ~ 100. Otherwise it may lead to judger error. The main judger will only read the first 8 characters from the output of your special judger and cut out by last num can get.
#Status details
	System Error: Invalid configuration
	Judger Error: Internal Judger error
	Accepted: The program has exited successfully and passed all tests.
	Wrong Answer: The program has exited successfully but printed the wrong output.
	Dangerous System Call: The program has been killed because of invoking dangerous system call.
	Runtime Error: The program has been killed because of errors like Segment Fault.
	Compile Error: The source can't be compiled correctly.
	Time Limit Exceed: The Time your program costs is too large.
	Memory Limit Exceed: The memory your program costs is too large.
	Output Limit Exceed: The output of your program is too large.
	Run Successfully: Run successfully but something goes wrong....
	with spj:
	Compile Special Judge Error: The source of your special judger can't be compiled correctly.
	Run Special Judge Error: The special judger exited with errors.
	Partly Correct: The special judger has found that the output of the program is not completely correct.


( Special judge mod has NOT been tested. It will be coming soon )  
**( Normal Edition memory using is based on ru_minflt. That may lead to the memory using which is detected is bigger than which is actually used )**  
**( Exact Edition memory using is based on ru_maxrss )**


##[Development Records](http://xtt.lcybox.com/wp-content/themes/XTT_A/article_tmp/sandbox.php)
