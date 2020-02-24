#include <mqueue.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <stdio.h>
#include <errno.h>

int main()
{

mqd_t mq_id;

/* Name of the message queue should start with / and it cannot have any slashes in between the name.
   The message queues will be created in the virtual file system. To see the message queue in /dev/mqueue dir,
   please follow below steps.

   mkdir /dev/mqueue => create a dir mqueue

   meaning: mount -t (type) device dir
            mount -t mqueue none /dev/mqueue

  mqueue is a valid type which exists in linux kernel.
  
  After the above steps, we can see the message queues created in the /dev/mqueue dir.
  root permission is needed to execute the server and client message queues.
  Change the permission of the server and client to execute without root.
  > ls -ld /dev/mqueue/
    drwxrwxrwt 2 root root 40 Jan 28 16:20 /dev/mqueue/

  > cat /dev/mqueue/mq_name
    QSIZE:0          NOTIFY:0     SIGNO:0     NOTIFY_PID:0

   QSIZE: Number of bytes of data in all messages in the queue.
   NOTIFY_PID: the process with this PID has used mq_notify(3) to register for asynchronous message notification.
   NOTIFY: says Notification method. 0 is SIGEV_SIGNAL; 1 is SIGEV_NONE; and 2 is SIGEV_THREAD
   SIGNO: what signal no SIGEV_SIGNAL uses.

  Once the message queue is created then it will exist till we reboot the system.
  To forcefully remove the message queue, use unlink()
*/
mq_id = mq_open("/mq_name", O_RDWR|O_CREAT );
if(mq_id == -1)
{
  printf("\n server: error in mq:");
  perror("Error: ");
}

/* last argument indicates the priority of the message.
   Higher the number, higher the priority*/
mq_send(mq_id, "SAI BABA", 100, 0 );

mq_send(mq_id, "JAI HANUMMAAN", 1000, 1 );

/* Eventhough "SAI BABA" is written first, "JAI HANUMAAN" is received first by the
   client since it has the higher priority.
  
   This happens because the messages are kept in decreasing order of the priority in the message queue.
*/

return 0;
}



/* Diagram:




user space
             process1             process2

              |                     |
              |                     |

======================================================
                      -------
                      -------
                    Message Queue


Kernel space

*/
