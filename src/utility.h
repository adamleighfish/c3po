//
//  utility.h
//  c3p0
//
//  Created by Adam Fish on 11/4/17.
//  Copyright © 2017 Adam Fish. All rights reserved.
//

#ifndef utility_h
#define utility_h

#include <cmath>

inline int RoundAndClamp(double x) {
    int min = 0;
    int max = 255;
    int raw = int(std::floor(255 * x + 0.5));

    if (raw < min) {
        return min;
    } else if (raw > max) {
        return max;
    }
    return raw;
}

#endif /* utility_h */
