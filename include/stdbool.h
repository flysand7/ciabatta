
#pragma once

#if !defined(__bool_true_false_are_defined)
    typedef _Bool bool;
    #define true ((bool)1)
    #define false ((bool)0)
    #define __bool_true_false_are_defined
#endif
