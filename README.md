# clock-sketch2020

Clock-sketch: a sketch-based algorithm with clock framework that can address item batch measurement

This repostory contains all the codes related to the manuscript submitted to SIGMOD 2021.

BOBHash is used as the hash function in the code.

Technical_report is the paper which contains all pseudo codes for four algorithms.

We give both single thread and multiple thread implementation for Clock-sketch.

Due to the file size limitation in Github, we cannot upload all of our used datasets. Please download the CAIDA dataset from https://www.caida.org/data/, https://cmt3.research.microsoft.com/SIGMOD2021/Submission/Details/25 and http://snap.stanford.edu/data/.



# Some Modifications

We've corrected some typos in this paper, which shall better help your understanding.

1. The source of dataset Criteo shall be https://ailab.criteo.com/criteo-attribution-modeling-bidding-dataset/.

2. In section 3.3, paragraph 2:
   "Item 𝑎 only appears in the error window, and may incur error in the measurement. Item 𝑏 only appears in the standard window and has no outdated information. Item 𝑐
appears in both the error window and the standard window, which may incur error in some applications (e.g., item batch activeness measurement) while not in other applications (e.g., item batch size measurement)."
  This graph should be corrected as follows:
  "Item in the error window may incur error in the measurement. Item 𝑎 only appears in the error window. It may cause false positive in activeness measurement. Item 𝑐 appears in both error window and standard window. Thus its occurance in the error window does not affect activeness measurement, which is already positive. Item 𝑏 only appears in the standard window and has no outdated information."
