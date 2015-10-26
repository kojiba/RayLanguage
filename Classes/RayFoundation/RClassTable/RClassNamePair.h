/**
 * RClassNamePair.h
 * ClassNamePair(name and id) for ClassTable.
 * Author Kucheruavyu Ilya (kojiba@ro.ru)
 * 2014 Ukraine Kharkiv
 *  _         _ _ _
 * | |       (_|_) |
 * | | _____  _ _| |__   __ _
 * | |/ / _ \| | | '_ \ / _` |
 * |   < (_) | | | |_) | (_| |
 * |_|\_\___/| |_|_.__/ \__,_|
 *          _/ |
 *         |__/
 **/

#ifndef __R_CLASS_NAME_PAIR_H__
#define __R_CLASS_NAME_PAIR_H__

#include "RayFoundation/RString/RString.h"

class(RClassNamePair)
    discipleOf(RString)

    size_t idForClassName;
endOf(RClassNamePair)


constructor(RClassNamePair));
destructor (RClassNamePair);

void RClassNamePairDeleter(pointer ptr);

printer    (RClassNamePair);

constMethod(RCompareFlags, compareWith, RClassNamePair), RClassNamePair *checkPair);

#endif /*__R_CLASS_NAME_PAIR_H__*/