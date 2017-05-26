/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "include/config.h"
#endif

#include "php.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include "php_ini.h"
#include "Zend/zend.h"
#include "ext/standard/info.h"
#include "ext/standard/md5.h"
#include "include/php_stwms.h"
#include "include/stwms_funcs.h"

/* If you declare any globals in php_stwms.h uncomment this:
 * 
 */
ZEND_DECLARE_MODULE_GLOBALS(stwms)


/* True global resources - no need for thread safety here */
static int le_stwms=0;

/* {{{ PHP_INI
 * Remove comments and fill if you need to have entries in php.ini
 */
 
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("stwms.version", "1.0.1", PHP_INI_ALL, OnUpdateLong, global_value, zend_stwms_globals, stwms_globals)
    STD_PHP_INI_ENTRY("stwms.name", "spring", PHP_INI_ALL, OnUpdateString, global_string, zend_stwms_globals, stwms_globals)
	STD_PHP_INI_ENTRY("stwms.email", "402085437@qq.com", PHP_INI_ALL, OnUpdateString, global_show, zend_stwms_globals, stwms_globals)
PHP_INI_END()

/* }}} */


/* *************用户可见函数************/
/* {{{ */
/*
 * stwms_var_dump(string arg)
 */
PHP_FUNCTION(stwms_test)
{
	char *para1,*ret1,*cstr1;
	zval *z,z1;
	zend_string *zkey;
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &z) == FAILURE) {
		return;  
	}
	//ret1="hello";
	//ZVAL_STRING(&z1,ret1); //将char * 转换为zval
	//zkey=zend_string_init(ret1,strlen(ret1),0); //将char * 转换为zend_string
	
	//cstr1=pestrdup("hello world",1); 
	para1=Z_STRVAL_P(z);
	if(para1){
		RETURN_STRING(para1);
	}else{
		RETURN_NULL();
	}
}


PHP_FUNCTION(stwms_array_concat)
{
    zval *arr, *prefix, *entry, *prefix_entry, value;
    zend_string *string_key, *result;
    zend_ulong num_key;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "aa", &arr, &prefix) == FAILURE) {
        return;
    }

    array_init_size(return_value, zend_hash_num_elements(Z_ARRVAL_P(arr)));

    ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(arr), num_key, string_key, entry) {
        if (string_key && zend_hash_exists(Z_ARRVAL_P(prefix), string_key)) {
            prefix_entry = zend_hash_find(Z_ARRVAL_P(prefix), string_key);
            if (Z_TYPE_P(entry) == IS_STRING && prefix_entry != NULL && Z_TYPE_P(prefix_entry) == IS_STRING) {
                result = strpprintf(0, "%s%s", Z_STRVAL_P(prefix_entry), Z_STRVAL_P(entry));
                ZVAL_STR(&value, result);
                zend_hash_update(Z_ARRVAL_P(return_value), string_key, &value);
            }   
        } else if (string_key == NULL && zend_hash_index_exists(Z_ARRVAL_P(prefix), num_key)){
            prefix_entry = zend_hash_index_find(Z_ARRVAL_P(prefix), num_key);
            if (Z_TYPE_P(entry) == IS_STRING && prefix_entry != NULL && Z_TYPE_P(prefix_entry) == IS_STRING) {
                result = strpprintf(0, "%s%s", Z_STRVAL_P(prefix_entry), Z_STRVAL_P(entry));
                ZVAL_STR(&value, result);
                zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, &value);
            }
        } else if (string_key) {
            zend_hash_update(Z_ARRVAL_P(return_value), string_key, entry);
            zval_add_ref(entry);
        } else  {
            zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, entry);
            zval_add_ref(entry);
        }
    }ZEND_HASH_FOREACH_END();

}

/*
 * stwms_makecode(string clearKey,string key,string symbol)
 */
