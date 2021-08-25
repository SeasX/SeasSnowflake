/*
  +----------------------------------------------------------------------+
  | SeasSnowflake                                                            |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2021 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:  SeasX Group <rock@php.net>                       |
  +----------------------------------------------------------------------+
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

extern "C" {
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "Zend/zend_exceptions.h"
#include "php7_wrapper.h"
}

#include "php_SeasSnowflake.h"
#include <iostream>
#include <map>
#include <sstream>
#include "./src/Timer.h"
#include "./src/IdWorker.h"

using namespace SeasSnowFlake::Src;
// using namespace clickhouse;
using namespace std;

zend_class_entry *SeasSnowflake_ce;

#ifdef COMPILE_DL_SEASSNOWFLAKE
extern "C" {
    ZEND_GET_MODULE(SeasSnowflake)
}
#endif

PHP_FUNCTION(SeasSnowflake_version)
{
    SC_RETURN_STRINGL(PHP_SEASSNOWFLAKE_VERSION, strlen(PHP_SEASSNOWFLAKE_VERSION));
}

static PHP_METHOD(SEASSNOWFLAKE_RES_NAME, __construct);
static PHP_METHOD(SEASSNOWFLAKE_RES_NAME, degenerate);
static PHP_METHOD(SEASSNOWFLAKE_RES_NAME, generate);

ZEND_BEGIN_ARG_INFO_EX(SeasSnowflake_construct, 0, 0, 1)
ZEND_ARG_INFO(0, parames)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(SeasSnowflake_generate, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(SeasSnowflake_degenerate, 0, 0, 1)
ZEND_ARG_INFO(0, id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(seasSnowflake_get_version_arginfo, 0, 0, 0)
ZEND_END_ARG_INFO()

/*   */
const zend_function_entry SeasSnowflake_functions[] =
{
    PHP_FE(SeasSnowflake_version,	seasSnowflake_get_version_arginfo)
    PHP_FE_END
};
/* }}} */


const zend_function_entry SeasSnowflake_methods[] =
{
       PHP_ME(SEASSNOWFLAKE_RES_NAME, __construct, SeasSnowflake_construct, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)  
       PHP_ME(SEASSNOWFLAKE_RES_NAME, generate, SeasSnowflake_generate, ZEND_ACC_PUBLIC )
       PHP_ME(SEASSNOWFLAKE_RES_NAME, degenerate, SeasSnowflake_degenerate, ZEND_ACC_PUBLIC) 
       PHP_FE_END
};

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(SeasSnowflake)
{
    zend_class_entry SeasSnowflake;
    INIT_CLASS_ENTRY(SeasSnowflake, SEASSNOWFLAKE_RES_NAME, SeasSnowflake_methods);
#if PHP_VERSION_ID >= 70000
    SeasSnowflake_ce = zend_register_internal_class_ex(&SeasSnowflake, NULL);
#else
    SeasSnowflake_ce = zend_register_internal_class_ex(&SeasSnowflake, NULL, NULL TSRMLS_CC);
#endif
    SeasSnowflake_ce->ce_flags |= ZEND_ACC_FINAL;
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(SeasSnowflake)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(SeasSnowflake)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(SeasSnowflake)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(SeasSnowflake)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "SeasSnowflake support", "enabled");
    php_info_print_table_row(2, "Version", PHP_SEASSNOWFLAKE_VERSION);
    php_info_print_table_row(2, "Author", "SeasX Group[email: rock@php.net]");
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ SeasSnowflake_module_entry
 */
zend_module_entry SeasSnowflake_module_entry =
{
    STANDARD_MODULE_HEADER,
    SEASSNOWFLAKE_RES_NAME,
    SeasSnowflake_functions,
    PHP_MINIT(SeasSnowflake),
    PHP_MSHUTDOWN(SeasSnowflake),
    PHP_RINIT(SeasSnowflake),
    PHP_RSHUTDOWN(SeasSnowflake),
    PHP_MINFO(SeasSnowflake),
    PHP_SEASSNOWFLAKE_VERSION,
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

PHP_METHOD(SEASSNOWFLAKE_RES_NAME, __construct)
{
    zval *parames;

#ifndef FAST_ZPP
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "z", &parames) == FAILURE)
    {
        return;
    }
#else
#undef IS_UNDEF
#define IS_UNDEF Z_EXPECTED_LONG
    ZEND_PARSE_PARAMETERS_START(1, 1)
    Z_PARAM_ARRAY(parames)
    ZEND_PARSE_PARAMETERS_END();
