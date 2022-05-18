/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_h3.h"
#include <h3/h3api.h>

/* If you declare any globals in php_h3.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(h3)
*/

/* True global resources - no need for thread safety here */

// extension global var

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("h3.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_h3_globals, h3_globals)
    STD_PHP_INI_ENTRY("h3.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_h3_globals, h3_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */

/* {{{ proto H3Index geoToH3(double $lat, double $lon, long $resolution)
 */
PHP_FUNCTION(geoToH3)
{
    zend_long resolution;
    double lat, lon;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ddl", &lat, &lon, &resolution) == FAILURE)
    {
        return;
    }

    GeoCoord location;
    location.lat = degsToRads(lat);
    location.lon = degsToRads(lon);

    H3Index indexed = geoToH3(&location, resolution);

    RETURN_LONG(indexed);
}
/* }}} */

/* {{{ proto array h3ToGeo(long $indexed)
 */
PHP_FUNCTION(h3ToGeo)
{
    zend_long indexed;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &indexed) == FAILURE)
    {
        return;
    }

    // Get the center coordinates.
    GeoCoord center;
    h3ToGeo(indexed, &center);

    double lat_zval, lon_zval;
    lat_zval = radsToDegs(center.lat);
    lon_zval = radsToDegs(center.lon);

    array_init(return_value);
    add_assoc_double(return_value, "lat", lat_zval);
    add_assoc_double(return_value, "lon", lon_zval);
}
/* }}} */

/* {{{ proto array h3ToGeo(long $indexed)
 */
PHP_FUNCTION(h3ToGeoBoundary)
{
    zend_long indexed;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &indexed) == FAILURE)
    {
        return;
    }

    // Get the vertices of the H3 index.
    GeoBoundary boundary;
    h3ToGeoBoundary(indexed, &boundary);

    array_init(return_value);
    // Indexes can have different number of vertices under some cases,
    // which is why boundary.numVerts is needed.
    for (int i = 0; i < boundary.numVerts; i++)
    {
        double lat, lon;

        lat = radsToDegs(boundary.verts[i].lat);
        lon = radsToDegs(boundary.verts[i].lon);

        zval lat_lon_arr;
        array_init(&lat_lon_arr);
        add_assoc_double(&lat_lon_arr, "lat", lat);
        add_assoc_double(&lat_lon_arr, "lon", lon);

        add_index_zval(return_value, i, &lat_lon_arr);
    }
}
/* }}} */

/* {{{ proto long h3GetResolution(long $indexed)
 */
PHP_FUNCTION(h3GetResolution)
{
    zend_long indexed;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &indexed) == FAILURE)
    {
        return;
    }

    int resolution = h3GetResolution(indexed);

    RETURN_LONG(resolution);
}
/* }}} */

/* {{{ proto int h3GetBaseCell(long $indexed)
 */
PHP_FUNCTION(h3GetBaseCell)
{
    zend_long indexed;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &indexed) == FAILURE)
    {
        return;
    }

    int base_cell = h3GetBaseCell(indexed);

    RETURN_LONG(base_cell);
}
/* }}} */

/* {{{ proto H3Index stringToH3(string $str)
 */
PHP_FUNCTION(stringToH3)
{
    char *str;
    size_t str_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &str, &str_len) == FAILURE)
    {
        return;
    }

    H3Index indexed = stringToH3(str);

    RETURN_LONG(indexed);
}
/* }}} */

/* {{{ proto string h3ToString(long $indexed)
 */
PHP_FUNCTION(h3ToString)
{
    zend_long indexed;
    char str[17] = "";

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &indexed) == FAILURE)
    {
        return;
    }

    h3ToString(indexed, str, 17);

    RETURN_STRINGL(str, 17);
}
/* }}} */

/* {{{ proto bool h3IsValid(long $indexed)
 */
PHP_FUNCTION(h3IsValid)
{
    zend_long indexed;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &indexed) == FAILURE)
    {
        return;
    }

    if (h3IsValid(indexed) == 0)
    {
        RETURN_FALSE;
    }
    else
    {
        RETURN_TRUE;
    }
}
/* }}} */

/* {{{ proto bool h3IsResClassIII(long $indexed)
 */
PHP_FUNCTION(h3IsResClassIII)
{
    zend_long indexed;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &indexed) == FAILURE)
    {
        return;
    }

    if (h3IsResClassIII(indexed) == 0)
    {
        RETURN_FALSE;
    }
    else
    {
        RETURN_TRUE;
    }
}
/* }}} */

/* {{{ proto bool h3IsPentagon(long $indexed)
 */
PHP_FUNCTION(h3IsPentagon)
{
    zend_long indexed;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &indexed) == FAILURE)
    {
        return;
    }

    if (h3IsPentagon(indexed) == 0)
    {
        RETURN_FALSE;
    }
    else
    {
        RETURN_TRUE;
    }
}
/* }}} */

/* {{{ proto array h3IsPentagon(long $indexed)
 */
PHP_FUNCTION(h3GetFaces)
{
    zend_long indexed;
    int arr_count;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &indexed) == FAILURE)
    {
        return;
    }

    arr_count = maxFaceCount(indexed);
    int *outs = (int *)calloc(arr_count, sizeof(int));

    h3GetFaces(indexed, outs);

    array_init(return_value);

    for (int i = 0; i < arr_count; i++)
    {
        add_index_long(return_value, i, outs[i]);
    }
    free(outs);
}
/* }}} */

/* {{{ proto long maxFaceCount(long $indexed)
 */
PHP_FUNCTION(maxFaceCount)
{
    zend_long indexed;
    int count;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &indexed) == FAILURE)
    {
        return;
    }

    count = maxFaceCount(indexed);

    RETURN_LONG(count);
}
/* }}} */

/* {{{ proto array kRing(long $indexed, long $k)
 */
PHP_FUNCTION(kRing)
{
    zend_long indexed, k;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &indexed, &k) == FAILURE)
    {
        return;
    }

    int arr_count = maxKringSize(k);
    H3Index *outs = (H3Index *)calloc(arr_count, sizeof(H3Index));

    kRing(indexed, k, outs);

    array_init(return_value);

    for (int i = 0; i < arr_count; i++)
    {
        add_index_long(return_value, i, outs[i]);
    }

    free(outs);
}
/* }}} */

