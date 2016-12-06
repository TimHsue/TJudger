#! /usr/bin/python
# -*- coding: utf-8 -*-

import TJudger


a = ["AC", "WA", "CE", "RE", "MLE", "DSC", "OLE"]


CFG = {
	'language':'C++',
	'source_name':'MLE/lll.cpp',
	'in_file':'lll.in',
	'out_file':'lll.out',
	'ans_file':'lll.ans',
	'time_limit':1000,
	'memory_limit':64,
	 'compile option':['-O2', '-lm', '-DONLINE_JUDGE']
}

for i in range(1, 10):
	for item in a:
		CFG['source_name'] = item + '/lll.cpp'
		# print item + " Config:"
		# print CFG
		RES = TJudger.run(CFG)
		# print item + " Result:"
		print RES['use_memory']
		print RES['use_time']
		print ""

	
'''
for i in range(1, 10):
	CFG['source_name'] = 'AC/lll.cpp'
	RES = TJudger.run(CFG)
	print RES
'''
