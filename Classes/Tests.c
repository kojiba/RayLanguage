#include <time.h>
#include <RayFoundation.h>

int RByteArrayTest(void) {
    size_t i;
    size_t j;
    for(i = 0; i < 32; ++i) {
        RByteArray *array = makeRByteArray(i);
        if(i == 31) {
            for(j = 0; j < 31; ++j) {
                array->array[j] = (byte) j;
            }
            RAY_TEST(array->size != 31, "RByteArrayTest. Bad array size", -1);
        }
        deleter(array, RByteArray);
    }
    return 0;
}

int StringDictionaryTest(void) {
    // some key constant
    RCString *key = RS("Veider");

    // create dictionary
    RStringDictionary *dictionary = $(nil, c(RStringDictionary)) );
    master(dictionary, RDictionary)->values->destructorDelegate = (void (*)(pointer)) stringDeleter;
    master(dictionary, RDictionary)->keys->destructorDelegate = (void (*)(pointer)) stringDeleter;

    // fill dictionary with some object-keys,
    // use RSC, cause we need copies of constant,
    // also use m(copy, RCString)
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Value"), RSC("Key"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Leia"), RSC("Han Solo"));
    $(dictionary, m(setObjectForKey, RStringDictionary)), RSC("Luke"), $(key, m(copy, RCString))) );

    RAY_TEST(dictionary->masterRDictionaryObject->keys->count > 3
            || dictionary->masterRDictionaryObject->values->count > 3,
            "StringDictionaryTest. Duplication of setBy key", -1);

    // find some object for key
    RCString *object = $(dictionary, m(getObjectForKey, RStringDictionary)), key);
    RAY_TEST(object == nil, "StringDictionaryTest. Error find object for key", -2)
    else {
        deallocator(key);
    }

    // destructs, and delete pointer
    deleter(dictionary, RStringDictionary);
    return 0;
}



int StringArrayTest(void) {
    unsigned i;
    RArray *stringArray = makeRArray();
    stringArray->destructorDelegate = (void (*)(pointer)) stringDeleter;
    for(i = 0; i < 10; ++i) {
        addObjectToRA(stringArray, stringWithFormat("Temp string %i", i));
    }
    RAY_TEST(stringArray->count != 10, "String array size is not 10", -1);

    $(stringArray, m(deleteObjects, RArray)), makeRRange(5, 4));
    RAY_TEST(stringArray->count != 6, "String array delete objects error", -2);
    deleter(stringArray, RArray);

    return 0;
}

int RDictionaryTest(void){
    RDictionary *dictionary = makeRDictionary();
    size_t iterator;
    fromStartForAll(iterator, 1, 20){
        size_t value = iterator;
        size_t key = iterator;
        $(dictionary, m(setObjectForKey, RDictionary)), (pointer) value, (pointer) key);
        RAY_TEST(dictionary->keys->count != dictionary->values->count, "Tests. RDictionaryCount of keys and values is not equal.", -1);
        RAY_TEST(dictionary->keys->count == 0 || dictionary->values->count == 0, "Tests. RDictionaryCount of keys or values 0.", -2);
    }
    deleter(dictionary, RDictionary);
    return 0;
}

int RClassTableTest(void){
    size_t iterator;
    registerClassOnce("Luke");
    registerClassOnce("Dart");
    registerClassOnce("Leia");
    registerClassOnce("Han Solo");
    // try once more, but here is only one record

    forAll(iterator, 100) {
        registerClassOnce("Leia");
        registerClassOnce("Dart");
        registerClassOnce("Luke");
    }

    RAY_TEST(RCTSingleton->masterRArrayObject->count > 20, "RCTSingleton->masterRArrayObject->size > 20", -1);
    RAY_TEST(RCTSingleton->masterRArrayObject->count == 0, "RCTSingleton->masterRArrayObject->size == 0", -2);
    char *checkName = "Leia";
    size_t id = $(RCTSingleton, m(getIdentifierByClassName, RClassTable)), checkName);
    RAY_TEST(id < 4, "RClassTable. Bad id for registered.", -3);
    return 0;
}

int RClassNamePairTest(void){
    RClassNamePair *pair = $(nil, c(RClassNamePair)) );
    if(pair != nil) {
        $(master(pair, RCString), m(setConstantString, RCString)), "RClassNamePairTest");
        pair->idForClassName = 4;
        deleter(pair, RClassNamePair);
    }
    return 0;
}

