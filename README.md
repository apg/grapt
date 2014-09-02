# grapt - a simple way to make graphs in unix pipelines

## Summary

`grapt` is a set of tools that allow you to easily visualize data from a
simple, easy to produce format. It was developed out of frustration when
I realized that getting the shape of data requires a lot of extra work.

`grapt` *isn't* and will *never* be a fully featured graphing package. 
See [gnuplot](http://gnuplot.info) if you have those needs. It will
however give you just enough insight into your data, and can easily be
invoked at the end of a shell pipeline that produces a bunch of numbers.

## Installation

You'll need [libcairo](http://cairographics.org/), make, and a C compiler.
Then:

```bash
$ make
```

Should get you an executable named `grapt`. Once you have that, copy it
into a place that is configured in your `$PATH`.

## Usage

```bash
$ grapt -h
usage: ./grapt [options]

  -H, --height=HEIGHT   Height of canvas
  -w, --width=WIDTH     Width of canvas
  -o, --output=FILENAME Output filename (defaults output.png)
  -h, --help            This message
  -s, --smooth=WINDOW   Smoothing window
  -t, --tee             Tee input to stdout
  -v, --version         Version information
```

## Examples

### Basic usage: Graph (0, 1), (0, 3), (0, 2)

```bash
$ grapt
1
3
2
^D
```

This should produce a PNG file called output.png, which gives you a basic
2 line segment plot.

![Basic Example](https://raw.githubusercontent.com/apg/grapt/master/examples/basic-usage.png)

### Advanced usage: IOPS for 30 seconds on sda

```bash
$ iostat 1 30 | grep 'sda' | awk '{print $2}' | grapt -o iops.png
```

![30 Second IOPS](https://raw.githubusercontent.com/apg/grapt/master/examples/iops.png)

### Advanced usage: token / word distribution in the grapt source code.

```bash
for n in `cat *.[ch] | sed -e 's/[^0-9a-zA-Z_]/ /g'`; do 
   echo $n; 
done | sort | uniq -c | sort -n | awk '{print $1}' \
| grapt -w 320 -H 240 -o word-dist.png
```

This gives you the word distribution from the source code, which at
the time of this writing was:

![Source code word distribution](https://raw.githubusercontent.com/apg/grapt/master/examples/word-dist.png)

### Advanced usage: Distribution of random numbers with smoothing

```bash
for n in {0..10000}; do 
   echo $(( $RANDOM % 100 )); 
done | sort -n | uniq -c | awk '{print $1}' \
| ./grapt -w 320 -H 240 -o random.png -s 21
```

This gives us 2 series on the plot. The first, in bright red, shows
the distribution of random numbers. The second one, in maroon, shows
us a smoothed version of the red series, using a simple moving average
smoothing algorithm, with a window size of 21. 

![Random Number Distribution](https://raw.githubusercontent.com/apg/grapt/mean-smoother/examples/random-dist-smoothed.png)

## Contributing and Feedback

If you'd like to fix or contribute something, please fork and submit a pull
request, or open an issue. There's lots of room for improvement, and much
more work to be done.

## Authors

Andrew Gwozdziewycz <web@apgwoz.com>

## Copyright

Copyright 2014, Andrew Gwozdziewycz, <web@apgwoz.com>

Licensed under the GNU GPLv3. See LICENSE for more details
