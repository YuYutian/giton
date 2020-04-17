#include "compiler_info_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

compiler_info *compiler_info_new_node(char *cn, char *cv, char **ln, int lnn, short lt, char *cp)
{
	compiler_info *node = (compiler_info*)malloc(sizeof(compiler_info));
	int index = 0;
	if (NULL == node)
		return NULL;
	node->CompilerName = (char*)malloc(sizeof(char) * CompilerNameLength);
	if (NULL == node->CompilerName)
		return NULL;
	snprintf(node->CompilerName, CompilerNameLength, "%s", cn);
	node->CompilerVersion = (char*)malloc(sizeof(char) * CompilerVersionLength);
	if (NULL == node->CompilerVersion)
		return NULL;
	snprintf(node->CompilerVersion, CompilerVersionLength, "%s", cv);
	node->CompilerPath = (char*)malloc(sizeof(char) *CompilerPathLength);
	if (NULL == node->CompilerPath)
		return NULL;
	snprintf(node->CompilerPath, CompilerPathLength, "%s", cp);
	node->LanguageType = lt;
	node->lnn = lnn;
	node->LanguageName = (char **)malloc(sizeof(char *) * lnn);
	if (NULL == node->LanguageName)
		return NULL;
	for(index = 0; index < lnn; index++)
	{
		node->LanguageName[index] = (char*)malloc(sizeof(char) * LanguageNameLength);
		if (NULL == node->LanguageName[index])
			return NULL;
		snprintf(node->LanguageName[index], LanguageNameLength, "%s", ln[index]);
	}
	node->spn = 0;
	node->SourcePath = NULL;
	node->next = NULL;
	return node;
}

void PrintList(compiler_info *plist)//打印链表
{
    compiler_info *tail = plist;
    while (tail)
    {
        printf("CompilerName: %s, CompilerVersion: %s, LanguageType: %d, CompilerPath: %s, LanguageName: ", tail->CompilerName, tail->CompilerVersion, tail->LanguageType, tail->CompilerPath);
	int index = 0;
	for(;index<tail->lnn;index++)
		printf("%s ", tail->LanguageName[index]);
	printf("\n");
	tail = tail->next;
    }
}

status compiler_info_push_front(compiler_info **pplist, char *cn, char *cv, char **ln, int lnn, short lt, char *cp)//头插
{
	if (*pplist == NULL)
	{
		*pplist = compiler_info_new_node(cn, cv, ln, lnn, lt, cp);
		if (*pplist == NULL)
		{
			return NOT_COMPLETED;
		}
	}
	else
	{
		compiler_info *tmp = compiler_info_new_node(cn, cv, ln, lnn, lt, cp);
		if (tmp == NULL)
		{
			return NOT_COMPLETED;
		}
		tmp->next = *pplist;
		*pplist = tmp;
	}
	return OK;
}

status compiler_info_free(compiler_info** pplist)//删除
{
	compiler_info* tmp = *pplist;
	compiler_info* p = NULL;
	while (tmp->next != NULL)
	{
		p = tmp;
		tmp = tmp->next;
		free(p->CompilerName);
		free(p->CompilerVersion);
                int index = 0;
		for(index = 0; index<tmp->lnn; index++)
			free(tmp->LanguageName[index]);
                free(tmp->LanguageName);
		free(p->CompilerPath);
		for(index = 0; index<tmp->spn; index++)
			free(tmp->SourcePath[index]);
		if(NULL != tmp->SourcePath)
			free(tmp->SourcePath);
		free(p);
	}
	p = NULL;
	pplist = NULL;
	return OK;
}

void test()
{
	compiler_info *list = NULL;
	char *(a[5]) = {"ln1", "ln2"};	
	compiler_info_push_front(&list, "cn", "cv", a, 2, 1, "cp");
	compiler_info_push_front(&list, "cn", "cv", a, 2, 0, "cp");
	PrintList(list);
	compiler_info_free(&list);
}


/*int main()
{
    test();
    return 0;
}*/
