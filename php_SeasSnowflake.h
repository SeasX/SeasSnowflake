/*
  +----------------------------------------------------------------------+
  | SeasSnowflake                                                            |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2018 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:  SeasX Group <rock@php.net>                          |
  +----------------------------------------------------------------------+
*/
#ifdef __cplusplus
#define __STDC_FORMAT_MACROS
#endif

#ifndef PHP_SEASSNOWFLAKE_H
#define PHP_SEASSNOWFLAKE_H

extern zend_module_entry SeasSnowflake_module_entry;
#define phpext_SeasSnowflake_ptr &SeasSnowflake_module_entry

#define PHP_SEASSNOWFLAKE_VERSION "1.0.0"

#ifdef PHP_WIN32
#	define PHP_SEASSNOWFLAKE_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_SEASSNOWFLAKE_API __attribute__ ((visibility("default")))
#else
#	define PHP_SEASSNOWFLAKE_API
#endif

extern "C" {
#ifdef ZTS
#include "TSRM.h"
#endif
}

typedef unsigned long ulong_t;

#ifdef ZTS
#define SEASSNOWFLAKE_G(v) TSRMG(SeasSnowflake_globals_id, zend_SeasSnowflake_globals *, v)
#else
#define SEASSNOWFLAKE_G(v) (SeasSnowflake_globals.v)
#endif

#define SEASSNOWFLAKE_RES_NAME "SeasSnowflake"

#endif	/* PHP_SEASSNOWFLAKE_H */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
