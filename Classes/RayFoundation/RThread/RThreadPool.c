#include <RThreadPool.h>
#include <RClassTable.h>

constructor(RThreadPool)) {
    object = allocator(RThreadPool);
    if(object != nil) {
        object->threads = makeRArray();
        if(object->threads != nil) {
//            object->threads->destructorDelegate = (void (*)(pointer)) RThreadDeleter;
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

method(void, addWithArg, RThreadPool), pointer argumentForNewWorker) {
    RThread *newOne = $(nil, c(RThread)), nil, object->delegateFunction, argumentForNewWorker);
    if(newOne != nil) {
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
    $(object->threads, m(enumerate, RArray)), &object->enumerator);
}