int RDynamicArrayTest(void){
    int i;
    RArray *dynamicArray = makeRArray();
    RAY_TEST(dynamicArray == nil, "Make RArray error", -1);
    dynamicArray->destructorDelegate = free;

    for (i = 0; i < 22; ++i) {
        char *a = RAlloc(sizeof(char) * 8);
        if(a != nil) {
            RMemCpy(a, "Hello  ", sizeof("Hello  "));
            a[6] = (char) (i % 10 + 48);
            addObjectToRA(dynamicArray, a);
        }
        RAY_TEST(dynamicArray->count != i + 1, "RArray bad size", -2);
    }

    sortRA(dynamicArray);

    RArray *sub = $(dynamicArray, m(getSubarray, RArray)), makeRRange(10, 10));
    RAY_TEST(sub == nil, "Subarray error.", -3);
    sub->destructorDelegate = nil;
    RAY_TEST(sub->count != 10, "Subarray bad size.", -4);
    sizeToFitRA(sub);
    RAY_TEST(sub->freePlaces != 0, "Subarray bad sizeToFit.", -5);

    deleter(dynamicArray, RArray);
    deleter(sub, RArray);
    return 0;
}

int RBufferTest(void) {
    size_t iterator;
    size_t sum = 0;
    size_t first_size = 0;
    RBuffer *buffer = $(nil, c(RBuffer)));

    forAll(iterator, 20) {
        RCString *temp = randomRCString();
        $(buffer, m(addData, RBuffer)), temp->baseString, temp->size + 1);
        sum += temp->size + 1;
        if(iterator == 0) {
            first_size = temp->size;
        }
        RAY_TEST(((buffer->count != iterator + 1) || buffer->totalPlaced != sum),
                "RBuffer. Test error, bad size or size", -1);

        deleter(temp, RCString);
    }
    char *temp = $(buffer, m(getDataCopy, RBuffer)), 0);

    RAY_TEST(RStringLength(temp) != first_size, "RBuffer. Bad getDataCopy", -2);

    deleter(buffer, RBuffer);
    deallocator(temp);
    return 0;
}

class(RListFinder)
    REnumerateDelegate master;

endOf(RListFinder)

rbool checkObjectRListFinder(pointer data, size_t index) {
    if(index == 2) {
        return no;
    }
    return yes;
}

void printfInt(pointer ptr) {
    printf("%qu\n", (unsigned long long int) ptr);
}

int RListTest(void) {
    size_t iterator;
    RListFinder finder;
    // link virtual
    finder.master.virtualCheckObject = checkObjectRListFinder;

    RList *list = constructorOfRList(nil);
    list->printerDelegate = printfInt;

    forAll(iterator, 20) {
        $(list, m(addHead, RList)), (pointer) iterator);
    }
    RAY_TEST(list->count != 20, "RList. Bad add head.", -1);

    RList* result = $(list, m(subList, RList)), makeRRange(5, 5));
    RAY_TEST(!result, "RList. Bad sublist.", -2);
    RAY_TEST(result->count != 5, "RList. Bad sublist count.", -3);

    RFindResult founded = $(result, m(enumerate, RList)), &finder.master, yes);

    RAY_TEST(!founded.object, "RList. Bad founded object.", -4);
    RAY_TEST(founded.index == list->count, "RList. Bad finded index.", -5);

    deleter(result, RList);

    $(list, m(deleteObjects, RList)), makeRRange(10, 10));
    RAY_TEST(list->count != 10, "RList. Bad count on delete.", -6);

    RArray *array = $(list, m(toRArray, RList)));
    RAY_TEST(!array, "RList. Bad array from list", -7);
    RAY_TEST(list->count != array->count && array->count != 10, "RList. Bad count of array from list.", -8);

    deleter(array, RArray);
    deleter(list, RList);
    return 0;
}

#define TEST_COUNT 10

RArray *arrayTest;

pointer func1(pointer arg) {
    int i;
    forAll(i, TEST_COUNT) {
        $(arrayTest, m(addObject, RArray)), RS((char*)arg));
    }
    return 0;
}

