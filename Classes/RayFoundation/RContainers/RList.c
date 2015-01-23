/**
 * RList.h
 * Realization of C dynamic list, in Ray additions.
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

#include <RList.h>
#include <RClassTable.h>

constructor(RList)) {
    object = allocator(RList);
    if(object != nil) {
            object->head = nil;
            object->tail = object->head;

            object->count = 0;
            object->destructorDelegate = nil;
            object->printerDelegate = nil;
            object->classId = registerClassOnce(toString(RList));
    }
    return object;
}

destructor(RList) {
    RNode *iterator = object->tail;
    if(iterator != nil) {
        if (object->destructorDelegate != nil) {
            for (; iterator != nil; iterator = iterator->next) {
                object->destructorDelegate(iterator->data);
                deallocator(iterator);
            }
        } else {
            for (; iterator->next != nil; iterator = iterator->next) {
                deallocator(iterator);
            }
        }
    }
}

printer(RList) {
    RNode *iterator = object->tail;
    RPrintf("\n%s object %p: { \n", toString(RList), object);
    RPrintf(" Count : %lu \n", object->count);
    for(; iterator != nil; iterator = iterator->next) {
        RPrintf("\t");
        if(object->printerDelegate != nil) {
            object->printerDelegate(iterator->data);
        } else {
            RPrintf("%p \n", iterator->data);
        }
    }
    RPrintf("} end of %s object %p \n", toString(RList), object);
}

method(void, addHead, RList), pointer src) {
    RNode *temp = allocator(RNode);
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
}

method(void, addTail, RList), pointer src) {
    RNode *temp = allocator(RNode);
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
}
