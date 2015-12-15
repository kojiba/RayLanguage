#ifndef __TESTS_H__
#define __TESTS_H__

#ifdef  __cplusplus
extern "C" {
#endif

void ComplexTest();

int RDynamicArrayTest (void);
int RListTest         (void);
int RClassNamePairTest(void);
int RClassTableTest   (void);
int RDictionaryTest   (void);
int StringArrayTest   (void);
int RDataTest         (void);
int RBufferTest       (void);
int RStringTest       (void);
int RThreadTest       (void);
int RThreadPoolTest   (void);

#ifdef  __cplusplus
}
#endif

#endif /*__TESTS_H__*/