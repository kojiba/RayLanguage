$constructor(MyClass)
) {
printf("Constructor of MyClass\n");

object = $allocator(MyClass);
//    You can get name of class in predeclared variable, but it's only optionally
//    a->nameOfClass = TOSTRING(MyClass);
object->
a = $allocator(
int);
object->
b = $allocator(
float);

object->a[0] = 2;
object->b[0] = 20.3;

return
object;
}

$destructor(MyClass) {

    $deallocator(object->a);
    $deallocator(object->b);

    printf("Destructor of MyClass\n");
}

$method(void, MyMethod, MyClass),

int a
) {
if (a == 1) {
printf("Hello world.\n");
} else {
printf("Goodbye world.\n");
}
}

$printer(MyClass) {
    printf("\n%s object 0x%p: { \n", toString(MyClass), object);

    printf(" a = %d \n", object->a[0]);
    printf(" b = %f \n", object->b[0]);

    printf("} end of %s object 0x%p \n\n", toString(MyClass), object);
}

$staticMethod(void, MyStaticMethod, MyClass),

double b
) {
printf("My static method with double %f \n", b);
}
