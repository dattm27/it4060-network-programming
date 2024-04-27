#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main () {
    printf("Before FORK\n");
    /*fork(); //tao mot ban sao cua tien trinh cha -> 2 
    fork(); //goi tien trinh chinh va tien trinh con tren -> 2 */
    // -> 4 lan after fork;
   
   /*pid_t pid = fork();  //tao mot ban sao cua tien trinh cha -> 2 
    printf("After FORK pid = %d\n", pid); */
    
   /*
   if (fork()==0){
        printf("Child process\n");
    }
    else {
        printf("Main process\n");
    }*/
    /*
    printf("Before foker, pid= %d\n", getpid());
    fork();
    fork();
    printf("After fork, pid = %d\n", getpid());*/
   /*for (int i = 0 ; i < 5 ;i++){
        if (fork () ==0){ // child process
            int pid = getpid();
            printf("child process %d started\n", pid);
            exit(0);
        }
    }
    */
   /*if(fork() ==0){
        printf("Child process started\n");
        sleep(10);
        printf("Child process done\n");
        exit(0);
   }
   sleep(2);
 
   printf("Parent process done\n");*/
   /*if(fork() ==0){
        printf("Child process started\n");
        sleep(10);
        printf("Child process done\n");
        exit(0);
   }
   printf("Waiting for child process\n");
   wait(NULL);
   printf("Parent process done\n");*/

   // Giả sử có 2 tiến trình con
   if(fork() ==0){
        printf("Child process started\n");
        sleep(15);
        printf("Child process done\n");
        exit(0);
   }
     if(fork() ==0){
        printf("Child process started\n");
        sleep(10);
        printf("Child process done\n");
        exit(0);
   }
    pid_t pid = wait(NULL); // trả về id của tiến trình chạy 10s (1 tiến trình kết thúc trước)    
    printf("Child process terminated: %d\n", pid);
    printf("Parent process done\n");
    return 0;
}