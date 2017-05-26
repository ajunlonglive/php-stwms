/*
 * stwms_funcs.h
 *
 *  Created on: 2017年5月12日
 *      Author: xiechunping
 */

#ifndef __EXT_STWMS_INCLUDE_STWMS_FUNCS_H__
#define __EXT_STWMS_INCLUDE_STWMS_FUNCS_H__
#include "zend.h"

int stwms_makecode(char *pass,char *clearKey,char *key,char *symbol);
int stwms_md5(char *pmd5str,char *arg);
int stwms_var_dump(zval *z, int level);
void stwms_sort(int *arr, int len);
#endif /* __EXT_STWMS_INCLUDE_STWMS_FUNCS_H__ */
