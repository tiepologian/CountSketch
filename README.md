# CountSketch
Simple C++ implementation of the Count Sketch algorithm

### Features
* C++11
* header-only
* currently supports strings and ints


### Usage
Import library
```
#include "CountSketch.h"
```

Create a counter
```
CountSketch counter(0.01, 0.1);
```

Add a few strings
```
counter.addString("Pizza");
counter.addString("Chips");
counter.addString("Pizza");
```

Get frequency
```
counter.getStringFrequency("Pizza")
```

Output is 2
