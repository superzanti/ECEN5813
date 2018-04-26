#Project 3 Readme File

For project 3, we were tasked  with implementing profiling code for standard,
custom, and DMA memmove and memset operations. For the Beaglebone Black, we
implemented only the functionality for the standard and custom functions.
Our findings agree with expectations about speed of various functions, namely
that the DMA operations were the fastest, except when the DMA overhead itself
took longer than simply transfering the bytes. following this, the standard
library memset and memmove functions were in second place, with consistently
fast speeds in both memmove and memset. In last place were our custom functions,
which are, while written appropriately, not optimized for the hardware they are
running on.

What was interesting to note was the consistent speed of the standard library
memset functions, with little change in speed between the 4 cases. Specifically
on the BBB (unoptimized), we saw the set [1562, 1337, 1687, 3575] for the 4 cases.
While the latter 3 cases increase reasonably with increasing requirements, it is
strange to see that the 10 byte case had seemingly extra overhead, possibly
due to up to 4 unaligned writes. A potential maximum
set of operations of (1 byte, 2 bytes, 4 bytes, 2 bytes, 1 byte).

this behavior was nearly the same in the optimized case, with [1550, 1312, 1675, 3587]
cycles.

this behavior was not seen in the memmove functions, which appear to increase
linearly as expected.

We have been unable to find the source of a scaling factor of 16 in our
systick profiling on the kl25z, however the same trends are largely seen.
what does stick out is the clear difference between the standard library memset
and memmove behavior, however, as they do increase linearly as originally expected.
As compared with our custom functions, the std libraries take roughly half the time,
meanwhile the DMA, when non-overlapping, outpaces both by 100 bytes, (visible from
memset's behavior) but when it has to deal with  overlap (as with memmove) it outpaces
the library functions by the 1000 byte trial.

most spectacular is the difference  between the DMA and the libraries by the
time it reaches the 5000 byte mark, taking roughly 1/10 the time of the standard libraries,
and approximately 1/60th the time of our custom functions.
