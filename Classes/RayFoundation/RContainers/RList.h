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

#ifndef __R_LIST_H__
#define __R_LIST_H__

#include <RBasics.h>
#include <RArray.h>

#ifdef RAY_LIST_THREAD_SAFE
    #include <RThread.h>
#endif

struct RList;

typedef struct RNode {
    struct RNode   *next;
    struct RNode   *previous;
           pointer  data;
} RNode;

class(RList) //--------------------------------------------------------------------------
    RNode  *head;
    RNode  *tail;

    size_t  count; // readonly, signaling about successful addition, deletion of elements

    void  (*destructorDelegate)(pointer);
    void  (*printerDelegate)   (pointer);

#ifdef RAY_LIST_THREAD_SAFE
    RMutexDescriptor mutex;
#endif
endOf(RList) //--------------------------------------------------------------------------

constructor (RList));
destructor  (RList);
printer     (RList);

// add
method(void,        addHead,       RList),    pointer src);
method(void,        addTail,       RList),    pointer src);

// get
method(pointer,     objectAtIndex, RList),    size_t index);
method(RList *,     subList,       RList),    RRange range); // copy of sub-part, with set-upd delegates

// enumerate
method(RFindResult, enumerate,     RList),    REnumerateDelegate *delegate, rbool isFromLeft);

// delete
method(void,        deleteObjects, RList),    RRange range);
method(void,        deleteObject,  RList),    size_t index);

// casts
method(struct RArray *,    toRArray,      RList));

#endif /*__R_LIST_H__*/
