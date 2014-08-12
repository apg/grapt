#include <cairo/cairo.h>
#include <stdlib.h>
#include <stdio.h>

#include "grapt.h"

#define WIDTH 640
#define HEIGHT 480


static void
draw_series(series_t *series)
{
  int i;
  double miny, minx, maxy, maxx, spanx, spany, stepx, stepy;

  cairo_surface_t *surface =
    cairo_image_surface_create(CAIRO_FORMAT_ARGB32, WIDTH, HEIGHT);
  cairo_t *cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

  cairo_rectangle(cr, 0, 0, WIDTH, HEIGHT);
  cairo_fill(cr);

  cairo_set_line_width(cr, 1.0);
  cairo_set_source_rgb(cr, 1.0, 0.0f, 0.0f);

  minx = series_minx(series);
  miny = series_miny(series);
  maxx = series_maxx(series);
  maxy = series_maxy(series);

  spanx = maxx - minx;
  spany = maxy - miny;

  stepx = WIDTH / spanx;
  stepy = HEIGHT / spany;

  printf("minx=%lf maxx=%lf miny=%lf maxy=%lf spanx=%lf spany=%lf stepx=%lf stepy=%lf\n",
         minx, maxx, miny, maxy, spanx, spany, stepx, stepy);

  printf("pt.x - minx * stepx = %lf\n", (series->pts[0].x - minx) * stepx);
  printf("H - (pt.y - miny * stepy = %lf\n", HEIGHT - ((series->pts[0].y - miny) * stepy));

  if (series->pts_used > 0) {
    cairo_move_to(cr, (series->pts[0].x - minx) * stepx,
                  HEIGHT - ((series->pts[0].y - miny) * stepy));

    /* draw the series */
    for (i = 1; i < series->pts_used; i++) {

      printf("pt[%d].x - minx * stepx = %lf\n", i, (series->pts[i].x - minx) * stepx);
      printf("H - (pt[%d].y - miny * stepy = %lf\n", i, HEIGHT - ((series->pts[i].y - miny) * stepy));


      cairo_line_to(cr, (series->pts[i].x - minx) * stepx,
                    HEIGHT - ((series->pts[i].y - miny) * stepy));
    }

    cairo_stroke(cr);
  }

  cairo_destroy(cr);
  cairo_surface_write_to_png(surface, "output.png");
  cairo_surface_destroy(surface);

}


int
main (int argc, char *argv[])
{
  int i;
  series_t series;
  series_init(&series);

  series_read(&series, stdin);

  double maxy = series_maxy(&series);

  for (i = 0; i < series.pts_used; i++) {
    printf("x = %lf, y = %lf\n", series.pts[i].x, series.pts[i].y);
  }

  draw_series(&series);

  return 0;
}
