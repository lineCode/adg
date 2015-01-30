/* ADG - Automatic Drawing Generation
 * Copyright (C) 2007-2015  Nicola Fontana <ntd at entidi.it>
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


#include <adg-test.h>
#include <cpml.h>


static cairo_path_data_t data[] = {
    /* Useless heading CPML_MOVE */
    { .header = { CPML_MOVE, 2 }},
    { .point = { 0, 0 }},

    /* First segment: a couple of lines of length 2 */
    { .header = { CPML_MOVE, 2 }},
    { .point = { 0, 0 }},
    { .header = { CPML_LINE, 2 }},
    { .point = { 2, 0 }},
    { .header = { CPML_LINE, 2 }},
    { .point = { 2, 2 }},

    /* Another useless CPML_MOVE with useless embedded data */
    { .header = { CPML_MOVE, 3 }},
    { .point = { 0, 0 }},
    { .point = { 0, 0 }},

    /* Second segment: a Bézier curve with a trailing CPML_CLOSE */
    { .header = { CPML_MOVE, 2 }},
    { .point = { 10, 13 }},
    { .header = { CPML_CURVE, 4 }},
    { .point = { 8, 9 }},
    { .point = { 10, 11 }},
    { .point = { 12, 13 }},
    { .header = { CPML_CLOSE, 1 }},

    /* A valid cairo segment considered invalid by CPML
     * because does not have a leading CPML_MOVE */
    { .header = { CPML_LINE, 2 }},
    { .point = { 10, 0 }},
    { .header = { CPML_CLOSE, 1 }},

    /* Another valid cairo segment invalid in CPML */
    { .header = { CPML_CLOSE, 1 }},

    /* Third segment: a couple of arcs */
    { .header = { CPML_MOVE, 2 }},
    { .point = { 14, 15 }},
    { .header = { CPML_ARC, 3 }},
    { .point = { 16, 17 }},
    { .point = { 18, 19 }},
    { .header = { CPML_ARC, 3 }},
    { .point = { 20, 21 }},
    { .point = { 22, 23 }},

    /* Forth segment: a floating CPML_CLOSE */
    { .header = { CPML_MOVE, 2 }},
    { .point = { 24, 25 }},
    { .header = { CPML_CLOSE, 1 }}
};

static cairo_path_data_t noop_data[] = {
    /* Useless heading CPML_MOVE */
    { .header = { CPML_MOVE, 2 }},
    { .point = { 0, 1 }},
    { .header = { CPML_MOVE, 4 }},
    { .point = { 2, 3 }},
    { .point = { 4, 5 }},
    { .point = { 6, 7 }},
    { .header = { CPML_MOVE, 2 }},
    { .point = { 8, 9 }}
};

static cairo_path_data_t y1_data[] = {
    { .header = { CPML_MOVE, 2 }},
    { .point = { 0, 1 }},
    { .header = { CPML_LINE, 2 }},
    { .point = { 5, 1 }}
};

cairo_path_t path = {
    CAIRO_STATUS_SUCCESS,
    data,
    G_N_ELEMENTS(data)
};

cairo_path_t noop_path = {
    CAIRO_STATUS_SUCCESS,
    noop_data,
    G_N_ELEMENTS(noop_data)
};

cairo_path_t y1_path = {
    CAIRO_STATUS_SUCCESS,
    y1_data,
    G_N_ELEMENTS(y1_data)
};


static void
_cpml_test_browsing(void)
{
    CpmlSegment segment;
    cpml_segment_from_cairo(&segment, &path);

    /* First segment */
    g_assert_cmpint(segment.data[0].header.type, ==, CPML_MOVE);
    g_assert_cmpint(segment.data[2].header.type, ==, CPML_LINE);

    cpml_segment_reset(&segment);
    g_assert_cmpint(segment.data[0].header.type, ==, CPML_MOVE);
    g_assert_cmpint(segment.data[2].header.type, ==, CPML_LINE);

    /* Second segment */
    g_assert_cmpint(cpml_segment_next(&segment), ==, 1);
    g_assert_cmpint(segment.data[0].header.type, ==, CPML_MOVE);
    g_assert_cmpint(segment.data[2].header.type, ==, CPML_CURVE);

    /* Third segment */
    g_assert_cmpint(cpml_segment_next(&segment), ==, 1);
    g_assert_cmpint(segment.data[0].header.type, ==, CPML_MOVE);
    g_assert_cmpint(segment.data[2].header.type, ==, CPML_ARC);

    /* Forth segment */
    g_assert_cmpint(cpml_segment_next(&segment), ==, 1);
    g_assert_cmpint(segment.data[0].header.type, ==, CPML_MOVE);
    g_assert_cmpint(segment.data[2].header.type, ==, CPML_CLOSE);

    g_assert_cmpint(cpml_segment_next(&segment), ==, 0);

    cpml_segment_reset(&segment);
    g_assert_cmpint(segment.data[0].header.type, ==, CPML_MOVE);
    g_assert_cmpint(segment.data[2].header.type, ==, CPML_LINE);
}