/* {{{ proto long maxKringSize(long $k)
 */
PHP_FUNCTION(maxKringSize)
{
    zend_long k;
    zend_long size;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &k) == FAILURE)
    {
        return;
    }

    size = maxKringSize(k);

    RETURN_LONG(size);
}
/* }}} */

/* {{{ proto array kRingDistances(long $k)
 */
PHP_FUNCTION(kRingDistances)
{
    zend_long indexed, k;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &indexed, &k) == FAILURE)
    {
        return;
    }

    int arr_count = maxKringSize(k);

    H3Index *outs = (H3Index *)calloc(arr_count, sizeof(H3Index));
    int *distances = (int *)calloc(arr_count, sizeof(int));
    kRingDistances(indexed, k, outs, distances);

    zval out_zvals, distance_zvals;
    array_init(&out_zvals);
    array_init(&distance_zvals);

    for (int i = 0; i < arr_count; i++)
    {
        add_index_long(&out_zvals, i, outs[i]);
        add_index_long(&distance_zvals, i, distances[i]);
    }

    array_init(return_value);
    add_index_zval(return_value, 0, &out_zvals);
    add_index_zval(return_value, 1, &distance_zvals);

    free(outs);
    free(distances);
}
/* }}} */

/* {{{ proto array hexRange(long $k)
 */
PHP_FUNCTION(hexRange)
{
    zend_long indexed, k;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &indexed, &k) == FAILURE)
    {
        return;
    }

    int arr_count = maxKringSize(k);

    H3Index *outs = (H3Index *)calloc(arr_count, sizeof(H3Index));
    if (hexRange(indexed, k, outs) != 0)
    {
        RETURN_FALSE;
    }

    array_init(return_value);

    for (int i = 0; i < arr_count; i++)
    {
        add_index_long(return_value, i, outs[i]);
    }

    free(outs);
}
/* }}} */

/* {{{ proto array hexRangeDistances(long $k)
 */
PHP_FUNCTION(hexRangeDistances)
{
    zend_long indexed, k;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &indexed, &k) == FAILURE)
    {
        return;
    }

    int arr_count = maxKringSize(k);

    H3Index *outs = (H3Index *)calloc(arr_count, sizeof(H3Index));
    int *distances = (int *)calloc(arr_count, sizeof(int));
    if (hexRangeDistances(indexed, k, outs, distances) != 0)
    {
        RETURN_FALSE;
    }

    zval out_zvals, distance_zvals;
    array_init(&out_zvals);
    array_init(&distance_zvals);

    for (int i = 0; i < arr_count; i++)
    {
        add_index_long(&out_zvals, i, outs[i]);
        add_index_long(&distance_zvals, i, distances[i]);
    }

    array_init(return_value);
    add_index_zval(return_value, 0, &out_zvals);
    add_index_zval(return_value, 1, &distance_zvals);

    free(outs);
    free(distances);
}
/* }}} */

/* {{{ proto array hexRanges(array $ranges, long $k)
 */
PHP_FUNCTION(hexRanges)
{
    zend_long k;
    zval *h3Set_zval;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "al", &h3Set_zval, &k) == FAILURE)
    {
        return;
    }

    int length = zend_hash_num_elements(Z_ARRVAL_P(h3Set_zval));
    H3Index *indexed = (H3Index *)calloc(length, sizeof(H3Index));

    zval *h3Indexed_zval;
    zend_ulong i;

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(h3Set_zval), i, h3Indexed_zval)
    {
        indexed[i] = Z_LVAL_P(h3Indexed_zval);
    }
    ZEND_HASH_FOREACH_END();

    int arr_count = maxKringSize(k) * length;

    H3Index *outs = (H3Index *)calloc(arr_count, sizeof(H3Index));
    if (hexRanges(indexed, length, k, outs) != 0)
    {
        RETURN_FALSE;
    }

    array_init(return_value);

    for (int i = 0; i < arr_count; i++)
    {
        add_index_long(return_value, i, outs[i]);
    }

    free(indexed);
    free(outs);
}
/* }}} */

/* {{{ proto array hexRing(long $indexed, long $k)
 */
PHP_FUNCTION(hexRing)
{
    zend_long indexed, k;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &indexed, &k) == FAILURE)
    {
        return;
    }

    int arr_count = 6 * k;

    H3Index *outs = (H3Index *)calloc(arr_count, sizeof(H3Index));
    if (hexRing(indexed, k, outs) != 0)
    {
        RETURN_FALSE;
    }

    array_init(return_value);

    for (int i = 0; i < arr_count; i++)
    {
        add_index_long(return_value, i, outs[i]);
    }

    free(outs);
}
/* }}} */

/* {{{ proto array h3Line(long $start, long $end)
 */
PHP_FUNCTION(h3Line)
{
    zend_long start, end;
    int size;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &start, &end) == FAILURE)
    {
        return;
    }

    size = h3LineSize(start, end);

    H3Index *outs = (H3Index *)calloc(size, sizeof(H3Index));

    if (h3Line(start, end, outs) != 0)
    {
        RETURN_FALSE;
    }

    array_init(return_value);

    for (int i = 0; i < size; i++)
    {
        add_index_long(return_value, i, outs[i]);
    }

    free(outs);
}
/* }}} */

/* {{{ proto long h3LineSize(long $start, long $end)
 */
PHP_FUNCTION(h3LineSize)
{
    zend_long start, end;
    int size;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &start, &end) == FAILURE)
    {
        return;
    }

    size = h3LineSize(start, end);

    if (size < 0)
    {
        RETURN_FALSE;
    }
    else
    {
        RETURN_LONG(size);
    }
}
/* }}} */

/* {{{ proto long h3Distance(long $origin, long $h3)
 */
