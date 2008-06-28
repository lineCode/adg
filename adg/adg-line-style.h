/* ADG - Automatic Drawing Generation
 * Copyright (C) 2007-2008, Nicola Fontana <ntd at entidi.it>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the 
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307, USA.
 */


#ifndef __ADG_LINE_STYLE_H__
#define __ADG_LINE_STYLE_H__

#include <adg/adg-style.h>
#include <adg/adg-enums.h>


G_BEGIN_DECLS


#define ADG_TYPE_LINE_STYLE             (adg_line_style_get_type ())
#define ADG_LINE_STYLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), ADG_TYPE_LINE_STYLE, AdgLineStyle))
#define ADG_LINE_STYLE_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), ADG_TYPE_LINE_STYLE, AdgLineStyleClass))
#define ADG_IS_LINE_STYLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ADG_TYPE_LINE_STYLE))
#define ADG_IS_LINE_STYLE_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), ADG_TYPE_LINE_STYLE))
#define ADG_LINE_STYLE_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), ADG_TYPE_LINE_STYLE, AdgLineStyleClass))

/* AdgLineStyle declared in adg-style.h */
typedef struct _AdgLineStyleClass   AdgLineStyleClass;
typedef struct _AdgLineStylePrivate AdgLineStylePrivate;

struct _AdgLineStyle
{
  AdgStyle		 style;

  /*< private >*/
  AdgLineStylePrivate	*priv;
};

struct _AdgLineStyleClass
{
  AdgStyleClass		 parent_class;
};


GType		adg_line_style_get_type	(void) G_GNUC_CONST;
AdgStyle *	adg_line_style_new	(void);
AdgStyle *	adg_line_style_from_id	(AdgLineStyleId		 id);
void		adg_line_style_apply	(const AdgLineStyle	*line_style,
					 cairo_t		*cr);


G_END_DECLS


#endif /* __ADG_LINE_STYLE_H__ */
