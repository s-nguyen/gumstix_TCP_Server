#include "leptonSPI.h"
#include <iostream>



int spi_cs0_fd = -1;
int spi_cs1_fd = -1;


unsigned char spi_mode = SPI_MODE_3;
unsigned char spi_bitsPerWord = 8;
unsigned int spi_speed = 10000000;



static void pabort(const char *s)
{
	perror(s);
	abort();
}

int leptonSPI::initCamera() {
    int status_value = -1;
        int *spi_cs_fd;


        //----- SET SPI MODE -----
        //SPI_MODE_0 (0,0)  CPOL=0 (Clock Idle low level), CPHA=0 (SDO transmit/change edge active to idle)
        //SPI_MODE_1 (0,1)  CPOL=0 (Clock Idle low level), CPHA=1 (SDO transmit/change edge idle to active)
        //SPI_MODE_2 (1,0)  CPOL=1 (Clock Idle high level), CPHA=0 (SDO transmit/change edge active to idle)
        //SPI_MODE_3 (1,1)  CPOL=1 (Clock Idle high level), CPHA=1 (SDO transmit/change edge idle to active)
        spi_mode = SPI_MODE_3;

        //----- SET BITS PER WORD -----
        spi_bitsPerWord = 8;

        //----- SET SPI BUS SPEED -----
        spi_speed = 10000000;				//10000000 = 10MHz (1uS per bit)



        spi_cs_fd = &spi_cs0_fd;



        *spi_cs_fd = open(std::string("/dev/spidev1.0").c_str(), O_RDWR);


        if (*spi_cs_fd < 0)
        {
            perror("Error - Could not open SPI device");
            exit(1);
        }

        status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MODE, &spi_mode);
        if(status_value < 0)
        {
            perror("Could not set SPIMode (WR)...ioctl fail");
            exit(1);
        }

        status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MODE, &spi_mode);
        if(status_value < 0)
        {
            perror("Could not set SPIMode (RD)...ioctl fail");
            exit(1);
        }

        status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bitsPerWord);
        if(status_value < 0)
        {
            perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
            exit(1);
        }

        status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bitsPerWord);
        if(status_value < 0)
        {
            perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
            exit(1);
        }

        status_value = ioctl(*spi_cs_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
        if(status_value < 0)
        {
            perror("Could not set SPI speed (WR)...ioctl fail");
            exit(1);
        }

        status_value = ioctl(*spi_cs_fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
        if(status_value < 0)
        {
            perror("Could not set SPI speed (RD)...ioctl fail");
            exit(1);
        }
        return(status_value);
}

int leptonSPI::closeCamera()
{
    int status_value = -1;
    int *spi_cs_fd;


    spi_cs_fd = &spi_cs0_fd;


    status_value = close(*spi_cs_fd);
    if(status_value < 0)
    {
        perror("Error - Could not close SPI device");
        exit(1);
    }
    return(status_value);
}

void leptonSPI::getFrame() {
    int resets = 0;
            for(int j=0;j<PACKETS_PER_FRAME;j++) {
                //if it's a drop packet, reset j to 0, set to -1 so he'll be at 0 again loop
                read(spi_cs0_fd, frame+sizeof(uint8_t)*PACKET_SIZE*j, sizeof(uint8_t)*PACKET_SIZE);
                int packetNumber = frame[j*PACKET_SIZE+1];
                if(packetNumber != j) {
                    j = -1;
                    resets += 1;
                    usleep(1000);
                    //Note: we've selected 750 resets as an arbitrary limit, since there should never be 750 "null" packets between two valid transmissions at the current poll rate
                    //By polling faster, developers may easily exceed this count, and the down period between frames may then be flagged as a loss of sync
                    if(resets == 750) {
                        closeCamera();
                        usleep(750000);
                        initCamera();
                    }
                }
            }

}

leptonSPI::leptonSPI() {
    initCamera();
	//Make PGM file
}

leptonSPI::~leptonSPI() {

}
