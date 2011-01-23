/* ADG - Automatic Drawing Generation
 * Copyright (C) 2007,2008,2009,2010,2011  Nicola Fontana <ntd at entidi.it>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

/*
 * This header is included by every .c files of the library to
 * enable the inclusion of the internal headers and initialize
 * some common stuff.
 */

#ifndef __ADG_INTERNAL_H__
#define __ADG_INTERNAL_H__


/* The following define enables the inclusion of internal headers */
#define __ADG_H__

#include <config.h>
#define G_LOG_DOMAIN  PACKAGE
#include "adg-utils.h"


#ifdef ENABLE_NLS

#include <libintl.h>

#ifndef GETTEXT_PACKAGE
#error You must define GETTEXT_PACKAGE before including adg-internal.h.  Did you forget to include config.h?
#endif

#ifdef gettext_noop
#define N_(String)              gettext_noop(String)
#else
#define N_(String)              (String)
#endif

#define _(String)               _adg_dgettext(GETTEXT_PACKAGE, String)
#define P_(String)              _adg_dgettext(GETTEXT_PACKAGE "-properties", String)
#define Q_(String)              _adg_dpgettext(GETTEXT_PACKAGE, String, 0)
#define C_(Context,String)      _adg_dpgettext(GETTEXT_PACKAGE, Context "\004" String, strlen(Context) + 1)
#define NC_(Context,String)     N_(String)

#else /* !ENABLE_NLS */

#define _(String)               (String)
#define P_(String)              (String)
#define Q_(String)              (String)
#define N_(String)              (String)
#define C_(Context,String)      (String)
#define NC_(Context, String)    (String)

#endif


G_CONST_RETURN gchar *  _adg_dgettext   (const gchar *domain,
                                         const gchar *msgid) G_GNUC_FORMAT(2);
G_CONST_RETURN gchar *  _adg_dpgettext  (const gchar *domain,
                                         const gchar *msgctxtid,
                                         gsize        msgidoffset) G_GNUC_FORMAT(2);


#endif /* __ADG_INTERNAL_H__ */
