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


#define BIND_PORT 22223
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
    char buff[BUFF_SIZE];
    int i;
    for(i=0;i<BUFF_SIZE;i++) buff[i] = 0x00;

    init_counter();
    
    
    struct sockaddr_in serv_adr;

    int sock = socket(AF_INET,SOCK_STREAM,0);
    

    
    return 0;
}



void init_counter(){
    _fd = open(COUNTER_DEV_FILE,O_RDWR);
    if(_fd < 0){
      printf("Error open %s file!\nMay be driver not loaded?\n",COUNTER_DEV_FILE);
      exit(-1);
    }
    printf("File %s is open, descriptor: %d\n",COUNTER_DEV_FILE,_fd);
    return;
}


void start_counter(){
    
    
    return;
}

void stop_counter(){


    
    return;
}

int read_counter(){


    return;
}

void write_counter_register(unsigned int regID, unsigned int value){
    ixpio_reg_t reg;
    reg.id = regID;
    reg.value = value;

    
    if(ioctl(_fd, IXPIO_WRITE_REG, &reg)){
        sigaction(SIGALRM, &act_old, NULL);
	printf("ERROR write to register! RegID: %d, value: %d\n",regID,value);
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
	printf("ERROR read register! RegID: %d\n",regID);
	close(_fd);
        exit(-1);
    }

    return reg.value;

}


void sig_handler(int sig){
    overflow ++;
    printf("Interrupt! Overflow +1: %d\n",overflow);
}
