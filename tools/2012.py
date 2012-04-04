import time

import horology

def format(ut, dt):
    ul, dl = map(time.localtime, (ut, dt))

    s = "    {.uh = %2d, .um = %2d, .dh = %2d, .dm = %2d}, // %04d-%02d-%02d" % (ul.tm_hour, ul.tm_min, dl.tm_hour, dl.tm_min, ul.tm_year, ul.tm_mon, ul.tm_mday)
    return s

lon = -10.756389
lat = 59.949444

t = 0

print """#include <inttypes.h>

#include "sun.h"

/*
struct sun_day {
    uint8_t uh;
    uint8_t um;
    uint8_t dh;
    uint8_t dm;
};
*/

const struct sun_day sun_up_down[] = {"""

for m in range(0, 13):
    for d in range(0, 32):
        try:
            t = time.mktime(time.strptime("2012-%02d-%02d 12:00" % (m, d), "%Y-%m-%d %H:%M"))
        except ValueError:
            pass

        up, noon, down = horology.sun_events(t, lon, lat)

        print format(up, down)

print "};"
