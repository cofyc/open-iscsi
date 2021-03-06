/*
 * Copyright (C) 2017 Red Hat, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Gris Ge <fge@redhat.com>
 */
#ifndef __ISCSI_USR_MISC_H__
#define __ISCSI_USR_MISC_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>
#include <dirent.h>

#include "libopeniscsiusr/libopeniscsiusr.h"

#define _good(rc, rc_val, out) \
	do { \
		rc_val = rc; \
		if (rc_val != LIBISCSI_OK) \
			goto out; \
	} while(0)

__DLL_LOCAL void _iscsi_log(struct iscsi_context *ctx, int priority,
			    const char *file, int line, const char *func_name,
			    const char *format, ...);
__DLL_LOCAL void _iscsi_log_stderr(struct iscsi_context *ctx, int priority,
				   const char *file, int line,
				   const char *func_name, const char *format,
				   va_list args);

#define _iscsi_log_cond(ctx, prio, arg...) \
	do { \
		if ((ctx != NULL) && \
		    (iscsi_context_log_priority_get(ctx) >= prio)) \
			_iscsi_log(ctx, prio, __FILE__, __LINE__, \
				   __FUNCTION__, ## arg); \
	} while (0)

#define _debug(ctx, arg...) \
	_iscsi_log_cond(ctx, LIBISCSI_LOG_PRIORITY_DEBUG, ## arg)
#define _info(ctx, arg...) \
	_iscsi_log_cond(ctx, LIBISCSI_LOG_PRIORITY_INFO, ## arg)
#define _warn(ctx, arg...) \
	_iscsi_log_cond(ctx, LIBISCSI_LOG_PRIORITY_WARNING, ## arg)
#define _error(ctx, arg...) \
	_iscsi_log_cond(ctx, LIBISCSI_LOG_PRIORITY_ERROR, ## arg)

#define _iscsi_getter_func_gen(struct_name, prop_name, prop_type) \
	prop_type struct_name##_##prop_name##_get(struct struct_name *d) \
	{ \
		assert(d != NULL); \
		return d->prop_name; \
	}

/*
 * Check pointer returned by malloc() or strdup() or calloc(), if NULL, set
 * rc as LIBISCSI_ERR_NO_MEMORY, report error and goto goto_out.
 */
#define _alloc_null_check(ctx, ptr, rc, goto_out) \
	do { \
		if (ptr == NULL) { \
			rc = LIBISCSI_ERR_NOMEM; \
			_error(ctx, iscsi_strerror(rc)); \
			goto goto_out; \
		} \
	} while(0)

__DLL_LOCAL int _scan_filter_skip_dot(const struct dirent *dir);

__DLL_LOCAL bool _file_exists(const char *path);

#endif /* End of __ISCSI_USR_MISC_H__ */