#undef IS_UNDEF
#define IS_UNDEF 0
#endif

    HashTable *_ht = Z_ARRVAL_P(parames);
    zval *value;

    zval *this_obj;
    this_obj = getThis();
  
 

    try
    {
#if PHP_VERSION_ID < 80000
        if (php_array_get_value(_ht, "worker_id", value))
        {
            convert_to_long(value);
            zend_update_property_long(SeasSnowflake_ce, this_obj, "worker_id", sizeof("worker_id") - 1, Z_LVAL_P(value) TSRMLS_CC);
        }

        if (php_array_get_value(_ht, "datacenter_id", value))
        {
            convert_to_long(value);
            zend_update_property_long(SeasSnowflake_ce, this_obj, "datacenter_id", sizeof("datacenter_id") - 1, Z_LVAL_P(value) TSRMLS_CC);
        }
#else
        if (php_array_get_value(_ht, "worker_id", value))
        {
            convert_to_long(value);
            zend_object *workerObject;
            workerObject=Z_OBJ_P(this_obj);
            zend_update_property_long(SeasSnowflake_ce, workerObject, "worker_id", sizeof("worker_id") - 1, Z_LVAL_P(value) );
        }

        if (php_array_get_value(_ht, "datacenter_id", value))
        {
            convert_to_long(value);
            zend_object *datecenterObject;
            datecenterObject=Z_OBJ_P(this_obj);
            zend_update_property_long(SeasSnowflake_ce, datecenterObject, "datacenter_id", sizeof("datacenter_id") - 1, Z_LVAL_P(value) );
        }
#endif

        
    }
    catch (const std::exception& e)
    {
        #if PHP_VERSION_ID < 80000
          sc_zend_throw_exception(NULL, e.what(), 0 TSRMLS_CC);
        #else
          sc_zend_throw_exception(NULL, e.what(), 0 );
        #endif
      
    }

    RETURN_TRUE;
}
/* }}} */

/* 
 * create a id 
 */
PHP_METHOD(SEASSNOWFLAKE_RES_NAME, generate)
{
    auto &idWorker = Singleton<IdWorker>::instance();
    zval *this_obj;
    this_obj = getThis();
    zval *worker_id = sc_zend_read_property(SeasSnowflake_ce, this_obj, "worker_id", sizeof("worker_id") - 1, 0);
    zval *datacenter_id = sc_zend_read_property(SeasSnowflake_ce, this_obj, "datacenter_id", sizeof("datacenter_id") - 1, 0);

    if (zend_parse_parameters_none() == FAILURE)
    {
        return;
    }

    idWorker.setDatacenterId(Z_LVAL_P(worker_id));
    idWorker.setWorkerId(Z_LVAL_P(datacenter_id));

    try{
       uint64_t id=  idWorker.generate();
       RETVAL_LONG(id);
    }catch (const std::exception& e){
         #if PHP_VERSION_ID < 80000
          sc_zend_throw_exception(NULL, e.what(), 0 TSRMLS_CC);
        #else
          sc_zend_throw_exception(NULL, e.what(), 0 );
        #endif
    }
    
    return;
 }


 /* 
 *  
 * degenerate(string id)
 */
PHP_METHOD(SEASSNOWFLAKE_RES_NAME, degenerate)
{
    char *id = NULL;
    size_t l_id = 0;
    // zval* params = NULL;
    
    auto &idWorker = Singleton<IdWorker>::instance();
    zval *this_obj;
    this_obj = getThis();
    zval *worker_id = sc_zend_read_property(SeasSnowflake_ce, this_obj, "worker_id", sizeof("worker_id") - 1, 0);
    zval *datacenter_id = sc_zend_read_property(SeasSnowflake_ce, this_obj, "datacenter_id", sizeof("datacenter_id") - 1, 0);


    idWorker.setDatacenterId(Z_LVAL_P(worker_id));
    idWorker.setWorkerId(Z_LVAL_P(datacenter_id));

    #ifndef FAST_ZPP
        if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|z", &id, &l_id, &params) == FAILURE)
        {
            return;
        }
    #else
    #undef IS_UNDEF
    #define IS_UNDEF Z_EXPECTED_LONG
        ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_STRING(id, l_id)
        Z_PARAM_OPTIONAL
    //  Z_PARAM_ARRAY(params)
        ZEND_PARSE_PARAMETERS_END();
    #undef IS_UNDEF
    #define IS_UNDEF 0
    #endif

    try
    {
        uint64_t id_i = idWorker.stringToUINT64(id);
        uint64_t interval=idWorker.degenerateInterval(id_i);
        uint workerId=idWorker.degenerateWorkerId(id_i);
        uint sequence= idWorker.degenerateSequence(id_i);
        uint dataCenterId=idWorker.degenerateDataCenterId(id_i);
        uint64_t beginTimestamp=idWorker.degenerateBeginTimestamp(id_i);
        uint64_t timestamp= idWorker.degenerateTimestamp(id_i);
        zval arr;

        array_init(&arr);
        add_assoc_long_ex(&arr, "worker_id", strlen("worker_id"), workerId);
        add_assoc_long_ex(&arr, "datacenter_id", strlen("datacenter_id"), dataCenterId);
        add_assoc_long_ex(&arr, "sequence", strlen("sequence"), sequence);
        add_assoc_long_ex(&arr, "timestamp", strlen("timestamp"), timestamp);
        add_assoc_long_ex(&arr, "begin_timestamp", strlen("begin_timestamp"), beginTimestamp);
        add_assoc_long_ex(&arr, "interval", strlen("interval"), interval);

        RETURN_ZVAL(&arr, 0, 1);
     } catch (const std::exception& e){
         #if PHP_VERSION_ID < 80000
          sc_zend_throw_exception(NULL, e.what(), 0 TSRMLS_CC);
        #else
          sc_zend_throw_exception(NULL, e.what(), 0 );
        #endif
    }
   return ;
 }
/* }}} */

