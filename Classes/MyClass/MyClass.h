#ifndef __MY_CLASS_H__
#define __MY_CLASS_H__

#include "RayFoundation.h"

$class(MyClass)
        int *a;
                float *b;
                $endOfClass(MyClass)

                $constructor(MyClass)
);
$destructor(MyClass);

$method(void, MyMethod, MyClass),

int a
);

$staticMethod(void, MyStaticMethod, MyClass),

double b
);
$printer(MyClass);

#endif /*__MY_CLASS_H__*/