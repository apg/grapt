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

#define VERSION "0.1.1"

char *config_output_file = DEFAULT_OUTPUT_FILE;
int config_width = DEFAULT_WIDTH;
int config_height = DEFAULT_HEIGHT;

static void
draw_series(series_t *series)
{
  int i;
  double spanx, spany, stepx, stepy;
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

  stepx = config_width / spanx;
  stepy = config_height / spany;

  if (series->pts_used > 0) {
    cairo_move_to(cr, (series->pts[0].x - window.min_x) * stepx,
                  config_height - ((series->pts[0].y - window.min_y) * stepy));

    /* draw the series */
    for (i = 1; i < series->pts_used; i++) {
      cairo_line_to(cr, (series->pts[i].x - window.min_x) * stepx,
                    config_height - \
                    ((series->pts[i].y - window.min_y) * stepy));
    }

    cairo_stroke(cr);
  }

  cairo_destroy(cr);
  cairo_surface_write_to_png(surface, config_output_file);
  cairo_surface_destroy(surface);
}


static char *options = "H:w:o:hv";
static struct option long_options[] = {
  {"height", required_argument,0, 'H' },
  {"width", required_argument, 0, 'w' },
  {"output", required_argument, 0, 'o' },
  {"help", no_argument, 0, 'h' },
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
