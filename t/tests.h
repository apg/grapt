#ifndef QUICK_TESTS_H_
#define QUICK_TESTS_H_

#include <stdio.h>

static int QTS_FAILURES = 0;
static int QTS_SUCCESSES = 0;

#define QTS_WINNER QTS_SUCCESSES++; fprintf(stderr, " ... SUCCESS\n")
#define QTS_LOSER QTS_FAILURES++; \
  fprintf(stderr, " ... FAILURE [%s:%d]\n", __FILE__, __LINE__)
#define QTS_ANNOUNCE(z) fprintf(stderr, "Test '%s'", z);
#define QTS_TEST(name, test) do { QTS_ANNOUNCE(name);  \
  if (test) { QTS_WINNER; } \
  else { QTS_LOSER; }      \
  } while (0);


#define IS(x, z) QTS_TEST(z, (x))
#define IS_EQ(x, y, z) QTS_TEST(z, ((x) == (y)))
#define IS_NE(x, y, z) QTS_TEST(z, ((x) != (y)))
#define IS_GE(x, y, z) QTS_TEST(z, ((x) >= (y)))
#define IS_LE(x, y, z) QTS_TEST(z, ((x) <= (y)))
#define IS_GT(x, y, z) QTS_TEST(z, ((x) > (y)))
#define IS_LT(x, y, z) QTS_TEST(z, ((x) < (y)))

int
report_results(char *name) 
{
  int total = QTS_SUCCESSES + QTS_FAILURES;
  float percent = (float)QTS_SUCCESSES / ((total > 0) ? total : 1) * 100;
  fprintf(stderr, "\n%s\n     Results: %d / %d "
          "(%.2f%% success rate), %d failures.\n",
          name, QTS_SUCCESSES, total, percent, QTS_FAILURES);
  return QTS_FAILURES;
}

#endif
