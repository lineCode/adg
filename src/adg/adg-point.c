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


/**
 * SECTION:adg-point
 * @Section_Id:AdgPoint
 * @title: AdgPoint
 * @short_description: A struct holding x, y coordinates
 *                     (either named or explicit)
 *
 * AdgPoint is an opaque structure that manages 2D coordinates,
 * either set explicitely throught adg_point_set_pair() and
 * adg_point_set_pair_explicit() or taken from a model with
 * adg_point_set_pair_from_model(). It can be thought as an
 * #AdgPair on steroid, because it adds named pair support to
 * a simple pair, enabling coordinates depending on #AdgModel.
 **/

/**
 * AdgPoint:
 *
 * This is an opaque struct: all its fields are privates.
 **/


#include "adg-internal.h"
#include "adg-point.h"
#include <string.h>


struct _AdgPoint {
    AdgPair      pair;
    AdgModel    *model;
    gchar       *name;
    gboolean     is_uptodate;
};


GType
adg_point_get_type(void)
{
    static int type = 0;

    if (G_UNLIKELY(type == 0))
        type = g_boxed_type_register_static("AdgPoint",
                                            (GBoxedCopyFunc) adg_point_dup,
                                            (GBoxedFreeFunc) adg_point_destroy);

    return type;
}

/**
 * adg_point_new:
 *
 * Creates a new empty #AdgPoint. The returned pointer
 * should be freed with adg_point_destroy() when no longer needed.
 *
 * The returned value should be freed with adg_point_destroy()
 * when no longer needed.
 *
 * Returns: a newly created #AdgPoint
 **/
AdgPoint *
adg_point_new(void)
{
    return g_new0(AdgPoint, 1);
}

/**
 * adg_point_dup:
 * @src: an #AdgPoint
 *
 * Duplicates @src. This operation also adds a new reference
 * to the internal model if @src is linked to a named pair.
 *
 * The returned value should be freed with adg_point_destroy()
 * when no longer needed.
 *
 * Returns: the duplicated #AdgPoint struct or %NULL on errors
 **/
AdgPoint *
adg_point_dup(const AdgPoint *src)
{
    AdgPoint *point;

    g_return_val_if_fail(src != NULL, NULL);

    if (src->model)
        g_object_ref(src->model);

    point = g_memdup(src, sizeof(AdgPoint));
    point->name = g_strdup(src->name);

    return point;
}

/**
 * adg_point_destroy:
 * @point: an #AdgPoint
 *
 * Destroys the @point instance, unreferencing the internal model if
 * @point is linked to a named pair.
 **/
void
adg_point_destroy(AdgPoint *point)
{
    g_return_if_fail(point != NULL);

    adg_point_set_pair_from_model(point, NULL, NULL);

    g_free(point);
}

/**
 * adg_point_copy:
 * @point: an #AdgPoint
 * @src: the source point to copy
 *
 * Copies @src into @point. If @point was linked to a named pair, the
 * reference to the old model is dropped. Similary, if @src is linked
 * to a model, a new reference to this new model is added.
 **/
void
adg_point_copy(AdgPoint *point, const AdgPoint *src)
{
    g_return_if_fail(point != NULL);
    g_return_if_fail(src != NULL);

    if (src->model != NULL)
        g_object_ref(src->model);
    if (point->model != NULL)
        g_object_unref(point->model);

    if (point->name != NULL)
        g_free(point->name);

    memcpy(point, src, sizeof(AdgPoint));
    point->name = g_strdup(src->name);
}

/**
 * adg_point_set:
 * @p_point: a pointer to an #AdgPoint
 * @new_point: the new point to assign
 *
 * Convenient method to assign @new_point to the #AdgPoint pointed
 * by @p_point. This is useful while implementing #AdgPoint
 * property setters.
 *
 * At the end *@p_point will be set to @new_point, allocating a new
 * #AdgPoint or destroying the previous one when necessary. It is
 * allowed to use %NULL as @new_point to unset *@p_point.
 *
 * Returns: %TRUE if the pointer pointed by @p_point has been changed,
 *          %FALSE otherwise
 **/
gboolean
adg_point_set(AdgPoint **p_point, const AdgPoint *new_point)
{
    g_return_val_if_fail(p_point != NULL, FALSE);

    if ((*p_point == new_point) ||
        (*p_point && new_point && adg_point_equal(*p_point, new_point)))
        return FALSE;

    if (*p_point == NULL)
        *p_point = adg_point_new();

    if (new_point) {
        adg_point_copy(*p_point, new_point);
    } else {
        adg_point_destroy(*p_point);
        *p_point = NULL;
    }

    return TRUE;
}

/**
 * adg_point_set_pair:
 * @point: an #AdgPoint
 * @pair: the #AdgPair to use
 *
 * Sets an explicit pair in @point by using the given @pair. If
 * @point was linked to a named pair in a model, this link is
 * dropped before setting the pair.
 **/
void
adg_point_set_pair(AdgPoint *point, const AdgPair *pair)
{
    g_return_if_fail(point != NULL);
    g_return_if_fail(pair != NULL);

    adg_point_set_pair_explicit(point, pair->x, pair->y);
}

/**
 * adg_point_set_pair_explicit:
 * @point: an #AdgPoint
 * @x: the x coordinate of the point
 * @y: the y coordinate of the point
 *
 * Works in the same way of adg_point_set_pair() but accept direct numbers
 * instead of an #AdgPair structure.
 **/