static void
_cpml_test_from_cairo(void)
{
    CpmlSegment segment;
    g_assert_cmpint(cpml_segment_from_cairo(NULL, &path), ==, 0);
    g_assert_cmpint(cpml_segment_from_cairo(&segment, NULL), ==, 0);
    g_assert_cmpint(cpml_segment_from_cairo(NULL, NULL), ==, 0);

    g_assert_cmpint(cpml_segment_from_cairo(&segment, &noop_path), ==, 0);
    g_assert_cmpint(cpml_segment_from_cairo(&segment, &path), ==, 1);
}

static void
_cpml_test_get_length(void)
{
    CpmlSegment segment;
    cpml_segment_from_cairo(&segment, &path);

    g_assert_cmpfloat(cpml_segment_get_length(NULL), ==, 0);

    /* First segment */
    g_assert_cmpfloat(cpml_segment_get_length(&segment), ==, 4);
    g_assert_cmpfloat(cpml_segment_get_length(&segment), ==, 4);

    /* Second segment */
    cpml_segment_next(&segment);
    /* TODO: Bézier curve length not yet implemented
     * g_assert_cmpfloat(cpml_segment_get_length(&segment), ==, ???); */

    /* Third segment */
    cpml_segment_next(&segment);
    g_assert_cmpfloat(cpml_segment_get_length(&segment), ==, 0);

    /* Forth segment */
    cpml_segment_next(&segment);
    g_assert_cmpfloat(cpml_segment_get_length(&segment), ==, 0);
}

static void
_cpml_test_put_intersections(void)
{
    CpmlSegment segment1, segment2;
    CpmlPair pair[2];

    cpml_segment_from_cairo(&segment1, &path);
    cpml_segment_from_cairo(&segment2, &y1_path);

    g_assert_cmpuint(cpml_segment_put_intersections(NULL, &segment2, 2, pair), ==, 0);
    g_assert_cmpuint(cpml_segment_put_intersections(&segment1, NULL, 2, pair), ==, 0);
    g_assert_cmpuint(cpml_segment_put_intersections(&segment1, &segment2, 0, pair), ==, 0);
    g_assert_cmpuint(cpml_segment_put_intersections(&segment1, &segment2, 2, NULL), ==, 0);

    /* The first segment of path intersects y1_path in (2, 1) */
    g_assert_cmpuint(cpml_segment_put_intersections(&segment1, &segment2, 2, pair), ==, 1);
    g_assert_cmpfloat(pair[0].x, ==, 2);
    g_assert_cmpfloat(pair[0].y, ==, 1);

    /* The third segment of path does not intersect y1_path */
    cpml_segment_next(&segment1);
    cpml_segment_next(&segment1);
    g_assert_cmpuint(cpml_segment_put_intersections(&segment1, &segment2, 2, pair), ==, 0);
}

static void
_cpml_test_to_cairo(void)
{
    cairo_t *cr;
    CpmlSegment segment;
    int length, last_length;

    cr = adg_test_cairo_context();
    cpml_segment_from_cairo(&segment, &path);

    g_assert_cmpint(adg_test_cairo_num_data(cr), ==, 0);
    cpml_segment_to_cairo(NULL, cr);
    g_assert_cmpint(adg_test_cairo_num_data(cr), ==, 0);
    cpml_segment_to_cairo(&segment, NULL);
    g_assert_cmpint(adg_test_cairo_num_data(cr), ==, 0);

    length = 0;
    do {
        last_length = length;
        cpml_segment_to_cairo(&segment, cr);
        length = adg_test_cairo_num_data(cr);
        g_assert_cmpint(length, >, last_length);
    } while (cpml_segment_next(&segment));

    cairo_destroy(cr);
}

static void
_cpml_test_dump(void)
{
#if GLIB_CHECK_VERSION(2, 38, 0)
    if (g_test_subprocess()) {
        CpmlSegment segment;

        /* This should not crash the process */
        cpml_segment_dump(NULL);

        cpml_segment_from_cairo(&segment, &path);
        cpml_segment_dump(&segment);

        cpml_segment_next(&segment);
        cpml_segment_dump(&segment);

        return;
    }

    g_test_trap_subprocess(NULL, 0, 0);
    g_test_trap_assert_passed();
    g_test_trap_assert_stderr_unmatched("?");
    g_test_trap_assert_stdout("*NULL*");
    g_test_trap_assert_stdout("*move*");
    g_test_trap_assert_stdout("*line*");
    g_test_trap_assert_stdout("*curve*");
    g_test_trap_assert_stdout_unmatched("*arc*");
#endif
}


int
main(int argc, char *argv[])
{
    adg_test_init(&argc, &argv);

    adg_test_add_func("/cpml/segment/behavior/browsing", _cpml_test_browsing);

    adg_test_add_func("/cpml/segment/method/from-cairo", _cpml_test_from_cairo);
    adg_test_add_func("/cpml/segment/method/get-length", _cpml_test_get_length);
    adg_test_add_func("/cpml/segment/method/put-intersections", _cpml_test_put_intersections);
    adg_test_add_func("/cpml/segment/method/to-cairo", _cpml_test_to_cairo);
    adg_test_add_func("/cpml/segment/method/dump", _cpml_test_dump);

    return g_test_run();
}
