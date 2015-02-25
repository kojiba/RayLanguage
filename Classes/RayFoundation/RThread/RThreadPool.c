#include <RThreadPool.h>
#include <RClassTable.h>

constructor(RThreadPool)) {
    object = allocator(RThreadPool);
    if(object != nil) {
        object->threads = makeRArray();
        if(object->threads != nil) {
            object->threads->destructorDelegate = RFree;
            object->threads->printerDelegate = (void (*)(pointer)) p(RThread);
            object->classId = registerClassOnce(toString(RThreadPool));
        } else {
            deallocator(object);
            object = nil;
        }
    }
    return object;
}

destructor(RThreadPool) {
    deleter(object->threads, RArray);
}

printer(RThreadPool) {
    RPrintf("%s object %p {", toString(RThreadPool), object);
    $(object->threads, p(RArray)));
    RPrintf("} end of %s\n", toString(RThreadPool));
}

method(void, addWithArg, RThreadPool), pointer argumentForNewWorker) {
//    RPrintf("malloc - %p\n", malloc);
    RThread *newOne = $(nil, c(RThread)), nil, object->delegateFunction, argumentForNewWorker);
//    RPrintf("malloc - %p\n", malloc);
    if(newOne != nil) {
//        RPrintf("Thread - %p added\n", newOne);
        $(object->threads, m(addObject, RArray)), newOne);
    } else {
        RError("RThreadPool. Add with arg bad worker allocation.", object);
    }
}

method(void, addWorker,  RThreadPool), RThread *worker) {
    $(object->threads, m(addObject, RArray)), worker);
}

method(void, join, RThreadPool)) {
    object->enumerator.virtualCheckObject = (rbool (*)(pointer, size_t)) m(join, RThread);
    $(object->threads, m(enumerate, RArray)), &object->enumerator, no);
}