int RThreadTest(void) {
    storePtrs();
    arrayTest = makeRArray();
    int i;
    arrayTest->destructorDelegate = free;
    RThread thread1;
    RThreadCreate(&thread1, nil, func1, "1 thread");

    forAll(i, TEST_COUNT) {
        $(arrayTest, m(addObject, RArray)), RS("main"));
    }

    $(&thread1, RThreadJoin));

    RAY_TEST(arrayTest->count != 2 * TEST_COUNT, "RThread bad array count.", -1);
    deleter(arrayTest, RArray);
    backPtrs();
    return 0;
}

int RCStringTest(void) {
    RCString *testString = RSC(
            "Warcraft II: Tides of Darkness is a fantasy-themed real-time strategy (RTS)"
                    "game published by Blizzard Entertainment and first released for DOS in 1995 and for Mac OS in 1996. The main game, "
                    "Warcraft II: Tides of Darkness, earned enthusiastic reviews, won most of the major PC gaming awards in 1996, and sold over 2 million copies. \n"
                    "\n"
                    "Later in 1996 Blizzard released an expansion pack Warcraft II: Beyond the Dark Portal for DOS and Mac OS, and a compilation Warcraft II: "
                    "The Dark Saga for the Sony PlayStation and Sega Saturn. The BattleNet Edition, released in 1999, provided Blizzard's online gaming service, "
                    "Battle.net, and replaced the MS-DOS version with a Windows one.\n"
                    "\n"
                    "Players must collect resources, and produce buildings and units in order to defeat an opponent in combat on the ground, in the air and in some "
                    "maps at sea. The more advanced combat units are produced at the same buildings as the basic units but also need the assistance of other buildings, "
                    "or must be produced at buildings that have prerequisite buildings. The majority of the main screen shows the part of the territory on which the player "
                    "is currently operating, and the minimap can select another location to appear in the larger display. The fog of war completely hides all territory which "
                    "the player has not explored, and shows only terrain but hides opponents' units and buildings if none of the player's units are present.\n"
                    "\n"
                    "Warcraft II 's predecessor Warcraft: Orcs & Humans, released in 1994, gained good reviews, collected three awards and was a finalist for three others, "
                    "and achieved solid commercial success. The game was the first typical RTS to be presented in a medieval setting and, by bringing multiplayer facilities "
                    "to a wider audience, made this mode essential for future RTS titles. Warcraft: Orcs & Humans laid the ground for Blizzard's style of RTS, which emphasized "
                    "personality and storyline. Although Blizzard's very successful StarCraft, first released in 1998, was set in a different universe, it was very similar to "
                    "Warcraft II in gameplay and in attention to personality and storyline. In 1996 Blizzard announced Warcraft Adventures: Lord of the Clans, an adventure game "
                    "in the Warcraft universe, but canceled the game in 1998. "
                    "Warcraft III: Reign of Chaos, released in 2002, used parts of Warcraft Adventures' characters and storyline and extended the gameplay used in Warcraft II."
    );

    RArray *words = $(testString, m(substringsSeparatedBySymCStr, RCString)), ".,:; /-!@=+?\n\t&");

    RAY_TEST(words == nil, "RCString. Words array wasn't created.", -1);
    RAY_TEST(words->count != 393, "RCString. Bad text words count.", -2);

    deleter(words, RArray);

    $(testString, m(deleteAllSubstringsCStr, RCString)), "\n");
    $(testString, m(removeRepetitionsOf, RCString)), ' ');

    RArray *sents = $(testString, m(substringsSeparatedBySymCStr, RCString)), ".");
    if(sents != nil) {
        RAY_TEST(sents->count != 15, "RCString. Bad text sentances count.", -2);
        deleter(sents, RArray);
    }
    deleter(testString, RCString);
    return 0;
}

void ComplexTest() {
    srand((unsigned int) time(nil));

    RPrintCurrentSystem();
    RPrintf("Number of processors - %u \n", processorsCount());
    RPrintf("Main tuid - %qu \n", currentTreadIdentifier());

    if(
           !RDynamicArrayTest()
        && !RListTest()
        && !RClassNamePairTest()
        && !RClassTableTest()
        && !RDictionaryTest()
        && !StringArrayTest()
        && !StringDictionaryTest()
        && !RByteArrayTest()
        && !RBufferTest()
        && !RCStringTest()
        #ifndef __WIN32
            && !RThreadTest() // fixme in progress
        #endif
    ) {
        RPrintLn("All tests passed successfully\n");
    } else {
        RError("TESTS ERROR!", nil);
    }
}