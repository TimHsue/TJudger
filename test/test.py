# -*- coding: utf-8 -*-

import TJudger

CFG = {
	'language':'C++',
	'source_name':'lll.cpp',
	'in_file':'lll.in',
	'out_file':'lll.out',
	'ans_file':'lll.ans',
	'time_limit':1000,
	'memory_limit':64,
}

# print "Begin"
RES = TJudger.run(CFG)
# print "End"

print RES