PHP_FUNCTION(h3Distance)
{
    zend_long origin, h3;
    int distance;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &origin, &h3) == FAILURE)
    {
        return;
    }

    distance = h3Distance(origin, h3);

    RETURN_LONG(distance);
}
/* }}} */

/* {{{ proto array experimentalH3ToLocalIj(long $origin, long $h3)
 */
//This function is experimental, and its output is not guaranteed to be compatible across different versions of H3.
PHP_FUNCTION(experimentalH3ToLocalIj)
{
    zend_long origin, h3;
    CoordIJ ij;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &origin, &h3) == FAILURE)
    {
        return;
    }

    if (experimentalH3ToLocalIj(origin, h3, &ij) != 0)
    {
        RETURN_FALSE;
    }
    else
    {
        array_init(return_value);
        add_assoc_long(return_value, "i", ij.i);
        add_assoc_long(return_value, "j", ij.j);
    }
}
/* }}} */

/* {{{ proto long experimentalLocalIjToH3(long $origin, array $ij_zval)
 */
//This function is experimental, and its output is not guaranteed to be compatible across different versions of H3.
PHP_FUNCTION(experimentalLocalIjToH3)
{
    zend_long origin;
    CoordIJ ij;
    zval *ij_zval;
    H3Index out;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "la", &origin, &ij_zval) == FAILURE)
    {
        return;
    }

    ij.i = Z_LVAL_P(zend_hash_str_find(Z_ARRVAL_P(ij_zval), "i", 1));
    ij.j = Z_LVAL_P(zend_hash_str_find(Z_ARRVAL_P(ij_zval), "j", 1));

    if (experimentalLocalIjToH3(origin, &ij, &out) != 0)
    {
        RETURN_FALSE;
    }
    else
    {
        RETURN_LONG(out);
    }
}
/* }}} */

/* {{{ proto long h3ToParent(long $indexed, int $parentRes)
 */
PHP_FUNCTION(h3ToParent)
{
    zend_long indexed;
    int parentRes;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &indexed, &parentRes) == FAILURE)
    {
        return;
    }

    H3Index h3Parent = h3ToParent(indexed, parentRes);

    RETURN_LONG(h3Parent);
}
/* }}} */

/* {{{ proto array h3ToChildren(long $indexed, int $childrenRes)
 */
PHP_FUNCTION(h3ToChildren)
{
    zend_long indexed, childrenRes;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &indexed, &childrenRes) == FAILURE)
    {
        return;
    }

    int childrenSize = maxH3ToChildrenSize(indexed, childrenRes);
    H3Index *h3Childrens = (H3Index *)calloc(childrenSize, sizeof(H3Index));
    h3ToChildren(indexed, childrenRes, h3Childrens);

    array_init(return_value);

    for (int i = 0; i < childrenSize; i++)
    {
        add_index_long(return_value, i, h3Childrens[i]);
    }

    free(h3Childrens);
}
/* }}} */

/* {{{ proto long maxH3ToChildrenSize(long $indexed, int $childrenRes)
 */
PHP_FUNCTION(maxH3ToChildrenSize)
{
    zend_long indexed, childrenRes;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &indexed, &childrenRes) == FAILURE)
    {
        return;
    }

    int childrenSize = maxH3ToChildrenSize(indexed, childrenRes);

    RETURN_LONG(childrenSize);
}
/* }}} */

/* {{{ proto long h3ToCenterChild(long $indexed, int $childrenRes)
 */
PHP_FUNCTION(h3ToCenterChild)
{
    zend_long indexed, childrenRes;
    H3Index centerChild;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &indexed, &childrenRes) == FAILURE)
    {
        return;
    }

    centerChild = h3ToCenterChild(indexed, childrenRes);

    RETURN_LONG(centerChild);
}
/* }}} */

/* {{{ proto array h3Compact(array $indexes)
 */
PHP_FUNCTION(h3Compact)
{
    zval *compactedSet_zval;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &compactedSet_zval) == FAILURE)
    {
        return;
    }

    int length = zend_hash_num_elements(Z_ARRVAL_P(compactedSet_zval));
    H3Index *indexed = (H3Index *)calloc(length, sizeof(H3Index));

    zval *h3Indexed_zval;
    zend_ulong i;

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(compactedSet_zval), i, h3Indexed_zval)
    {
        indexed[i] = Z_LVAL_P(h3Indexed_zval);
    }
    ZEND_HASH_FOREACH_END();

    H3Index *outs = (H3Index *)calloc(length, sizeof(H3Index));
    if (compact(indexed, outs, length) != 0)
    {
        RETURN_FALSE;
    }

    array_init(return_value);

    for (int i = 0; i < length; i++)
    {
        if (outs[i] == 0)
            break;
        add_index_long(return_value, i, outs[i]);
    }

    free(outs);
    free(indexed);
}
/* }}} */

/* {{{ proto array uncompact(array $compacted, long $resolution)
 */
PHP_FUNCTION(uncompact)
{
    zend_long uncompactRes;
    zval *compactedSet_zval;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "al", &compactedSet_zval, &uncompactRes) == FAILURE)
    {
        return;
    }

    int length = zend_hash_num_elements(Z_ARRVAL_P(compactedSet_zval));
    H3Index *indexed = (H3Index *)calloc(length, sizeof(H3Index));

    zval *h3Indexed_zval;
    zend_ulong i;

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(compactedSet_zval), i, h3Indexed_zval)
    {
        indexed[i] = Z_LVAL_P(h3Indexed_zval);
    }
    ZEND_HASH_FOREACH_END();

    int uncompactedSize = maxUncompactSize(indexed, length, uncompactRes);
    H3Index *outs = (H3Index *)calloc(uncompactedSize, sizeof(H3Index));
    if (uncompact(indexed, length, outs, uncompactedSize, uncompactRes) != 0)
    {
        RETURN_FALSE;
    }

    array_init(return_value);

    for (int i = 0; i < uncompactedSize; i++)
    {
        add_index_long(return_value, i, outs[i]);
    }

    free(indexed);
    free(outs);
}
/* }}} */

/* {{{ proto long maxUncompactSize(array $compacted, long $resolution)
 */
