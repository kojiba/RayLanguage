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

#ifndef __R_LIST_H__
#define __R_LIST_H__

#include <RSyntax.h>

typedef struct RNode {
    struct RNode   *next;
    struct RNode   *previous;
           pointer  data;
} RNode;

class(RList)
    RNode  *head;
    RNode  *tail;
    size_t  count;
    void  (*destructorDelegate)(pointer);
    void  (*printerDelegate)   (pointer);
endOf(RList)

constructor (RList));
destructor  (RList);
printer     (RList);

method(void, addHead, RList),    pointer src);
method(void, addTail, RList),    pointer src);

#endif /*__R_LIST_H__*/
