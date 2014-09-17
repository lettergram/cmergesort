Merge Sort in C
===============

To Compile: gcc mergesort.c -o sort
To Run: time ./sort <Segments> <SAMPLE.txt >/dev/null

## To Run
Command for Run:  ./sort <Segments> SAMPLE.txt
Command for Save: ./sort <Segments>  <SAMPLE.txt >SORTED.txt
Command for Time: time ./sort <Segments> <SAMPLE.txt >/dev/null

Segments refers to the number of segments the data should be brokenup into. In other words, if we had 24 data points, and chose a segment size of 5, we would have 4, 5 data point segments and 1, 4 data point segment.

For each segment I sort using the qsort() function in c.

I provided 3 sample text files with massive ammounts of numbers:

* sampleSetSmall.txt: 	 123 lines @ ~0.5 Kb
* sampleSetMedium.txt: 	 4 314 939 lines @ 29.7Mb 
* sampleSetLarge.txt:  	 9 002 751 lines @ 88.6Mb
* sampleSetUltimate.txt: 100 000 000 lines @ 984.5 Mb (.zip)

<a href="https://www.dropbox.com/sh/ubytgyzswuqqx6y/AACrl2P38R-fuxPvCnyZ1WyZa?dl=0">DropBox link for datasets</a>

Enjoy.