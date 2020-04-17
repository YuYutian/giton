#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exjson.h"
#include "status.h"
#include "compiler_info_list.h"

#define MAX_BUF_LENGTH 102400
#define MAX_STRING_LENGTH 512

int getCompilerPath(char *buf, char *whichpath, char *cn);
status getCompilerVersion(char *buf, char *cp, char *ver);
compiler_info *compiler_info_new_node(char *cn, char *cv, char **ln, int lnn, short lt, char *cp);
status compiler_info_push_front(compiler_info **pplist, char *cn, char *cv, char **ln, int lnn, short lt, char *cp);
void PrintList(compiler_info *plist);
status compiler_info_free(compiler_info** pplist);

status readConfig(EXJSON **v, char config_path[])
{
    FILE *fp = NULL;
    char child_config_buf[MAX_BUF_LENGTH] = {0};
	fp = fopen(config_path,"r"); /*child_config*/
	if(fp == NULL)
	{
		return CONFIG_ERROR;
	}
	fread(child_config_buf, MAX_BUF_LENGTH, 1, fp);
	*v = decode_json(child_config_buf);
	fclose(fp);
	return OK;
}

status getAllCompilersInfo(compiler_info **list, char *whichpath, EXJSON *TargetLanguage)
{
    int index = 0;
    int ret = 0;
    char *name = NULL;
    int *type = NULL;
    EXJSON *target = NULL;
    char *version = NULL;
    EXJSON *TargetLanguageItem = exjson_get_val_from_index(TargetLanguage, index);
    
    char cp[MAX_STRING_LENGTH] = {0};
    char cv[MAX_STRING_LENGTH] = {0};
    
    while(NULL != TargetLanguageItem)
    {
        name = exjson_get_val_from_key(TargetLanguageItem, "name");
        type = exjson_get_val_from_key(TargetLanguageItem, "type");
        target = exjson_get_val_from_key(TargetLanguageItem, "target");
        version = exjson_get_val_from_key(TargetLanguageItem, "version");
        memset(cp, '\0', MAX_STRING_LENGTH);
        ret = getCompilerPath(cp, whichpath, name);
        switch(ret)
        {
            case 0: memset(cp, '\0', MAX_STRING_LENGTH); break;
            case 1: break;
            default: return NOT_COMPLETED;
        }
        if(ret == 0)
        {
            index++;
            TargetLanguageItem = exjson_get_val_from_index(TargetLanguage, index);
            continue;
        }
        memset(cv, '\0', MAX_STRING_LENGTH); 
        switch(getCompilerVersion(cv, cp, version))
        {
            case OK: break;
            default: return NOT_COMPLETED;
        }
        //printf("[%s] [%d] [%s] [%s]\n", name, *type, cp, cv);
        int index2 = 0;
        int index3 = 0;
        char *LanguageNameItem = exjson_get_val_from_index(target, index2);
        while(NULL != LanguageNameItem)
        { 
	        index2++;
            LanguageNameItem = exjson_get_val_from_index(target, index2);  
	    }
	    char **ln = NULL;
        ln = (char **)malloc(sizeof(char *) * index2);
	    if (NULL == ln)
		    return NOT_COMPLETED;
	    for(index3 = 0; index3 < index2; index3++)
	    {
		    ln[index3] = (char*)malloc(sizeof(char) * index2);
		    if (NULL == ln[index3])
		    {
		        free(ln);/////////////////////////////////////////////////////////////////////////////////////////////!!!!
			    return NOT_COMPLETED;
			}
		    snprintf(ln[index3], LanguageNameLength, "%s", LanguageNameItem = exjson_get_val_from_index(target, index3));
		}	
        compiler_info_push_front(list, name, cv, ln, index2, *type, cp);
        for(index3 = 0; index3 < index2; index3++)
	    {
		    free(ln[index3]);
		}
		free(ln);
        index++;
        TargetLanguageItem = exjson_get_val_from_index(TargetLanguage, index);
    }
    return OK;
}

status writeData(char* output, compiler_info *plist)//打印链表
{
    EXJSON *exjson = INIT_EXJSON();
    EXJSON *list = INIT_EXJSON();  
    compiler_info *tail = plist;
    char ln[MAX_STRING_LENGTH] = {0};
    while (tail)
    {
        EXJSON *array = INIT_EXJSON();
        memset(ln, '\0', MAX_STRING_LENGTH);
        add_array_string(array, tail->CompilerName);
        add_array_string(array, tail->CompilerVersion);
        int index = 0;
	    for(;index<tail->lnn;index++)
	    {
	        strncat(ln, tail->LanguageName[index], LanguageNameLength);
	        strncat(ln, " ", 1);
	    }
	    ln[LanguageNameLength - 1] = '\0'; 
        add_array_string(array, ln);
        add_array_int(array, tail->LanguageType);
        add_array_string(array, tail->CompilerPath);
        //printf("CompilerName: %s, CompilerVersion: %s, LanguageType: %d, CompilerPath: %s, LanguageName: ", tail->CompilerName, tail->CompilerVersion, tail->LanguageType, tail->CompilerPath);
	    //printf("\n");
	    tail = tail->next;
	    add_array_array(list, array);
    }
    add_object_array(exjson, "compiler_list", list);
    char *str = encode_json(exjson);
    printf("%s", str);
    free(str);
    destroy_exjson(exjson);
    return 0;
}
	
int main(int argc, char *argv[])
{
    if(argc != 9)
        return ARGV_ERROR;
    EXJSON *v = NULL;
    if(readConfig(&v, argv[7]) < 0)
        return CONFIG_ERROR;
    int *ConfigID = exjson_get_val_from_key(v, "ConfigID");
    char *Type = exjson_get_val_from_key(v, "Type");
    char *Which = exjson_get_val_from_key(v, "Which");
	EXJSON *SourceCheckingRange = exjson_get_val_from_key(v, "SourceCheckingRange");
	EXJSON *TargetLanguage = exjson_get_val_from_key(v, "TargetLanguage");
	EXJSON *SourceType = exjson_get_val_from_key(v, "SourceType");
    //printf("scr:%s", (char*)(exjson_get_val_from_index(SourceCheckingRange, 0)));
    compiler_info *list = NULL;
    getAllCompilersInfo(&list, Which, TargetLanguage);
    writeData("", list);
    
    compiler_info_free(&list);
    destroy_exjson(v);
    return OK;
}
