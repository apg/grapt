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

#include <stdlib.h>
#include <stdio.h>
#include "../grapt.h"
#include "./tests.h"

int
main(int argc, char **argv)
{
  int n = 10000;
  series_t series;
  point_t pt;
  window win;
  double value;

  series_init(&series);

  IS_EQ(series.pts_alloc, 0, "initial alloc is 0");
  IS_EQ(series.pts_used, 0, "initial used is 0");
  IS_EQ(series.pts, NULL, "inital pts is NULL");
  IS_EQ(series.next, NULL, "initial next is NULL");

  while (n > 0) {
    pt.x = (double)n;
    pt.y = (double)n * n;
    series_append(&series, pt);
    n--;
  }

  IS_EQ(series.pts_used, 10000, "pts_used is 10000");
  IS_GE(series.pts_alloc, 10000, "pts_alloc ge 10000");

  series_window(&series, &win);

  IS_EQ((int)win.min_x, 1, "minx == 1");
  IS_EQ((int)win.max_x, 10000, "maxx == 10000");
  IS_EQ((int)win.min_y, 1, "miny == 1");
  IS_EQ((int)win.max_y, 100000000, "maxy == 100000000");

  return report_results(argv[0]);
}
