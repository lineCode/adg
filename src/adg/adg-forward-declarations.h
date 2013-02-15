/* ADG - Automatic Drawing Generation
 * Copyright (C) 2007,2008,2009,2010,2011,2012  Nicola Fontana <ntd at entidi.it>
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


#if !defined(__ADG_H__)
#error "Only <adg.h> can be included directly."
#endif


#ifndef __ADG_FORWARD_DECLARATIONS_H__
#define __ADG_FORWARD_DECLARATIONS_H__


G_BEGIN_DECLS

/* Forward declarations are kept in a separate file so it can be
 * easily skipped by gtkdoc-scan: gtk-doc is inclined to mess up
 * sections when forward declarations are used.
 */

/* Needed by adg-entity.h */
typedef struct _AdgCanvas       AdgCanvas;
typedef struct _AdgStyle        AdgStyle;
typedef struct _AdgPoint        AdgPoint;
typedef gint                    AdgDress;

/* Needed by adg-table.h */
typedef struct _AdgTableRow     AdgTableRow;
typedef struct _AdgTableCell    AdgTableCell;

G_END_DECLS


#endif /* __ADG_FORWARD_DECLARATIONS_H__ */
