
#include <stdio.h>    /* Standard input/output definitions */
#include <stdlib.h>   /* Standard library definitions */
#include <fcntl.h>    /* File control definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>

#define SERIALPORT "/dev/ttyACM0"

/** Function Declarations **/

//string encodeNetstring(string);
//string decodeNetstring(string);


char *portname = SERIALPORT;
char buf[256];

int main(int argc, const char * argv[]) {
    
    int fd;
    /* Open the file descriptor in non-blocking mode */
    fd = open(portname, O_RDWR | O_NOCTTY);
    
    /* Set up the control structure */
    struct termios toptions;

    /* Get currently set options for the tty */
    tcgetattr(fd, &toptions);
    
    
    //////////////////////////
    //* Set custom options *//
    //////////////////////////
    
    /* 9600 baud */
    cfsetispeed(&toptions, B9600);
    cfsetospeed(&toptions, B9600);
    /* 8 bits, no parity, no stop bits */
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    /* no hardware flow control */
    toptions.c_cflag &= ~CRTSCTS;
    /* enable receiver, ignore status lines */
    toptions.c_cflag |= CREAD | CLOCAL;
    /* disable input/output flow control, disable restart chars */
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
    /* disable canonical input, disable echo,
     disable visually erase chars,
     disable terminal-generated signals */
    toptions.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    /* disable output processing */
    toptions.c_oflag &= ~OPOST;
    
    /* wait for 24 characters to come in before read returns */
    toptions.c_cc[VMIN] = 12;
    /* no minimum time to wait before read returns */
    toptions.c_cc[VTIME] = 0;
    
    /* commit the options */
    tcsetattr(fd, TCSANOW, &toptions);
   
    
//*****************************************//
//*   TODO: Write everything below here...
//*****************************************//
    /*
    NETSTRING_ERROR_TOO_LONG      More than 999999999 bytes in a field
    NETSTRING_ERROR_NO_COLON      No colon was found after the number
    NETSTRING_ERROR_TOO_SHORT     Number of bytes greater than buffer length
    NETSTRING_ERROR_NO_COMMA      No comma was found at the end
    NETSTRING_ERROR_LEADING_ZERO  Leading zeros are not allowed
    NETSTRING_ERROR_NO_LENGTH     Length not given at start of netstring 
    */

    /* Wait for the Arduino to reset */
    usleep(1000*1000);
    /* Flush anything already in the serial buffer */
    tcflush(fd, TCIFLUSH);
    /* read up to 256 bytes from the fd and store in buf */
    int n = read(fd, buf, 128);
    
    /* print how many bytes read */
    printf("%i bytes got read...\n", n);
    /* print what's in the buffer */
    printf("Buffer contains...\n%s\n", buf);

    
    return 0;
}


