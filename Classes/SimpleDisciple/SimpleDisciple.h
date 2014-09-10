#ifndef __SIMPLE_DISCIPLE_H__
#define __SIMPLE_DISCIPLE_H__

#include "RayFoundation.h"
#include "MyClass.h"

$class(NewMyClass)   //temp class
$endOfClass(NewMyClass)

//----------------------------------------------------------------------------------------------------------------------

$class(SimpleDisciple)
$discipleOf(MyClass)
$discipleOf(NewMyClass)
        char *string;
                $endOfClass(SimpleDisciple)

                $constructor(SimpleDisciple)
);
$destructor(SimpleDisciple);
$printer(SimpleDisciple);

#endif /*__SIMPLE_DISCIPLE_H__*/