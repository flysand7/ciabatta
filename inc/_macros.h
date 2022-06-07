
#pragma once

#define _str_(x) #x
#define _str(x) _str_(x)

#define _con(a,b) a ## b

#define _countof(arr) (sizeof (arr) / sizeof ((arr)[0]))
