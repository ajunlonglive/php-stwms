/*
 * stwms_funcs.c
 *
 *  Created on: 2017年5月12日
 *      Author: xiechunping
 */

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/md5.h"
#include "Zend/zend.h"
#include "include/stwms_funcs.h"

/**
 *  
 */
int stwms_makecode(char *pass,char *clearKey,char *key,char *symbol)
{
	int clearKeyLen=0,keyLen=0,symbolLen=0;
	char *default_symbol="!@#$%^*()";
	int symbols[256];
	char md5str[33];
	char sec1[233];
	char sec2[233];
	int i=0,k=0,dec=0;
	clearKeyLen=strlen(clearKey);
	keyLen=strlen(key);
	symbolLen=strlen(symbol);
	
	if(!keyLen){
		key=clearKey+(int)floor(clearKeyLen/2);
		keyLen=strlen(key);
	}
	
	if(!symbolLen){
		symbol=default_symbol;
		symbolLen=strlen(symbol);
	}
	
	for(i=0;i<symbolLen;i++){
		symbols[i]=(int)*(symbol+i);
	}

	stwms_sort(symbols,symbolLen);
	
	stwms_md5(sec1,clearKey);
	strncat(sec1,key,233-keyLen);
	stwms_md5(sec1,sec1);	
	
	stwms_md5(sec2,key);
	strncat(sec2,clearKey,233-clearKeyLen);
	stwms_md5(sec2,sec2);

	for(i=0;i<32;i++){
		dec=(int)(sec1[i])+(int)(sec2[i]);
		if(dec>126){
			dec=dec-126;
			if(dec>126){
				dec=dec-126;
			}
			if(dec<33){
				dec+=33;
			}
		}
		if((dec<48 || (dec>57 && dec<65) || (dec>90 && dec<97) || dec>122) && symbolLen){
			char in_array=0;
			for(k=0;k<symbolLen;k++){
				if(symbols[k]==dec){
					in_array=1;
					break;
				}
			}
			if(!in_array){
				dec=symbols[(i<symbolLen?i:i%symbolLen)];
			}
		}
		pass[i]=(char)dec;
	}
	
	pass[i]='\0';
	return 0;
}

int stwms_md5(char *pmd5str,char *arg)
{
	PHP_MD5_CTX context;
	unsigned char digest[16];
	
	PHP_MD5Init(&context);
	PHP_MD5Update(&context, arg, strlen(arg));
	PHP_MD5Final(digest, &context);	
	make_digest_ex(pmd5str, digest, 16);
	return 0;
}

void stwms_sort(int *arr, int len)//arr为数组地址，len为数组长度。
{
    int i, j;
    int v;
    //排序主体
    for(i = 0; i < len - 1; i ++)
    {
        for(j = i+1; j < len; j ++)
        {
            if(arr[i] > arr[j])//如前面的比后面的大，则交换。
            {
                v = arr[i];
                arr[i] = arr[j];
                arr[j] = v;
            }
        }
    }
}

int stwms_var_dump(zval *z, int level) 
{
	HashTable *ht;
	int ret;
	zend_string *key;
	zend_ulong index;
	zval *pData;
	
	switch (Z_TYPE_P(z)) {
	case IS_NULL:
		php_printf("%*stype = null, refcount = %d%s\n", level * 4, "",
				Z_REFCOUNT_P(z),
				Z_ISREF_P(z) ? ", is_ref " : "");
		break;
	
	case IS_FALSE:
		php_printf("%*stype = bool, refcount = -1, value = false\n", level * 4,""); 
		break;
		
	case IS_TRUE:
		php_printf("%*stype = bool, refcount = -1, value = true\n", level * 4,""); 
		break;

	case IS_LONG:
		php_printf("%*stype = long, refcount =-1 %s, value = \"%ld\"\n", 
				level * 4, "", Z_ISREF_P(z) ? ", is_ref " : "",zval_get_long(z));
		break;

	case IS_STRING:
		php_printf(
				"%*stype = string, refcount = %d%s, value = \"%s\", len = %d\n",
				level * 4, "", Z_REFCOUNT_P(z), Z_ISREF_P(z) ? ", is_ref " : "",
				Z_STRVAL_P(z), Z_STRLEN_P(z));
		break;
		break;
		
	case IS_DOUBLE:
		php_printf("%*stype = double, refcount = -1%s, value = %0.6f\n",
				level * 4, "", Z_ISREF_P(z) ? ", is_ref " : "",
				zval_get_double(z));

		break;

	case IS_RESOURCE:
		php_printf("%*stype = resource, refcount = %d%s, resource_id = %d\n",
				level * 4, "", Z_REFCOUNT_P(z), Z_ISREF_P(z) ? ", is_ref " : "",
				Z_RES_VAL_P(z));

		break;

	case IS_ARRAY:
		ht = Z_ARRVAL_P(z);

		zend_hash_internal_pointer_reset(ht);
		php_printf("%*stype = array, refcount = %d%s, value = %s\n", level * 4,
				"", Z_REFCOUNT_P(z), Z_ISREF_P(z) ? ", is_ref " : "",
						!zend_hash_has_more_elements(ht) ? "" : "empty");

		while ( HASH_KEY_NON_EXISTENT!=(ret = zend_hash_get_current_key(ht, &key, &index))) { 
			if ( HASH_KEY_IS_STRING == ret) {
				php_printf("%*skey is string \"%s\"", (level + 1) * 4, "", key->val);
			} else if (HASH_KEY_IS_LONG == ret) {
				php_printf("%*skey is long %d", (level + 1) * 4, "", index);
			}
			pData = zend_hash_get_current_data(ht);
			stwms_var_dump(pData, level + 1);
			zend_hash_move_forward(ht);
		}
		zend_hash_internal_pointer_end(Z_ARRVAL_P(z));
		break;
	case IS_OBJECT:
		php_printf("%*stype = object, refcount = %d%s\n", level * 4, "",
				Z_REFCOUNT_P(z),
				Z_ISREF_P(z) ? ", is_ref " : "");
		break;
	default:
		php_printf("%*sunknown type, refcount = %d%s\n", level * 4, "",
				Z_REFCOUNT_P(z),
				Z_ISREF_P(z) ? ", is_ref " : "");
		break;
	}
	return 0;
}