PHP_FUNCTION(maxUncompactSize)
{
    zend_long uncompactRes;
    zval *compactedSet_zval;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "al", &compactedSet_zval, &uncompactRes) == FAILURE)
    {
        return;
    }

    int length = zend_hash_num_elements(Z_ARRVAL_P(compactedSet_zval));
    H3Index *indexed = (H3Index *)calloc(length, sizeof(H3Index));

    zval *h3Indexed_zval;
    zend_ulong i;

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(compactedSet_zval), i, h3Indexed_zval)
    {
        indexed[i] = Z_LVAL_P(h3Indexed_zval);
    }
    ZEND_HASH_FOREACH_END();

    int uncompactedSize = maxUncompactSize(indexed, length, uncompactRes);

    free(indexed);

    RETURN_LONG(uncompactedSize);
}
/* }}} */

/* {{{ proto bool h3IndexesAreNeighbors(long $origin, long $destination)
 */
PHP_FUNCTION(h3IndexesAreNeighbors)
{
    zend_long origin, destination;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &origin, &destination) == FAILURE)
    {
        return;
    }

    if (h3IndexesAreNeighbors(origin, destination) == 1)
    {
        RETURN_TRUE;
    }
    else
    {
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto long getH3UnidirectionalEdge(long $origin, long $destination)
 */
PHP_FUNCTION(getH3UnidirectionalEdge)
{
    zend_long origin, destination;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "ll", &origin, &destination) == FAILURE)
    {
        return;
    }

    H3Index index = getH3UnidirectionalEdge(origin, destination);
    if (index == 0)
    {
        RETURN_FALSE;
    }

    RETURN_LONG(index);
}
/* }}} */

/* {{{ proto bool h3UnidirectionalEdgeIsValid(long $edge)
 */
PHP_FUNCTION(h3UnidirectionalEdgeIsValid)
{
    zend_long edge;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &edge) == FAILURE)
    {
        return;
    }

    if (h3UnidirectionalEdgeIsValid(edge) == 1)
    {
        RETURN_TRUE;
    }
    else
    {
        RETURN_FALSE;
    }
}
/* }}} */

/* {{{ proto long getOriginH3IndexFromUnidirectionalEdge(long $edge)
 */
PHP_FUNCTION(getOriginH3IndexFromUnidirectionalEdge)
{
    zend_long edge;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &edge) == FAILURE)
    {
        return;
    }

    H3Index index = getOriginH3IndexFromUnidirectionalEdge(edge);

    RETURN_LONG(index);
}
/* }}} */

/* {{{ proto long getDestinationH3IndexFromUnidirectionalEdge(long $edge)
 */
PHP_FUNCTION(getDestinationH3IndexFromUnidirectionalEdge)
{
    zend_long edge;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &edge) == FAILURE)
    {
        return;
    }

    H3Index index = getDestinationH3IndexFromUnidirectionalEdge(edge);

    RETURN_LONG(index);
}
/* }}} */

/* {{{ proto array getH3IndexesFromUnidirectionalEdge(long $edge)
 */
PHP_FUNCTION(getH3IndexesFromUnidirectionalEdge)
{
    zend_long edge;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &edge) == FAILURE)
    {
        return;
    }

    H3Index originDestination[2];
    getH3IndexesFromUnidirectionalEdge(edge, originDestination);

    array_init(return_value);

    for (int i = 0; i < 2; i++)
    {
        add_index_long(return_value, i, originDestination[i]);
    }
}
/* }}} */

/* {{{ proto array getH3UnidirectionalEdgesFromHexagon(long $edge)
 */
PHP_FUNCTION(getH3UnidirectionalEdgesFromHexagon)
{
    zend_long edge;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &edge) == FAILURE)
    {
        return;
    }

    H3Index edges[6];
    getH3UnidirectionalEdgesFromHexagon(edge, edges);

    array_init(return_value);

    for (int i = 0; i < 6; i++)
    {
        add_index_long(return_value, i, edges[i]);
    }
}
/* }}} */

/* {{{ proto array getH3UnidirectionalEdgeBoundary(long $edge)
 */
PHP_FUNCTION(getH3UnidirectionalEdgeBoundary)
{
    zend_long edge;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &edge) == FAILURE)
    {
        return;
    }

    // Get the vertices of the H3 index.
    GeoBoundary boundary;
    getH3UnidirectionalEdgeBoundary(edge, &boundary);

    array_init(return_value);
    // Indexes can have different number of vertices under some cases,
    // which is why boundary.numVerts is needed.
    for (int v = 0; v < boundary.numVerts; v++)
    {
        double lat, lon;

        lat = radsToDegs(boundary.verts[v].lat);
        lon = radsToDegs(boundary.verts[v].lon);

        zval lat_lon_arr;
        array_init(&lat_lon_arr);

        add_assoc_double(&lat_lon_arr, "lat", lat);
        add_assoc_double(&lat_lon_arr, "lon", lon);

        add_index_zval(return_value, v, &lat_lon_arr);
    }
}
/* }}} */

/* {{{ proto array polyfill(array $geopolygons, long $resolution)
 */
