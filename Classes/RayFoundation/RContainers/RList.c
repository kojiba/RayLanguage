/**
 * RList.h
 * Realization of C dynamic
 * double-linked list, in Ray additions.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 1/23/15 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#define copyDelegatesToResult() result->destructorDelegate = object->destructorDelegate; \
                                result->printerDelegate    = object->printerDelegate

#include <RList.h>
#include <RClassTable.h>

#ifdef RAY_LIST_THREAD_SAFE
    #define listMutex &object->mutex
    #define RMutexLockList() RMutexLock(listMutex)
    #define RMutexUnlockList() RMutexUnlock(listMutex)
#else
    // sets empty
    #define listMutex
    #define RMutexLockList()
    #define RMutexUnlockList()
#endif

constructor(RList)) {
    object = allocator(RList);
    if(object != nil) {
        object->head = nil;
        object->tail = object->head;

        object->count = 0;
        object->destructorDelegate = nil;
        object->printerDelegate = nil;
        object->classId = registerClassOnce(toString(RList));
#ifdef RAY_LIST_THREAD_SAFE
        mutexWithType(&object->mutex, RMutexNormal); // note: be aware with normal mute
#endif
    }
    return object;
}

destructor(RList) {
    RNode *iterator = object->tail;
    RNode *temp;
    RMutexLockList();
    if (object->destructorDelegate != nil) {
        while(iterator != nil) {
            temp = iterator->next;
            object->destructorDelegate(iterator);
            deallocator(iterator);
            iterator = temp;
        }
    } else {
       while(iterator != nil) {
            temp = iterator->next;
            deallocator(iterator);
            iterator = temp;
        }
    }
    RMutexUnlockList();
}

printer(RList) {
    RNode *iterator = object->tail;
    RMutexLockList();
    RPrintf("%s object %p: { \n", toString(RList), object);
    RPrintf(" Count : %lu \n", object->count);
    for(; iterator != nil; iterator = iterator->next) {
        RPrintf("\t");
        if(object->printerDelegate != nil) {
            object->printerDelegate(iterator->data);
        } else {
            RPrintf("%p \n", iterator->data);
        }
    }
    RPrintf("} end of %s object %p \n\n", toString(RList), object);
    RMutexUnlockList();
}

#pragma mark Add

method(void, addHead, RList), pointer src) {
    RNode *temp = allocator(RNode);
    RMutexLockList();
    if(temp != nil) {
        temp->data = src;
        temp->next = nil;
        if (object->count == 0) {
            object->head = temp;
            object->tail = temp;
            temp->previous = nil;
        } else {
            object->head->next = temp;
            temp->previous = object->head;
            object->head = temp;
        }
        ++object->count;
    }
    RMutexUnlockList();
}

method(void, addTail, RList), pointer src) {
    RNode *temp = allocator(RNode);
    RMutexLockList();
    if(temp != nil) {
        temp->data = src;
        temp->previous = nil;
        if (object->count == 0) {
            object->head = temp;
            object->tail = temp;
            temp->next = nil;
        } else {
            object->tail->previous = temp;
            temp->next = object->tail;
            object->tail = temp;
        }
        ++object->count;
    }
    RMutexUnlockList();
}

#pragma mark Private Node At Index

constMethod(RNode *, nodeAtIndex, RList), size_t index) {
    size_t  delta   = object->count - index;
    RNode *iterator = nil;
    RMutexLockList();
    if(delta <= object->count) {
        // go from head
        if(delta < object->count / 2) {
            iterator = object->head;
            for (; delta != 0; --delta) {
                iterator = iterator->previous;
            }

        // go from tail
        } else {
            iterator = object->tail;
            for (; index != 0; --index) {
                iterator = iterator->next;
            }
        }
    } else {
        RWarning("RList. Bad index for node.", object);
    }
    RMutexUnlockList();
    return iterator;
}

#pragma mark Get

constMethod(pointer, objectAtIndex, RList), size_t index) {
    RNode *node = $(object, m(nodeAtIndex, RList)), index);
    if(node != nil) {
        return node->data;
    }
    return nil;
}

method(RList *, subList, RList), RRange range) {
    if(range.size + range.start <= object->count) {
        RList *result = $(nil, c(RList)));
        if(result != nil) {
            rbool toLeftDirection = yes;
            RNode *iterator = nil;

            // set up delegates
            copyDelegatesToResult();

            if(range.start < object->count / 2) {
                iterator = $(object, m(nodeAtIndex, RList)), range.start);
            } else {
                iterator = $(object, m(nodeAtIndex, RList)), range.start + range.size);
                toLeftDirection = no;
            }
            RMutexLockList();
            if(toLeftDirection) {
                for(; range.size != 0; --range.size) {
                    $(result, m(addHead, RList)), iterator->data);
                    iterator = iterator->next;
                }

            } else {
                for(; range.size != 0; --range.size) {
                    $(result, m(addTail, RList)), iterator->data);
                    iterator = iterator->previous;
                }
            }
            RMutexUnlockList();
            return result;
        }
    } else {
        RWarning("RList. Bad range to get sub list.", object);
    }
    return nil;
}

#pragma mark Enumerate

constMethod(RFindResult, enumerate, RList), REnumerateDelegate *delegate, rbool isFromLeft) {
    RNode *iterator = object->tail;
    size_t numericIterator;
    RFindResult result;
    result.index  = object->count;
    result.object = nil;
    RMutexLockList();
    if(isFromLeft) {
        for (numericIterator = 0; iterator != nil; iterator = iterator->next, ++numericIterator) {
            if(!$(delegate, m(checkObject, REnumerateDelegate)), iterator->data, numericIterator)) {
                break;
            }
        }
    } else {
        for (numericIterator = object->count - 1; iterator != nil; iterator = iterator->previous, --numericIterator) {
            if(!$(delegate, m(checkObject, REnumerateDelegate)), iterator->data, numericIterator)) {
                break;
            }
        }
    }
    if(iterator != nil) {
        result.index = numericIterator;
        result.object = iterator->data;
    }

    RMutexUnlockList();
    return result;
}

#pragma mark Delete

method(void, deleteObjects, RList), RRange range) {
    if(range.size + range.start <= object->count) {
        RNode *iterator = nil,
              *storedHead = nil,
              *storedTail = nil;

        rbool toLeftDirection = yes;

        if(range.start < object->count / 2) {
            iterator = $(object, m(nodeAtIndex, RList)), range.start);
        } else {
            iterator = $(object, m(nodeAtIndex, RList)), range.start + range.size);
            toLeftDirection = no;
        }
        RMutexLockList();
        object->count -= range.size;

        // if moves to head ---->
        if(toLeftDirection) {
            storedTail = iterator->previous;

            if(object->destructorDelegate != nil) {
                for(; range.size != 0; --range.size) {
                    object->destructorDelegate(iterator->data);
                    storedHead = iterator->next;
                    deallocator(iterator);
                    iterator = storedHead;
                }
            } else {
                for(; range.size != 0; --range.size) {
                    storedHead = iterator->next;
                    deallocator(iterator);
                    iterator = storedHead;
                }
            }


        // if moves to tail <----
        } else {
            storedHead = iterator->next;

            if(object->destructorDelegate != nil) {
                for(; range.size != 0; --range.size) {
                    object->destructorDelegate(iterator->data);
                    storedTail = iterator->previous;
                    deallocator(iterator);
                    iterator = storedTail;
                }
            } else {
                for(; range.size != 0; --range.size) {
                    storedTail = iterator->previous;
                    deallocator(iterator);
                    iterator = storedTail;
                }
            }
        }

        // finally connect
        if(storedTail != nil) {
            storedTail->next = storedHead;
        } else {
            object->tail = iterator;
        }

        if(storedHead != nil) {
            storedHead->previous = storedTail;
        } else {
            object->head = iterator;
        }
        RMutexUnlockList();
    } else {
        RWarning("RList. Bad range to delete.", object);
    }
}

method(void, deleteObject,  RList), size_t index) {
    RNode *node = $(object, m(nodeAtIndex, RList)), index);
    if(node != nil) {
        RMutexLockList();
        if(object->destructorDelegate != nil) {
            object->destructorDelegate(node->data);
        }
        node->next->previous = node->previous;
        node->previous->next = node->next;
        deallocator(node);
        --object->count;
        RMutexUnlockList();
    }
}

#pragma mark Casts

constMethod(RArray*, toRArray, RList)) {
    RArray *result = makeRArrayOptions(object->count, sizeMultiplierOfRArrayDefault, nil);
    if(result != nil) {
        copyDelegatesToResult();

        RMutexLockList();
        RNode *iterator = object->tail;
        for (; iterator != nil; iterator = iterator->next) {
            $(result, m(addObjectUnsafe, RArray)), iterator->data);
        }
        RMutexUnlockList();
    }
    return result;
}

