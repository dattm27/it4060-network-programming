/*
OK
Tính số PI sử dụng đa luồng
*/
#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<pthread.h>
long num_steps = 1000000;
#define NUM_THREADS 8

double local_sum[NUM_THREADS];

// định nghĩa hàm thực thi của luồng
void *thread_proc(void *);

double step;
int main () {
    double x, sum = 0;
     step = 1.0/num_steps;

    
    pthread_t tids[NUM_THREADS];
    int params[NUM_THREADS];

    struct timespec start, stop;
    clock_gettime(CLOCK_REALTIME, &start);
    for(int i = 0 ; i< NUM_THREADS; i++){
        params[i] = i;
        pthread_create(&tids[i], NULL, thread_proc, &params[i]);
    }

    for(int i = 0 ; i < NUM_THREADS; i++){
        pthread_join(tids[i], NULL);
    }
   
    
    
    //tach vong lap for ra thanh nhieu phan, moi pha duoc cong boi mot luong rieng
    sum = 0;
   for (int i = 0;  i < NUM_THREADS; i++)
    {
        sum += local_sum[i];

    }

    clock_gettime(CLOCK_REALTIME, &stop);
    double pi = sum*step;
    printf("PI = %.10f\n", pi);
    double elapsed = (stop.tv_sec - start.tv_sec) * 1e3 + (stop.tv_nsec - start.tv_nsec)/1e6;
    printf("elapsed time = %.10f ms\n", elapsed );

    
}

void *thread_proc(void *arg){
    int idx = *(int *) arg;
    double x, csum = 0 ;
    for (int i = idx ; i< num_steps ; i+= NUM_THREADS){
        x = (i+ 0.5)*step;
        csum += 4.0/ (1.0  + x*x);
    }
    local_sum[idx] = csum;

}