PHP_FUNCTION(polyfill)
{
    zval *geopolygon_zval;
    zend_long res;
    int polyfillsize;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "al", &geopolygon_zval, &res) == FAILURE)
    {
        return;
    }

    zval *geofence_zval = zend_hash_str_find(Z_ARRVAL_P(geopolygon_zval), "geofence", 8);
    zval *holes_zval = zend_hash_str_find(Z_ARRVAL_P(geopolygon_zval), "holes", 5);

    int geofence_verts_num = zend_hash_num_elements(Z_ARRVAL_P(geofence_zval));
    GeoCoord *geofence_verts = (GeoCoord *)calloc(geofence_verts_num, sizeof(GeoCoord));

    zval *vert_zval;
    zend_ulong i, j;

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(geofence_zval), i, vert_zval)
    {
        GeoCoord vert;
        vert.lat = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(vert_zval), "lat", 3)));
        vert.lon = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(vert_zval), "lon", 3)));
        geofence_verts[i] = vert;
    }
    ZEND_HASH_FOREACH_END();

    Geofence geoface_geoface = {geofence_verts_num, geofence_verts};

    int holesnum = zend_hash_num_elements(Z_ARRVAL_P(holes_zval));
    Geofence *holes_geofaces = (Geofence *)calloc(holesnum, sizeof(Geofence));

    zval *holesnum_geoface_zval;

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(holes_zval), i, holesnum_geoface_zval)
    {
        int holes_vertsnum = zend_hash_num_elements(Z_ARRVAL_P(holesnum_geoface_zval));
        GeoCoord *holes_verts = (GeoCoord *)calloc(holes_vertsnum, sizeof(GeoCoord));
        zval *holes_verts_zval;
        ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(holesnum_geoface_zval), j, holes_verts_zval)
        {
            GeoCoord vert;
            vert.lat = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(holes_verts_zval), "lat", 3)));
            vert.lon = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(holes_verts_zval), "lon", 3)));
            holes_verts[j] = vert;
        }
        ZEND_HASH_FOREACH_END();
        Geofence holes_geoface = {holes_vertsnum, holes_verts};
        holes_geofaces[i] = holes_geoface;
        free(holes_verts);
    }
    ZEND_HASH_FOREACH_END();

    GeoPolygon geopolygon = {geoface_geoface, holesnum, holes_geofaces};

    polyfillsize = maxPolyfillSize(&geopolygon, res);
    H3Index *polyfillOut = (H3Index *)calloc(polyfillsize, sizeof(H3Index));
    polyfill(&geopolygon, res, polyfillOut);

    array_init(return_value);

    for (int i = 0; i < polyfillsize; i++)
    {
        add_index_long(return_value, i, polyfillOut[i]);
    }

    free(geofence_verts);
    free(holes_geofaces);
    free(polyfillOut);
}
/* }}} */

/* {{{ proto array maxPolyfillSize(array $geopolygons, long $resolution)
 */
PHP_FUNCTION(maxPolyfillSize)
{
    zval *geopolygon_zval;
    zend_long res;
    int polyfillsize;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "al", &geopolygon_zval, &res) == FAILURE)
    {
        return;
    }

    zval *geofence_zval = zend_hash_str_find(Z_ARRVAL_P(geopolygon_zval), "geofence", 8);
    zval *holes_zval = zend_hash_str_find(Z_ARRVAL_P(geopolygon_zval), "holes", 5);

    int geofence_verts_num = zend_hash_num_elements(Z_ARRVAL_P(geofence_zval));
    GeoCoord *geofence_verts = (GeoCoord *)calloc(geofence_verts_num, sizeof(GeoCoord));

    zval *vert_zval;
    zend_ulong i, j;

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(geofence_zval), i, vert_zval)
    {
        GeoCoord vert;
        vert.lat = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(vert_zval), "lat", 3)));
        vert.lon = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(vert_zval), "lon", 3)));
        geofence_verts[i] = vert;
    }
    ZEND_HASH_FOREACH_END();

    Geofence geoface_geoface = {geofence_verts_num, geofence_verts};

    int holesnum = zend_hash_num_elements(Z_ARRVAL_P(holes_zval));
    Geofence *holes_geofaces = (Geofence *)calloc(holesnum, sizeof(Geofence));

    zval *holesnum_geoface_zval;

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(holes_zval), i, holesnum_geoface_zval)
    {
        int holes_vertsnum = zend_hash_num_elements(Z_ARRVAL_P(holesnum_geoface_zval));
        GeoCoord *holes_verts = (GeoCoord *)calloc(holes_vertsnum, sizeof(GeoCoord));
        zval *holes_verts_zval;
        ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(holesnum_geoface_zval), j, holes_verts_zval)
        {
            GeoCoord vert;
            vert.lat = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(holes_verts_zval), "lat", 3)));
            vert.lon = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(holes_verts_zval), "lon", 3)));
            holes_verts[j] = vert;
        }
        ZEND_HASH_FOREACH_END();
        Geofence holes_geoface = {holes_vertsnum, holes_verts};
        holes_geofaces[i] = holes_geoface;
        free(holes_verts);
    }
    ZEND_HASH_FOREACH_END();

    GeoPolygon geopolygon = {geoface_geoface, holesnum, holes_geofaces};

    polyfillsize = maxPolyfillSize(&geopolygon, res);

    free(geofence_verts);
    free(holes_geofaces);

    RETURN_LONG(polyfillsize);
}
/* }}} */

/* {{{ proto array h3SetToLinkedGeo(array $h3set)
 */
PHP_FUNCTION(h3SetToLinkedGeo)
{
    zval *h3set_zval;
    LinkedGeoPolygon polygon;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "a", &h3set_zval) == FAILURE)
    {
        return;
    }

    int numHexes = zend_hash_num_elements(Z_ARRVAL_P(h3set_zval));
    H3Index *h3set = (H3Index *)calloc(numHexes, sizeof(H3Index));

    zval *h3Indexed_zval;
    zend_ulong i;

    ZEND_HASH_FOREACH_NUM_KEY_VAL(Z_ARRVAL_P(h3set_zval), i, h3Indexed_zval)
    {
        h3set[i] = Z_LVAL_P(h3Indexed_zval);
    }
    ZEND_HASH_FOREACH_END();

    h3SetToLinkedGeo(h3set, numHexes, &polygon);

    free(h3set);

    LinkedGeoPolygon *polygonloop = &polygon;
    array_init(return_value);

    while (polygonloop != NULL)
    {
        zval geoloop_zval;
        array_init(&geoloop_zval);

        LinkedGeoLoop *geoloop = polygonloop->first;

        while (geoloop != NULL)
        {
            zval geocoord_zval;
            array_init(&geocoord_zval);

            LinkedGeoCoord *coordloop = geoloop->first;

            while (coordloop != NULL)
            {
                zval vertex_zval;
                array_init(&vertex_zval);

                double lat, lon;

                lat = radsToDegs(coordloop->vertex.lat);
                lon = radsToDegs(coordloop->vertex.lon);

                add_assoc_double(&vertex_zval, "lat", lat);
                add_assoc_double(&vertex_zval, "lon", lon);

                add_next_index_zval(&geocoord_zval, &vertex_zval);

                coordloop = coordloop->next;
            }

            add_next_index_zval(&geoloop_zval, &geocoord_zval);

            geoloop = geoloop->next;
        }

        add_next_index_zval(return_value, &geoloop_zval);

        polygonloop = polygonloop->next;
    }

    destroyLinkedPolygon(&polygon);
}
/* }}} */

