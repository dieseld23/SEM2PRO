#include <iostream>
#include <pthread.h> 
main()  {
    pthread_t f2_thread, f1_thread; 
    void *f2(), *f1();
    int i1;
    i1 = 1;
    pthread_create(&f1_thread,NULL,f1,&i1);
    pthread_create(&f2_thread,NULL,f2,&i1);
    pthread_join(f1_thread,NULL);
    pthread_join(f2_thread,NULL);
}
void *f1(int *x){
  std::cout << *x << std::endl;
}
void *f2(int *x){
  sleep(1)
  std::cout << ++(*x) << std::endl;
}