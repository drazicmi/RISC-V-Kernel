//#include "../lib/hw.h"
//#include "../h/Thr.hpp"
//#include "../h/print.hpp"
//#include "../h/workers.hpp"
//
//
//
//static uint64 fibonacci(uint64 n)
//{
//    if (n == 0 || n == 1) { return n; }
//    if (n % 4 == 0) Thr::yield();
//    return fibonacci(n - 1) + fibonacci(n - 2);
//}
//
//void workerBodyA(void* arg)
//{
//    uint8 i = 0;
//    for (; i < 3; i++)
//    {
//        printString("A: i=");
//        printInteger(i);
//        printString("\n");
//    }
//
//    printString("A: yield\n");
//    __asm__ ("li t1, 7");
//    Thr::yield();
//
//    uint64 t1 = 0;
//    __asm__ ("mv %[t1], t1" : [t1] "=r"(t1));
//
//    printString("A: t1=");
//    printInteger(t1);
//    printString("\n");
//
//    uint64 result = fibonacci(20);
//    printString("A: fibonaci=");
//    printInteger(result);
//    printString("\n");
//
//    for (; i < 6; i++)
//    {
//        printString("A: i=");
//        printInteger(i);
//        printString("\n");
//    }
//
//    Thr::running->setFinished(true);
//    Thr::yield();
//}
//
//void workerBodyB(void* arg)
//{
//    uint8 i = 10;
//    for (; i < 13; i++)
//    {
//        printString("B: i=");
//        printInteger(i);
//        printString("\n");
//    }
//
//    printString("B: yield\n");
//    __asm__ ("li t1, 5");
//    Thr::yield();
//
//    uint64 result = fibonacci(23);
//    printString("A: fibonaci=");
//    printInteger(result);
//    printString("\n");
//
//    for (; i < 16; i++)
//    {
//        printString("B: i=");
//        printInteger(i);
//        printString("\n");
//    }
//
//    Thr::running->setFinished(true);
//    Thr::yield();
//}
//
//void threadExitTest(void* arg)
//{
//    printString("Ispis treba");
//    thread_exit();
//    printString("Ispis ne treba");
//}
//
//void threadJoinRadilica(void* arg)
//{
//    int i = 0;
//    while(i < 5)
//    {
//        printString("i = ");
//        printInteger(i);
//        printString("\n");
//        i++;
//        Thr::yield();
//    }
//    printString("Nit radilica se zavrsila\n");
//}
//
//void threadJoinCekalica(void* arg){
//    printString("Nit cekalica pocinje cekanje\n");
//    thread_join((thread_t)arg);
//    printString("Nit cekalica je sacekala i zavrsila se\n");
//}
