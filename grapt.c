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

#include <cairo/cairo.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "grapt.h"

#define DEFAULT_WIDTH 640
#define DEFAULT_HEIGHT 480
#define DEFAULT_OUTPUT_FILE "output.png"
#define DEFAULT_PADDING 10

#define VERSION "0.1.2"

char *config_output_file = DEFAULT_OUTPUT_FILE;
int config_width = DEFAULT_WIDTH;
int config_height = DEFAULT_HEIGHT;
int config_padding = DEFAULT_PADDING;
int config_tee = 0;

static void
draw_series(series_t *series)
{
  int i;
  double spanx, spany, stepx, stepy, thisx, thisy;
  window_t window;

  cairo_surface_t *surface =
    cairo_image_surface_create(CAIRO_FORMAT_ARGB32, \
                               config_width, config_height);
  cairo_t *cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);

  cairo_rectangle(cr, 0, 0, config_width, config_height);
  cairo_fill(cr);

  cairo_set_line_width(cr, 1.0);
  cairo_set_source_rgb(cr, 1.0, 0.0f, 0.0f);

  data_window(series, &window);

  spanx = window.max_x - window.min_x;
  spany = window.max_y - window.min_y;

  stepx = (config_width - (2 * config_padding)) / spanx;
  stepy = (config_height - (2 * config_padding)) / spany;

  if (series->pts_used > 0) {
    thisx = (series->pts[0].x - window.min_x) * stepx;
    thisy = config_height - ((series->pts[0].y - window.min_y) * stepy);

    cairo_move_to(cr, thisx + config_padding, thisy - config_padding);

    /* draw the series */
    for (i = 1; i < series->pts_used; i++) {
      thisx = (series->pts[i].x - window.min_x) * stepx;
      thisy = config_height - ((series->pts[i].y - window.min_y) * stepy);
      cairo_line_to(cr, thisx + config_padding, thisy - config_padding);
    }

    cairo_stroke(cr);
  }

  cairo_destroy(cr);
  cairo_surface_write_to_png(surface, config_output_file);
  cairo_surface_destroy(surface);
}


static char *options = "H:w:o:htv";
static struct option long_options[] = {
  {"height", required_argument,0, 'H' },
  {"width", required_argument, 0, 'w' },
  {"output", required_argument, 0, 'o' },
  {"help", no_argument, 0, 'h' },
  {"tee", no_argument, 0, 't' },
  {"version", no_argument, 0, 'v' },
  {0, 0, 0, 0 }
};

static void
usage(char *arg0)
{
  fprintf(stderr, "usage: %s [options]\n\n"
          "    -H, --height        Height of canvas\n"
          "    -w, --width         Width of canvas\n"
          "    -o, --output        Output filename (defaults output.png)\n"
          "    -h, --help          This message\n"
          "    -t, --tee           Tee input to stdout\n"
          "    -v, --version       Version information\n",
          arg0);
}

static int
optparse(int argc, char *argv[])
{
  char *endptr;
  int c;
  for (;;) {
    c = getopt_long(argc, argv, options,
                    long_options, NULL);
    if (c == -1) {
      break;
    }

    switch (c) {
    case 'H':
      config_height = (int)strtol(optarg, &endptr, 10);
      if (config_height <= 0 || errno == ERANGE || errno != 0) {
        fprintf(stderr, "invalid height\n\n");
        perror("strtol");
        exit(EXIT_FAILURE);
      }
      else if (optarg == endptr) {
        fprintf(stderr, "no digits found for height\n\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'w':
      config_width = (int)strtol(optarg, &endptr, 10);
      if (config_width <= 0 || errno == ERANGE || errno != 0) {
        fprintf(stderr, "invalid width\n\n");
        perror("strtol");
        exit(EXIT_FAILURE);
      }
      else if (optarg == endptr) {
        fprintf(stderr, "no digits found for width\n\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 'o':
      if (strlen(optarg) > 0) {
        config_output_file = optarg;
      } 
      else {
        fprintf(stderr, "invalid output file\n");
        exit(EXIT_FAILURE);
      }
      break;
    case 't':
      config_tee = 1;
      break;
    case 'v':
      fprintf(stderr, "%s version %s\n", argv[0], VERSION);
      exit(EXIT_SUCCESS);
    case '?':
    case 'h':
      usage(argv[0]);
      exit(EXIT_FAILURE);
    }
  }
}

int
main (int argc, char *argv[])
{
  int i;
  series_t series;

  optparse(argc, argv);

  series_init(&series);
  series_read(&series, stdin);
  draw_series(&series);

  return 0;
}