/* {{{ proto double degsToRads(souble $lat_lon)
 */
PHP_FUNCTION(degsToRads)
{
    double lat_lon;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &lat_lon) == FAILURE)
    {
        return;
    }

    double radians_lat_lon = degsToRads(lat_lon);

    RETURN_DOUBLE(radians_lat_lon);
}
/* }}} */

/* {{{ proto double radsToDegs(souble $lat_lon)
 */
PHP_FUNCTION(radsToDegs)
{
    double lat_lon;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "d", &lat_lon) == FAILURE)
    {
        return;
    }

    double degrees_lat_lon = radsToDegs(lat_lon);

    RETURN_DOUBLE(degrees_lat_lon);
}
/* }}} */

/* {{{ proto double hexAreaKm2(long $res)
 */
PHP_FUNCTION(hexAreaKm2)
{
    zend_long res;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &res) == FAILURE)
    {
        return;
    }

    double kilometers2 = hexAreaKm2(res);

    RETURN_DOUBLE(kilometers2);
}
/* }}} */

/* {{{ proto double hexAreaM2(long $res)
 */
PHP_FUNCTION(hexAreaM2)
{
    zend_long res;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &res) == FAILURE)
    {
        return;
    }

    double meters2 = hexAreaM2(res);

    RETURN_DOUBLE(meters2);
}
/* }}} */

/* {{{ proto double cellAreaKm2(long $h)
 */
PHP_FUNCTION(cellAreaKm2)
{
    zend_long h;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &h) == FAILURE)
    {
        return;
    }

    double kilometers2 = cellAreaKm2(h);

    RETURN_DOUBLE(kilometers2);
}
/* }}} */

/* {{{ proto double cellAreaM2(long $h)
 */
PHP_FUNCTION(cellAreaM2)
{
    zend_long h;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &h) == FAILURE)
    {
        return;
    }

    double meters2 = cellAreaM2(h);

    RETURN_DOUBLE(meters2);
}
/* }}} */

/* {{{ proto double cellAreaRads2(long $h)
 */
PHP_FUNCTION(cellAreaRads2)
{
    zend_long h;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &h) == FAILURE)
    {
        return;
    }

    double rads2 = cellAreaRads2(h);

    RETURN_DOUBLE(rads2);
}
/* }}} */

/* {{{ proto double edgeLengthKm(long $res)
 */
PHP_FUNCTION(edgeLengthKm)
{
    zend_long res;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &res) == FAILURE)
    {
        return;
    }

    double kilometers = edgeLengthKm(res);

    RETURN_DOUBLE(kilometers);
}
/* }}} */

/* {{{ proto double edgeLengthM(long $res)
 */
PHP_FUNCTION(edgeLengthM)
{
    zend_long res;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &res) == FAILURE)
    {
        return;
    }

    double meters = edgeLengthM(res);

    RETURN_DOUBLE(meters);
}
/* }}} */

/* {{{ proto double exactEdgeLengthKm(long $edge)
 */
PHP_FUNCTION(exactEdgeLengthKm)
{
    zend_long edge;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &edge) == FAILURE)
    {
        return;
    }

    double kilometers = exactEdgeLengthKm(edge);

    RETURN_DOUBLE(kilometers);
}
/* }}} */

/* {{{ proto double exactEdgeLengthM(long $edge)
 */
PHP_FUNCTION(exactEdgeLengthM)
{
    zend_long edge;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &edge) == FAILURE)
    {
        return;
    }

    double meters = exactEdgeLengthM(edge);

    RETURN_DOUBLE(meters);
}
/* }}} */

/* {{{ proto double exactEdgeLengthRads(long $edge)
 */
PHP_FUNCTION(exactEdgeLengthRads)
{
    zend_long edge;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &edge) == FAILURE)
    {
        return;
    }

    double rads = exactEdgeLengthM(edge);

    RETURN_DOUBLE(rads);
}
/* }}} */

/* {{{ proto double numHexagons(long $res)
 */
PHP_FUNCTION(numHexagons)
{
    zend_long res;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &res) == FAILURE)
    {
        return;
    }

    zend_long number_of_indexes = numHexagons(res);

    RETURN_LONG(number_of_indexes);
}
/* }}} */

/* {{{ proto array getRes0Indexes()
 */
PHP_FUNCTION(getRes0Indexes)
{
    int indexcount = res0IndexCount();
    H3Index *outs = (H3Index *)calloc(indexcount, sizeof(H3Index));

    getRes0Indexes(outs);

    array_init(return_value);
    for (int i = 0; i < indexcount; i++)
    {
        add_index_long(return_value, i, outs[i]);
    }

    free(outs);
}
/* }}} */

/* {{{ proto array res0IndexCount()
 */
PHP_FUNCTION(res0IndexCount)
{
    RETURN_LONG(res0IndexCount());
}
/* }}} */

/* {{{ proto array getPentagonIndexes(long $res)
 */
PHP_FUNCTION(getPentagonIndexes)
{
    zend_long res;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &res) == FAILURE)
    {
        return;
    }

    //int indexcount=pentagonIndexCount();
    H3Index outs[12];

    getPentagonIndexes(res, outs);

    array_init(return_value);
    for (int i = 0; i < 12; i++)
    {
        add_index_long(return_value, i, outs[i]);
    }
}
/* }}} */

/* {{{ proto long pentagonIndexCount()
 */
PHP_FUNCTION(pentagonIndexCount)
{
    //RETURN_LONG(pentagonIndexCount());
    RETURN_LONG(12);
}
/* }}} */

