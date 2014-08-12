#ifndef GRAPT_H_
#define GRAPT_H_

/**
 * A point, which is contained in a series.
 */
typedef struct point {
  double x;
  double y;
} point_t;

/**
 * A series is a group of points.
 */
typedef struct series {
  point_t *pts;
  size_t pts_used;
  size_t pts_alloc;
  struct series *next;
} series_t;

void series_init(series_t *series);
void series_destroy(series_t *series);
void series_append(series_t *series, point_t pt);

/* Copies the pts, used and alloc, in a series. NULLs the next.
 */
int series_copy(series_t *series, series_t *to);
void series_transform(series_t *series, point_t (*func)(point_t x));
double series_minx(series_t *series);
double series_miny(series_t *series);
double series_maxx(series_t *series);
double series_maxy(series_t *series);
void series_sort_points(series_t *series);

void series_read(series_t *series, FILE *in);


#endif
