#ifndef __COMPILERINFOLIST_H__

#include "status.h"

#define CompilerNameLength 40
#define CompilerVersionLength 100
#define LanguageNameLength 20
#define CompilerPathLength 100
#define SourcePathLength 100

typedef struct compiler_info
{
	char *CompilerName;
	char *CompilerVersion;
	char **LanguageName;
	int lnn;
	short LanguageType;
	char *CompilerPath;
	char **SourcePath;
	int spn;
	struct compiler_info *next;
} compiler_info;

compiler_info *compiler_info_new_node(char *cn, char *cv, char **ln, int lnl, short lt, char *cp);//初始化并赋值
void PrintList(compiler_info *plist);//输出
status compiler_info_push_front(compiler_info **pplist, char *cn, char *cv, char **ln, int lnl, short lt, char *cp);//头插
status compiler_info_free(compiler_info** pplist);//删除


#endif//__COMPILERINFOLIST_H__
