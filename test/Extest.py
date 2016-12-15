#! /usr/bin/python
# -*- coding: utf-8 -*-

import os
import ExTJudger


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

'''
CFG['source_name'] = 'MLE/lll.cpp'
# print CFG
res = TJudger.run(CFG)
print res


for i in range(1, 10):
	CFG['source_name'] = 'AC/lll.cpp'
	RES = TJudger.run(CFG)
	print RES

'''

a = ["AC", "WA", "RE", "TLE", "MLE", "OLE", "MLE(stack)", "CE", "DSC"]
# a = ["AC"]

for item in a:
	CFG['source_name'] = item + '/lll.cpp'
	print item + " Config:"
	print CFG
	RES = ExTJudger.run(CFG)
	print item + " Result:"
	print RES
	print ""