/* {{{ proto double pointDistKm(array $a, array $b)
 */
PHP_FUNCTION(pointDistKm)
{
    zval *a_zval, *b_zval;
    GeoCoord a, b;
    double kilometers;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "aa", &a_zval, &b_zval) == FAILURE)
    {
        return;
    }

    a.lat = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(a_zval), "lat", 3)));
    a.lon = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(a_zval), "lon", 3)));
    b.lat = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(b_zval), "lat", 3)));
    b.lon = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(b_zval), "lon", 3)));

    kilometers = pointDistKm(&a, &b);

    RETURN_DOUBLE(kilometers);
}
/* }}} */

/* {{{ proto double pointDistM(array $a, array $b)
 */
PHP_FUNCTION(pointDistM)
{
    zval *a_zval, *b_zval;
    GeoCoord a, b;
    double meters;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "aa", &a_zval, &b_zval) == FAILURE)
    {
        return;
    }

    a.lat = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(a_zval), "lat", 3)));
    a.lon = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(a_zval), "lon", 3)));
    b.lat = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(b_zval), "lat", 3)));
    b.lon = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(b_zval), "lon", 3)));

    meters = pointDistM(&a, &b);

    RETURN_DOUBLE(meters);
}
/* }}} */

/* {{{ proto double pointDistRads(array $a, array $b)
 */
PHP_FUNCTION(pointDistRads)
{
    zval *a_zval, *b_zval;
    GeoCoord a, b;
    double rads;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "aa", &a_zval, &b_zval) == FAILURE)
    {
        return;
    }

    a.lat = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(a_zval), "lat", 3)));
    a.lon = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(a_zval), "lon", 3)));
    b.lat = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(b_zval), "lat", 3)));
    b.lon = degsToRads(Z_DVAL_P(zend_hash_str_find(Z_ARRVAL_P(b_zval), "lon", 3)));

    rads = pointDistRads(&a, &b);

    RETURN_DOUBLE(rads);
}
/* }}} */

/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/

/* {{{ php_h3_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_h3_init_globals(zend_h3_globals *h3_globals)
{
    h3_globals->global_value = 0;
    h3_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(h3)
{
    /* If you have INI entries, uncomment these lines
    REGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(h3)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(h3)
{
#if defined(COMPILE_DL_H3) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(h3)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(h3)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "h3 support", "enabled");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
    */
}
/* }}} */

