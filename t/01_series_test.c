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
  IS_EQ((int)series_minx(&series), 1, "minx == 1");
  IS_EQ((int)series_maxx(&series), 10000, "maxx == 10000");
  IS_EQ((int)series_miny(&series), 1, "miny == 1");
  IS_EQ((int)series_maxy(&series), 100000000, "maxy == 100000000");

  return report_results(argv[0]);
}
