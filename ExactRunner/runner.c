#include "runner.h"


int read_config(Config *CFG) {
	char tmp[300];
	int cp_nums = 0;
	FILE *stream;
	
	stream = fopen("judger.cfg", "r");
	if (stream == NULL) {
		REPORTER("Open config fail");
		return -1;
	}
	
	fscanf(stream, "%s\n", tmp);
	CFG -> language = (char *)malloc((strlen(tmp) + 1) * sizeof(char));
	strcpy(CFG -> language, tmp);
	
	fscanf(stream, "%s", tmp);
	CFG -> source_name = (char *)malloc((strlen(tmp) + 1) * sizeof(char));
	strcpy(CFG -> source_name, tmp);	
	
	fscanf(stream, "%s", tmp);
	CFG -> in_file = (char *)malloc((strlen(tmp) + 1) * sizeof(char));
	strcpy(CFG -> in_file, tmp);	
	
	fscanf(stream, "%s", tmp);
	CFG -> out_file = (char *)malloc((strlen(tmp) + 1) * sizeof(char));
	strcpy(CFG -> out_file, tmp);	
	
	fscanf(stream, "%s", tmp);
	CFG -> ans_file = (char *)malloc((strlen(tmp) + 1) * sizeof(char));
	strcpy(CFG -> ans_file, tmp);	
	
	fscanf(stream, "%d%d%d", &CFG -> time_limit, &CFG -> memory_limit, &cp_nums);
	
	CFG -> compile_option = (char **)malloc(sizeof(char*) * (cp_nums + 1));
	for (int i = 0; i < cp_nums; i++) {
		char tmp[200];
		fscanf(stream, "%s", tmp);
		CFG -> compile_option[i] = (char *)malloc((strlen(tmp) + 1)* sizeof(char));
		strcpy(CFG -> compile_option[i], tmp);
	}
	CFG -> compile_option[cp_nums] = NULL;
	
	if (fscanf(stream, "%s", tmp) != EOF) {
		CFG -> special_judge = (char *)malloc((strlen(tmp) + 1) * sizeof(char));
		strcpy(CFG -> special_judge, tmp);
		
		fscanf(stream, "%s", tmp);
		CFG -> spj_language = (char *)malloc((strlen(tmp) + 1) * sizeof(char));
		strcpy(CFG -> spj_language, tmp);
	} else {
		CFG -> spj_language = NULL;
		CFG -> special_judge = NULL;
	}
	
	fclose(stream);
	
	return 0;
}


void free_cfg(Config *CFG) {
	if (CFG -> language != NULL) free(CFG -> language);
	if (CFG -> source_name != NULL) free(CFG -> source_name);
	if (CFG -> in_file != NULL) free(CFG -> in_file);
	if (CFG -> out_file != NULL) free(CFG -> out_file);
	if (CFG -> ans_file != NULL) free(CFG -> ans_file);
	if (CFG -> compile_option != NULL) {
		for (int i = 0; CFG -> compile_option[i] != NULL; i++) {
			free(CFG -> compile_option[i]);
		}
		free(CFG -> compile_option);
	}
}

int write_result(Result *RES) {
	FILE *stream;
	stream = fopen("judger.res", "w");
	if (stream == NULL) {
		REPORTER("Open result fail");
		return -1;
	}
	fprintf(stream, "%s\n%d\n%d\n%d", RES -> status, RES -> score, RES -> use_time, RES -> use_memory);
	
	stream = fopen("judger.compile", "w");
	if (stream == NULL) {
		REPORTER("Open compile information fail");
		return -1;
	}
	fprintf(stream, "%s\n", RES -> compile_info);
	
	stream = fopen("judger.in", "w");
	if (stream == NULL) {
		REPORTER("Open in file fail");
		return -1;
	}
	fprintf(stream, "%s\n", RES -> in);
	
	stream = fopen("judger.out", "w");
	if (stream == NULL) {
		REPORTER("Open out file fail");
		return -1;
	}
	fprintf(stream, "%s\n", RES -> out);
	
	stream = fopen("judger.ans", "w");
	if (stream == NULL) {
		REPORTER("Open ans file fail");
		return -1;
	}
	fprintf(stream, "%s\n", RES -> ans);
	
	fclose(stream);
	
	return 0;
}


int main(void) {
	Config CFG;
	Result RES;	
	
	if (read_config(&CFG) != 0) {
		REPORTER("Read config fail");
		free_cfg(&CFG);
		return 0;
	}
	
	RES = run(&CFG);

	if (write_result(&RES) != 0) {
		REPORTER("Write config fail");
		free_cfg(&CFG);
		return 0;
	}
	
	free_cfg(&CFG);
	
	return 0;
}