/* {{{ argument information */
ZEND_BEGIN_ARG_INFO_EX(arginfo_geoToH3, 0, 0, 2)
    ZEND_ARG_TYPE_INFO(0, lat, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, lon, IS_DOUBLE, 0)
    ZEND_ARG_TYPE_INFO(0, resolution, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3ToGeo, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3ToGeoBoundary, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(arginfo_h3GetResolution, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3GetBaseCell, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_stringToH3, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3ToString, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3IsValid, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3IsResClassIII, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3IsPentagon, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3GetFaces, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_maxFaceCount, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_kRing, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_maxKringSize, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_kRingDistances, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hexRange, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hexRangeDistances, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hexRanges, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, k, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hexRing, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, k, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3Line, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, start, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, end, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3LineSize, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, start, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, end, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3Distance, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, origin, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, h3, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_experimentalH3ToLocalIj, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, origin, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, h3, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_experimentalLocalIjToH3, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, origin, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, h3, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3ToParent, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, parentRes, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3ToChildren, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, childrenRes, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_maxH3ToChildrenSize, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, childrenRes, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3ToCenterChild, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexed, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, childrenRes, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3Compact, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, indexes, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_uncompact, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, compacted, IS_ARRAY, 1)
    ZEND_ARG_TYPE_INFO(0, resolution, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_maxUncompactSize, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, compacted, IS_ARRAY, 1)
    ZEND_ARG_TYPE_INFO(0, resolution, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3IndexesAreNeighbors, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, origin, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, destination, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getH3UnidirectionalEdge, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, origin, IS_LONG, 1)
    ZEND_ARG_TYPE_INFO(0, destination, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3UnidirectionalEdgeIsValid, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, edge, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getOriginH3IndexFromUnidirectionalEdge, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, edge, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getDestinationH3IndexFromUnidirectionalEdge, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, edge, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getH3IndexesFromUnidirectionalEdge, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, edge, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getH3UnidirectionalEdgesFromHexagon, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, edge, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getH3UnidirectionalEdgeBoundary, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, edge, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_polyfill, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, geopolygons, IS_ARRAY, 1)
    ZEND_ARG_TYPE_INFO(0, resolution, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_maxPolyfillSize, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, geopolygons, IS_ARRAY, 1)
    ZEND_ARG_TYPE_INFO(0, resolution, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_h3SetToLinkedGeo, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, h3set, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_degsToRads, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, lat_lon, IS_DOUBLE, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_radsToDegs, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, lat_lon, IS_DOUBLE, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hexAreaKm2, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, res, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_hexAreaM2, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, res, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_cellAreaKm2, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, h, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_cellAreaM2, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, h, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_cellAreaRads2, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, h, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_edgeLengthKm, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, res, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_edgeLengthM, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, res, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_exactEdgeLengthKm, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, edge, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_exactEdgeLengthM, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, edge, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_exactEdgeLengthRads, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, edge, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_numHexagons, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, res, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getRes0Indexes, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_res0IndexCount, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_getPentagonIndexes, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, res, IS_LONG, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pentagonIndexCount, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pointDistKm, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, a, IS_ARRAY, 1)
    ZEND_ARG_TYPE_INFO(0, b, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pointDistM, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, a, IS_ARRAY, 1)
    ZEND_ARG_TYPE_INFO(0, b, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_pointDistRads, 0, 0, 0)
    ZEND_ARG_TYPE_INFO(0, a, IS_ARRAY, 1)
    ZEND_ARG_TYPE_INFO(0, b, IS_ARRAY, 1)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ h3_functions[]
 *
 * Every user visible function must have an entry in h3_functions[].
 */
const zend_function_entry h3_functions[] = {
  
    // global helper

    // Indexing functions
    PHP_FE(geoToH3,         arginfo_geoToH3)
    PHP_FE(h3ToGeo,         arginfo_h3ToGeo)
    PHP_FE(h3ToGeoBoundary, arginfo_h3ToGeoBoundary)
    
    // Index inspection functions
    PHP_FE(h3GetResolution, arginfo_h3GetResolution)
    PHP_FE(h3GetBaseCell,   arginfo_h3GetBaseCell)
    PHP_FE(stringToH3,      arginfo_stringToH3)
    PHP_FE(h3ToString,      arginfo_h3ToString)
    PHP_FE(h3IsValid,       arginfo_h3IsValid)
    PHP_FE(h3IsResClassIII, arginfo_h3IsResClassIII)
    PHP_FE(h3IsPentagon,    arginfo_h3IsPentagon)
    PHP_FE(h3GetFaces,      arginfo_h3GetFaces)
    PHP_FE(maxFaceCount,    arginfo_maxFaceCount)
    
    // Grid traversal functions
    PHP_FE(kRing,                   arginfo_kRing)
    PHP_FE(maxKringSize,            arginfo_maxKringSize)
    PHP_FE(kRingDistances,          arginfo_kRingDistances)
    PHP_FE(hexRange,                arginfo_hexRange)
    PHP_FE(hexRangeDistances,       arginfo_hexRangeDistances)
    PHP_FE(hexRanges,               arginfo_hexRanges)
    PHP_FE(hexRing,                 arginfo_hexRing)
    PHP_FE(h3Line,                  arginfo_h3Line)
    PHP_FE(h3LineSize,              arginfo_h3LineSize)
    PHP_FE(h3Distance,              arginfo_h3Distance)
    PHP_FE(experimentalH3ToLocalIj, arginfo_experimentalH3ToLocalIj)
    PHP_FE(experimentalLocalIjToH3, arginfo_experimentalLocalIjToH3)
    
    // Hierarchical grid functions
    PHP_FE(h3ToParent,          arginfo_h3ToParent)
    PHP_FE(h3ToChildren,        arginfo_h3ToChildren)
    PHP_FE(maxH3ToChildrenSize, arginfo_maxH3ToChildrenSize)
    PHP_FE(h3ToCenterChild,     arginfo_h3ToCenterChild)
    PHP_FE(h3Compact,           arginfo_h3Compact)
    PHP_FE(uncompact,           arginfo_uncompact)
    PHP_FE(maxUncompactSize,    arginfo_maxUncompactSize)
    
    // Unidirectional edge functions
    PHP_FE(h3IndexesAreNeighbors,                       arginfo_h3IndexesAreNeighbors)
    PHP_FE(getH3UnidirectionalEdge,                     arginfo_getH3UnidirectionalEdge)
    PHP_FE(h3UnidirectionalEdgeIsValid,                 arginfo_h3UnidirectionalEdgeIsValid)
    PHP_FE(getOriginH3IndexFromUnidirectionalEdge,      arginfo_getOriginH3IndexFromUnidirectionalEdge)
    PHP_FE(getDestinationH3IndexFromUnidirectionalEdge, arginfo_getDestinationH3IndexFromUnidirectionalEdge)
    PHP_FE(getH3IndexesFromUnidirectionalEdge,          arginfo_getH3IndexesFromUnidirectionalEdge)
    PHP_FE(getH3UnidirectionalEdgesFromHexagon,         arginfo_getH3UnidirectionalEdgesFromHexagon)
    PHP_FE(getH3UnidirectionalEdgeBoundary,             arginfo_getH3UnidirectionalEdgeBoundary)
    
    // Region functions
    PHP_FE(polyfill,            arginfo_polyfill)
    PHP_FE(maxPolyfillSize,     arginfo_maxPolyfillSize)
    PHP_FE(h3SetToLinkedGeo,    arginfo_h3SetToLinkedGeo)
    
    // Miscellaneous H3 functions
    PHP_FE(degsToRads,          arginfo_degsToRads)
    PHP_FE(radsToDegs,          arginfo_radsToDegs)
    PHP_FE(hexAreaKm2,          arginfo_hexAreaKm2)
    PHP_FE(hexAreaM2,           arginfo_hexAreaM2)
    PHP_FE(cellAreaKm2,         arginfo_cellAreaKm2)
    PHP_FE(cellAreaM2,          arginfo_cellAreaM2)
    PHP_FE(cellAreaRads2,       arginfo_cellAreaRads2)
    PHP_FE(edgeLengthKm,        arginfo_edgeLengthKm)
    PHP_FE(edgeLengthM,         arginfo_edgeLengthM)
    PHP_FE(exactEdgeLengthKm,   arginfo_exactEdgeLengthKm)
    PHP_FE(exactEdgeLengthM,    arginfo_exactEdgeLengthM)
    PHP_FE(exactEdgeLengthRads, arginfo_exactEdgeLengthRads)
    PHP_FE(numHexagons,         arginfo_numHexagons)
    PHP_FE(getRes0Indexes,      arginfo_getRes0Indexes)
    PHP_FE(res0IndexCount,      arginfo_res0IndexCount)
    PHP_FE(getPentagonIndexes,  arginfo_getPentagonIndexes)
    PHP_FE(pentagonIndexCount,  arginfo_pentagonIndexCount)
    PHP_FE(pointDistKm,         arginfo_pointDistKm)
    PHP_FE(pointDistM,          arginfo_pointDistM)
    PHP_FE(pointDistRads,       arginfo_pointDistRads)

    PHP_FE_END /* Must be the last line in h3_functions[] */
};
/* }}} */

/* {{{ h3_module_entry
 */
zend_module_entry h3_module_entry = {
    STANDARD_MODULE_HEADER,
    "h3",
    h3_functions,
    PHP_MINIT(h3),
    PHP_MSHUTDOWN(h3),
    PHP_RINIT(h3),     /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(h3), /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(h3),
    PHP_H3_VERSION,
    STANDARD_MODULE_PROPERTIES};
/* }}} */

#ifdef COMPILE_DL_H3
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(h3)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
