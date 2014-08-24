/**
 * This file is part of grapt
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

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

typedef struct window {
  double min_x, max_x, min_y, max_y;
} window_t;

void series_init(series_t *series);
void series_destroy(series_t *series);
void series_append(series_t *series, point_t pt);

/* Copies the pts, used and alloc, in a series. NULLs the next.
 */
int series_copy(series_t *series, series_t *to);
void series_transform(series_t *series, point_t (*func)(point_t x));
void series_window(series_t *series, window_t *window);
void series_sort_points(series_t *series);
void series_read(series_t *series, FILE *in);

/**
 * Function used to scale the points in every series.
 */
typedef void (series_scale_fun)(point_t *pt, void *udata);

/**
 * Scale each series of points by an arbitrary function.
 * UDATA is an environment with extra info for the function.
 */
int series_scale(series_t *series, series_scale_fun *f, void *udata);

void data_window(series_t *series, window_t *window);

#endif
