#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <mqueue.h>

#include <sys/time.h>
#include <sys/resource.h> // needed for struct rlimit

/*Man mq_overview*/

int main(int argc, char *argv[])
{
    struct mq_attr attr;
    
    /* getrlimit() and setrlimit() gets and sets the resource limits.
       Each resource is associated with soft and hard limit
       struct rlimit {
               rlim_t rlim_cur; =>  Soft limit 
               rlim_t rlim_max; =>  Hard limit (ceiling for rlim_cur) 
           };

      The soft limit is the value that the kernel enforces for the corre‐
      sponding resource. The hard limit acts as a ceiling for the soft
      limit.

      an unprivileged process can set only its soft limit to a value
      in the range from 0 up to the hard limit.

      A privileged process/one with the CAP_SYS_RESOURCE capability
      can make arbitary changes to both the limit values.

      The value RLIM_INFINITY denotes no limit on a resource.
 
      RLIMIT_MSGQUEUE: places a limit on the amount of space that can be consumed by
      all of the message queues belonging to a process's real user ID.

      It gives 819200 bytes(800 KB) by default.
    */
    struct rlimit limit;
    if(getrlimit(RLIMIT_MSGQUEUE,&limit) == -1) {
        perror("getrlimit");
        return 1;
    }
    printf("RLIMIT_MSGQUEUE cur = %ld,max = %ld\n",(long)limit.rlim_cur,(long)limit.rlim_max);

    mqd_t q = mq_open("/mqtest",O_RDWR|O_CREAT,0660,NULL);
    if(q == -1) {
        perror("mq_open");
        return 1;
    }
    if (mq_getattr(q, &attr) == -1) {
        perror("mq_getattr");
        return 1;
    }
    printf("mq_flags %ld\n",  attr.mq_flags);

    /* Maximum number of messages in a queue. It will be 10 by default
     cat /proc/sys/fs/mqueue/msg_max
    */
    printf("mq_maxmsg %ld\n", attr.mq_maxmsg);

    /* Maximum message size. By default 8192 bytes (8 KB)
       cat /proc/sys/fs/mqueue/msgsize_max.
       It can be set upto 2^31-1 (INT_MAX)  */
    printf("mq_msgsize %ld\n",attr.mq_msgsize);

    /* No of messages in the queue
       Bydefault it gives 0.
    */
    printf("mq_curmsgs %ld\n",attr.mq_curmsgs);
    mq_close(q);
    mq_unlink("/mqtest");
    return 0;
}
