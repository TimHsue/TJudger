#! /usr/bin/python
# -*- coding: utf-8 -*-

import sys
import TJudger


a = ["AC", "WA", "CE", "RE", "MLE", "DSC", "OLE"]
def test():
	for i in range(1, 10):
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

		CFG['source_name'] = 'AC/lll.cpp'
		# print CFG
		res = TJudger.run(CFG)


def run():
    CFG = {
        'language':'C++',
        'source_name':'AC/lll.cpp',
        'in_file':'lll.in',
        'out_file':'lll.out',
        'ans_file':'lll.ans',
        'time_limit':1000,
        'memory_limit':64,
        'compile option':['-O2', '-lm', '-DONLINE_JUDGE']
    }
    
    res = TJudger.run(CFG)
    
    result = {}
    result['status'] = res['status']
    
    if not 'use_time' in res:
        result['time_used'] = 0
    else:
        result["time_used"] = res["use_time"]
    if not 'use_memory' in res:
        result['memory_used'] = 0
    else:
        result["memory_used"] = res["use_memory"]
        
    result['score'] = res['score']
    
    if 'in' in res:
        result["input"] = res['in'][0: min(120, len(res['in']))]
    if 'ans' in res:
        result["answer"] = res['ans'][0: min(120, len(res['ans']))]
    if 'out' in res:
        result["user_out"] = res['out'][0: min(120, len(res['out']))]
    if 'compile_info' in res:
        result['compile_info'] = res['compile_info']

    return result


def judge():
    result = {"status": "Judging", "score": 0, "total_time": 0, "max_memory": 0, "case_num": 0}

    for i in range(1, 10):
        res = run()
        
        if res['status'] == "Compile Error":
            result['compiler_output'] = res['compile_info']
            result['status'] = "Compile Error"
            return result
        
        result[i] = res
        result["case_num"] += 1
        result["total_time"] += res["time_used"]
        if res["memory_used"] > result["max_memory"]:
            result["max_memory"] = res["memory_used"]

        if res["status"] == "Accepted":
            result["score"] += 1.0 / 10.0 * res['score']
        elif result["status"] == "Judging":
            result["status"] = res["status"]
        
    result["score"] = int(result["score"] + 0.1)
    if result["status"] == "Judging":
        result["status"] = "Accepted"
    return result


if __name__ == '__main__':
	print judge()

'''
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
	
'''
for i in range(1, 10):
	CFG['source_name'] = 'AC/lll.cpp'
	RES = TJudger.run(CFG)
	print RES
'''