void
adg_point_set_pair_explicit(AdgPoint *point, gdouble x, gdouble y)
{
    g_return_if_fail(point != NULL);

    /* Unlink the named pair dependency, if any */
    adg_point_set_pair_from_model(point, NULL, NULL);

    point->pair.x = x;
    point->pair.y = y;
    point->is_uptodate = TRUE;
}

/**
 * adg_point_set_pair_from_model:
 * @point: an #AdgPoint
 * @model: the #AdgModel
 * @name: the id of a named pair in @model
 *
 * Links the @name named pair of @model to @point, so any subsequent
 * call to adg_point_update() will read the named pair content. A
 * new reference is added to @model while the previous model (if any)
 * is unreferenced.
 *
 * It is allowed to use %NULL as @model, in which case the possible
 * link between @point and the named pair is dropped.
 **/
void
adg_point_set_pair_from_model(AdgPoint *point,
                              AdgModel *model, const gchar *name)
{
    g_return_if_fail(point != NULL);
    g_return_if_fail(model == NULL || name != NULL);

    /* Check if unlinking a yet unlinked point */
    if (model == NULL && point->model == NULL)
        return;

    /* Return if the named pair is not changed */
    if (model == point->model &&
        (model == NULL || strcmp(point->name, name) == 0)) {
        return;
    }

    if (model != NULL)
        g_object_ref(model);

    if (point->model != NULL) {
        /* Remove the old named pair */
        g_object_unref(point->model);
        g_free(point->name);
        point->model = NULL;
        point->name = NULL;
    }

    point->is_uptodate = FALSE;

    if (model != NULL) {
        /* Set the new named pair */
        point->model = model;
        point->name = g_strdup(name);
    }
}

/**
 * adg_point_get_pair:
 * @point: an #AdgPoint
 *
 * #AdgPoint is an evolution of the pair concept, but internally the
 * relevant data is still stored in an #AdgPair struct. This function
 * gets this struct, updating the internal value from the linked
 * named pair if needed.
 *
 * Returns: the pair of @point
 **/
const AdgPair *
adg_point_get_pair(AdgPoint *point)
{
    g_return_val_if_fail(point != NULL, NULL);

    if (!point->is_uptodate) {
        const AdgPair *pair;

        if (point->model == NULL) {
            /* A point with explicit coordinates not up to date
             * is an unexpected condition */
            g_warning(_("%s: trying to get a pair from an undefined point"),
                      G_STRLOC);
            return NULL;
        }

        pair = adg_model_get_named_pair(point->model, point->name);

        if (pair == NULL) {
            g_warning(_("%s: `%s' named pair not found in `%s' model instance"),
                      G_STRLOC, point->name,
                      g_type_name(G_TYPE_FROM_INSTANCE(point->model)));
            return NULL;
        }

        cpml_pair_copy(&point->pair, pair);
    }

    return (AdgPair *) point;
}

/**
 * adg_point_get_model:
 * @point: an #AdgPoint
 *
 * Gets the source model of the named pair bound to @point, or
 * returns %NULL if @point is an explicit pair. The returned
 * value is owned by @point.
 *
 * Returns: an #AdgModel or %NULL
 **/
AdgModel *
adg_point_get_model(AdgPoint *point)
{
    g_return_val_if_fail(point != NULL, NULL);
    return point->model;
}

/**
 * adg_point_get_name:
 * @point: an #AdgPoint
 *
 * Gets the name of the named pair bound to @point, or returns
 * %NULL if @point is an explicit pair. The returned value is
 * owned by @point and should not be modified or freed.
 *
 * Returns: the name of the named pair or %NULL
 **/
const gchar *
adg_point_get_name(AdgPoint *point)
{
    g_return_val_if_fail(point != NULL, NULL);
    return point->name;
}

/**
 * adg_point_invalidate:
 * @point: an #AdgPoint
 *
 * Invalidates @point, forcing a refresh of its internal #AdgPair if
 * the point is linked to a named pair. If @point is explicitely set,
 * this function has no effect.
 **/
void
adg_point_invalidate(AdgPoint *point)
{
    g_return_if_fail(point != NULL);

    if (point->model != NULL)
        point->is_uptodate = FALSE;
}

/**
 * adg_point_equal:
 * @point1: the first point to compare
 * @point2: the second point to compare
 *
 * Compares @point1 and @point2 and returns %TRUE if the points are
 * equals. The comparison is made by matching also where the points
 * are bound. If you want to compare only the coordinates, use
 * adg_pair_equal() directly on their pairs:
 *
 * |[
 * adg_pair_equal(adg_point_get_pair(point1), adg_point_get_pair(point2));
 * ]|
 *
 * Returns: %TRUE if @point1 is equal to @point2, %FALSE otherwise
 **/
gboolean
adg_point_equal(const AdgPoint *point1, const AdgPoint *point2)
{
    g_return_val_if_fail(point1 != NULL, FALSE);
    g_return_val_if_fail(point2 != NULL, FALSE);

    /* Check if the points are not bound to the same model
     * or if only one of the two points is explicit */
    if (point1->model != point2->model)
        return FALSE;

    /* Handle points bound to named pairs */
    if (point1->model)
        return g_strcmp0(point1->name, point2->name) == 0;

    /* Handle points with explicit coordinates */
    return adg_pair_equal(&point1->pair, &point2->pair);
}
