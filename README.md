# TJudger

###NowVersion 1.0 β


###Clarify
Now version just contains a sandbox.... will updata some time later.  
TODO:
	
	python API
	main judger
	comparator
	plugin support


###Usage
check if you've already install libseccomp-dev;  
otherwise ``sudo apt-get install libseccomp-dev``  
``#include "sandbox.cpp"``  
**notice: compile with -lseccomp**  
use``SandBox xxx;``to create a sandbox;  
use``RunConfig yyy(...);``to creat a config list;  
use``RunResult RES;``to creat a result struct;  
like ``RunConfig yyy(true, true, (char*)"./lll", (char*)"lll.in", (char*)"lll.out", (char*)"lll.ans", LimitList(1000, 64));``;  
means use sandbox and limit time, mem.... and exe file is lll, in file is lll.in out file is lll.out ans file is lll.ans, time limit is 1000ms, mem limit is 64MB;  
then run ``(SandBox)xxx.run(yyy, res)``;  
you will get result in res;  
it contains:  

	use_time
	use_memory
	run_statu (SYSTEM SIGNAL)
	judger_error (help find bug....)

ans there is a sample in sandbox.cpp - int main();  
if you want to try, please compile with -lseccomp

My English is not hao. see no understand ladao....  

**The first release wiil happen within the week~**
