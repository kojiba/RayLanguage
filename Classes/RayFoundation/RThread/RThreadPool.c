#include <RThreadPool.h>
#include <RClassTable.h>

constructor(RThreadPool)) {
    object = allocator(RThreadPool);
    if(object != nil) {
        object->threads = makeRArray();
        if(object->threads != nil) {
            object->threads->destructorDelegate = RThreadDeleter;
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
    RThread *newOne = $(nil, c(RThread)), nil, object->delegate, argumentForNewWorker);
    $(object->threads, m(addObject, RArray)), newOne);
}

method(void, addWorker,  RThreadPool), RThread *worker) {
    $(object->threads, m(addObject, RArray)), worker);
}

method(void, join, RThreadPool)) {
    object->enumerator.checkObject = (rbool (*)(pointer, size_t)) m(join, RThread);
    $(object->threads, m(enumerate, RArray)), &object->enumerator);
}