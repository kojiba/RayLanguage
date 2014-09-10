$constructor(SimpleDisciple)
) {
printf("Constructor of SimpleDisciple\n");

//    You decide where you alloc and initialize your masterClassObject, I'm prefer incapsulation
object = $allocator(SimpleDisciple);
$master(object, MyClass
) =
$(NULL, c(MyClass)
) );
object->
string = "Hello world";

return
object;
}

$destructor(SimpleDisciple) {


    $($master(object, MyClass), d(MyClass)) );
    $deallocator($master(object, MyClass));

    printf("Destructor of SimpleDisciple\n");
}

$printer(SimpleDisciple) {
    printf("\n%s object 0x%p: { \n", toString(SimpleDisciple), object);
    printf("Contains ---\n");

    // printer of masterClass
    $($master(object, MyClass), p(MyClass)) );
    printf("--- end Contains\n");
    printf(" string = %s \n", object->string);

    printf("} end of %s object 0x%p \n\n", toString(SimpleDisciple), object);
}