PHP_FUNCTION(stwms_makecode)
{
	zend_string *zclearKey=NULL,*zkey=NULL,*zsymbol=NULL;
	char *clearKey="",*key="",*symbol="";
	char pass[33];
    
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S|SS", &zclearKey,&zkey,&zsymbol) == FAILURE) {
		return;  
	}

	if(zclearKey && ZSTR_LEN(zclearKey)>0){
		clearKey=ZSTR_VAL(zclearKey);
	}
	if(zkey && ZSTR_LEN(zkey)>0){
		key=ZSTR_VAL(zkey);
	}
	if(zsymbol && ZSTR_LEN(zsymbol)>0){
		symbol=ZSTR_VAL(zsymbol);
	}

#ifdef PHP_STWMS_DEBUG
	php_printf("clearKeyLen:%d,clearKey:%s\n",strlen(clearKey),clearKey);
	php_printf("keyLen:%d,key:%s\n",strlen(key),key);
	php_printf("symbol:%d,symbol:%s\n",strlen(symbol),symbol);
#endif
	stwms_makecode(pass,clearKey,key,symbol);
	RETURN_STRING(pass);	
}


/*
 * stwms_var_dump(string arg)
 */
PHP_FUNCTION(stwms_var_dump)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;
	zval *z;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &z) == FAILURE) {
		return;  
	}
	
	php_printf("var_dumps(%d):<pre>",le_stwms++);
	stwms_var_dump(z, 2);
	php_printf("\n</pre>");
	RETURN_NULL();
}

/* }}} */



/* {{{ php_stwms_init_globals
 * Uncomment this function if you have INI entries
 */

static void php_stwms_init_globals(zend_stwms_globals *stwms_globals)
{
	stwms_globals->global_value = 0;
	stwms_globals->global_string = NULL;
	stwms_globals->global_show = NULL;
}

zend_bool stwms_global_callback(zend_string *name TSRMLS_DC)
{
	zval gvalue;
	ZVAL_STRING(&gvalue,"hello"); 
//	array_init(&gvalue);
//	/* 增加 些数值 */
//	add_next_index_long(&gvalue, 1);
//	add_next_index_long(&gvalue, 20);
//	add_next_index_long(&gvalue, 300);
//	add_assoc_long(&gvalue, "life", 42);
//	add_assoc_string(&gvalue, "name", "spring");
	zend_hash_update(&EG(symbol_table), name, &gvalue);
	return SUCCESS;
}

/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(stwms)
{
	REGISTER_INI_ENTRIES();
	//注册全局变量
	//zend_register_auto_global(zend_string_init("_STWMS", sizeof("_STWMS")-1, 1),1, stwms_global_callback TSRMLS_DC);
	
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(stwms)
{
	/* uncomment this line if you have INI entries
	 * 
	 */
	UNREGISTER_INI_ENTRIES();
	
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(stwms)
{
#if defined(COMPILE_DL_STWMS) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(stwms)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(stwms)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "stwms support", "enabled");
	php_info_print_table_row(2,"author","Xie Chunping");
	php_info_print_table_row(2,"contact","spring.wind2006@163.com");
	php_info_print_table_end();

	/* 
	 * Remove comments if you have entries in php.ini
	 */
	DISPLAY_INI_ENTRIES();
	
}
/* }}} */

/* {{{ stwms_functions[]
 *
 * Every user visible function must have an entry in stwms_functions[].
 */
const zend_function_entry stwms_functions[] = {
	PHP_FE(stwms_var_dump,	NULL)		/* display every type var. */
	PHP_FE(stwms_makecode,	NULL)		/* make password. */
	PHP_FE(stwms_test,	NULL)		/* for test. */
	PHP_FE(stwms_array_concat,	NULL)		/* array concat. */
	PHP_FE_END	/* Must be the last line in stwms_functions[] */
};
/* }}} */

/* {{{ stwms_module_entry
 */
zend_module_entry stwms_module_entry = {
	STANDARD_MODULE_HEADER,
	"stwms",
	stwms_functions,
	PHP_MINIT(stwms),
	PHP_MSHUTDOWN(stwms),
	PHP_RINIT(stwms),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(stwms),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(stwms),
	PHP_STWMS_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_STWMS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(stwms)
#endif

