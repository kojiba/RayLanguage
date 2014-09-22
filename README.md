RayLanguage
===========

Ray additions to C language defines some C-based syntax, 
that makes object-oriented life easier for C developers.
All based on defines. Here you can find dynamic array for C.

Include:
```C
#include "RayFoundation.h"
```

Declare class:

```C
class(MyClass)
    int *a;
    float *b;
endOfClass(MyClass)
```

Declare methods:

```C
$constructor(MyClass));
$destructor(MyClass);
$method(void, MyMethod, MyClass), int a);
$staticMethod(void, MyStaticMethod, MyClass), double b);
$printer(MyClass);
```

Declare disciple

```C
$class(SimpleDisciple) $discipleOf(MyClass)
    char *string;
$endOfClass(SimpleDisciple)
```

Implement methods:

```C
$method(void, MyMethod, MyClass), int a) {
    if (a == 1) {
        printf("Hello world.\n");
    } else {
        printf("Goodbye world.\n");
    }
}

constructor(MyClass)) {
    printf("Constructor\n");

    object = $allocator(MyClass);
//    You can get name of class in predeclared variable, but it's only optionally
//    a->nameOfClass = TOSTRING(MyClass);
    object->a = $allocator(int);
    object->b = $allocator(float);

    object->a[0] = 2;
    object->b[0] = 20.3;

    return object;
}

destructor(MyClass) {

    $deallocator(object->a);
    $deallocator(object->b);
    object->a = 0;
    object->b = 0;

    printf("Destructor of MyClass\n");
}

printer(MyClass) {
    printf("\n%s object 0x%.x: { \n", TOSTRING(MyClass), object);

    printf(" a = %d \n", object->a[0]);
    printf(" b = %f \n", object->b[0]);

    printf("} end of %s object 0x%.x \n\n", TOSTRING(MyClass), object);
}

staticMethod(void, MyStaticMethod, MyClass), double b) {
    printf("My static method with double %f \n", b);
}
```

Creating and work with object:
```C
printf("Heap: \n\n");
    // Work with pointers
    MyClass *ob1;
    ob1 = $(NULL, c(MyClass)) );
    $(ob1, m(MyMethod, MyClass)), 1);
    $(ob1, m(MyMethod, MyClass)), 26062000);

    $(NULL, sm(MyStaticMethod, MyClass)), 0.451);

    $(ob1, p(MyClass)));
    $(ob1, d(MyClass)));
    // additional pointer cleanup
    $(ob1, free));

    // Work with stack
    printf("\nStack: \n\n");
    {
        MyClass ob2 = *$(NULL, c(MyClass)) );
        $(&ob2, m(MyMethod, MyClass)), 1);
        $(&ob2, m(MyMethod, MyClass)), 23082011);

        $(NULL, sm(MyStaticMethod, MyClass)), 0.451);

        $(&ob2, p(MyClass)) );
        $(&ob2, d(MyClass)) );
    }
```
