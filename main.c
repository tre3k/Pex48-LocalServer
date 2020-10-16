#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>


#include <sys/socket.h>
#include <netinet/in.h>

#include <ixpio.h>


#define BIND_PORT 22224
#define BUFF_SIZE 256

#define COUNTER_DEV_FILE "/dev/ixpio1"


/* define const RegID */
// interrupt registers
static const unsigned int INT_MASK_REG =                 0x03;
static const unsigned int AUX_PIN_STAT_REG =             0x04;
static const unsigned int INT_POL_REG =                  0x05;

// Read/Write Port ports and Control worlds wBase+0xc0 ... +0xdc
static const unsigned int PORTA_REG =                     0x0d;
static const unsigned int PORTB_REG =                     0x0e;
static const unsigned int PORTC_REG =                     0x0f;
static const unsigned int CONTROL_WORD_IO =               0x10;

// Counters register
static const int COUNTER0_REG =                           0x15;
static const int COUNTER1_REG =                           0x16;
static const int COUNTER2_REG =                           0x17;
static const int CW_8254 =                                0x18;

static const int CLOCK_INT_CW =                           0x1d;


static unsigned int _fd = 0;
static unsigned int overflow = 0;
static ixpio_signal_t sig;
static struct sigaction act;
static struct sigaction act_old;


int main(int argc,char **argv){
    char cmd = 0x00;
    int i, conn;

    init_counter();
    
    
    struct sockaddr_in serv_addr,client_addr;
    socklen_t client_len;

    int sock = socket(AF_INET,SOCK_STREAM,0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(BIND_PORT);

    if(bind(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){
      printf("sock: Error bind!\n");
      exit(-1);
    }
    
    if(listen(sock,10)<0){
      printf("sock: Error listen port: %d\n",BIND_PORT);
      exit(-1);
    }


    client_len = sizeof(client_addr);
    while(1){
      cmd = 0x00;
      conn = accept(sock,(struct sockaddr *)&client_addr,&client_len);
      if(conn < 0){
	printf("sock: Error accept!\n");
      }
      recv(conn,&cmd,1,0);
      printf("sock: accept command: %c\n",cmd);

      switch(cmd){
	// start command
      case 's':
	break;
      }
      
      
    }
    
    return 0;
}



void init_counter(){
    _fd = open(COUNTER_DEV_FILE,O_RDWR);
    if(_fd < 0){
      printf("counter: Error open %s file!\nMay be driver not loaded?\n",COUNTER_DEV_FILE);
      exit(-1);
    }
    printf("counter: File %s is open, descriptor: %d\n",COUNTER_DEV_FILE,_fd);
    return;
}


void start_counter(){
    printf("counter: Start counter!\n");
    
    
    return;
}

void stop_counter(){
    printf("counter: Stop counter!\n");

    
    return;
}

int read_counter(){
    unsigned int count = 123;
    printf("counter: current count: %d", count);

    return;
}

void write_counter_register(unsigned int regID, unsigned int value){
    ixpio_reg_t reg;
    reg.id = regID;
    reg.value = value;

    
    if(ioctl(_fd, IXPIO_WRITE_REG, &reg)){
        sigaction(SIGALRM, &act_old, NULL);
	printf("counter: ERROR write to register! RegID: %d, value: %d\n",regID,value);
        close(_fd);
	exit(-1);
    }
    
    return;
}

unsigned int read_counter_register(unsigned int regID){
    ixpio_reg_t reg;
    reg.id = regID;
    reg.value = 0;

    if(ioctl(_fd, IXPIO_READ_REG, &reg)){
        sigaction(SIGALRM, &act_old, NULL);
	printf("counter: ERROR read register! RegID: %d\n",regID);
	close(_fd);
        exit(-1);
    }

    return reg.value;

}

void set_counter1(uint16_t value){
    write_counter_register(COUNTER0_REG,value & 0x00ff);
    write_counter_register(COUNTER0_REG,(value & 0xff00)>>8);
    return;
}

uint16_t get_counter1() {
    uint16_t retval = 0;
    uint8_t c_l = 0, c_h = 0;

    c_l = read_counter_register(COUNTER0_REG)&0xff;
    c_h = read_counter(COUNTER0_REG)&0xff;

    retval = 65535-((c_h<<8)|c_l);
    return retval;
}



void sig_handler(int sig){
    overflow ++;
    printf("counter: Interrupt! Overflow +1: %d\n",overflow);
}
