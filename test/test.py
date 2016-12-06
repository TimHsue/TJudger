# -*- coding: utf-8 -*-

import TJudger


a = ["AC", "WA", "CE", "RE", "TLE", "MLE", "DSC", "OLE"]


CFG = {
	'language':'C++',
	'source_name':'MLE/lll.cpp',
	'in_file':'lll.in',
	'out_file':'lll.out',
	'ans_file':'lll.ans',
	'time_limit':1000,
	'memory_limit':64,
	'compile option': ['-lm', '-DONLINE_JUDGE']
}

for item in a:
	CFG['source_name'] = item + '/lll.cpp'
	print item + " Config:"
	print CFG
	RES = TJudger.run(CFG)
	print item + " Result:"
	print RES
	print "\n"

