#include <mqueue.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <stdio.h>        /* For mode constants */
#include <errno.h>

int main()
{

mqd_t mq_id;
char buf[1000] = {0};


mq_id = mq_open("/mq_name", O_RDWR|O_CREAT );

if(-1 == mq_id)
{
  printf("\n error in mq");
  perror("Error: ");
}

/* mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio);
   msg_len (buf length) should be greater than mq_msgsize (8192 Bytes).
   msg_prio argument returns the priority of the message */
if(-1 == mq_receive(mq_id, buf, 9000, NULL ))
{
  printf("\n error:");
  perror("Error cli");;
}
else
  printf("\n received %s", buf);


if(-1 == mq_receive(mq_id, buf, 9000, NULL ))
{
  printf("\n error:");
  perror("Error cli");;
}
else
  printf("\n received %s", buf);

return 0;

}
