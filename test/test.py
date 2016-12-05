# -*- coding: utf-8 -*-

import TJudger

CFG = {
	'language':'C++',
	'source_name':'lll.cpp',
	'in_file':u'/home/timhsue/TJudger/test/lll.in',
	'out_file':u'lll.out',
	'ans_file':u'/home/timhsue/TJudger/test/lll.ans',
	'time_limit':1000,
	'memory_limit':64,
	'compile option': ['-O2', '-lm', '-DONLINE_JUDGE']
}

print CFG
# print "Begin"
RES = TJudger.run(CFG)
# print "End"

print RES
