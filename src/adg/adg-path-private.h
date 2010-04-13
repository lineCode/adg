/* ADG - Automatic Drawing Generation
 * Copyright (C) 2007,2008,2009,2010  Nicola Fontana <ntd at entidi.it>
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


#ifndef __ADG_PATH_PRIVATE_H__
#define __ADG_PATH_PRIVATE_H__

#include "adg-pair.h"


G_BEGIN_DECLS

typedef struct _AdgNamedPair     AdgNamedPair;
typedef enum   _AdgAction        AdgAction;
typedef struct _AdgOperation     AdgOperation;
typedef struct _AdgPathPrivate   AdgPathPrivate;

struct _AdgNamedPair {
    const gchar *name;
    AdgPair      pair;
};

enum _AdgAction {
    ADG_ACTION_NONE,
    ADG_ACTION_CHAMFER,
    ADG_ACTION_FILLET
};

struct _AdgOperation{
    AdgAction action;

    union {
        struct {
            gdouble delta1, delta2;
        } chamfer;
        struct {
            gdouble radius;
        } fillet;
    } data;

};

struct _AdgPathPrivate {
    gboolean             cp_is_valid;
    AdgPair              cp;

    struct {
        CpmlPath         path;
        GArray          *array;
    }                    cpml;

    CpmlPrimitive        last;
    CpmlPrimitive        over;
    AdgOperation         operation;
};

G_END_DECLS


#endif /* __ADG_PATH_PRIVATE_H__ */