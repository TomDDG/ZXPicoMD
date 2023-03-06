#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(int errorcode);


// convert binary file to const uint8_t suitable for ssd1306 OLED
int main(int argc, char* argv[]) {


	if (argc < 4) {
        fprintf(stdout,"Usage rotbin variablename infile outfile\n");   
        exit(0);
    }

    // open files
    FILE *fp_in,*fp_out;
    if ((fp_in=fopen(argv[2],"rb"))==NULL) error(1);
    if ((fp_out=fopen(argv[3],"wb"))==NULL) error(2);
    //
    //
    fseek(fp_in, 0, SEEK_END); // jump to the end of the file to get the length
	uint filesize = ftell(fp_in); // get the file size
    rewind(fp_in);
    //
    if(filesize%8!=0) error(2); // not an even 8bytes
    //
    uint i,j,k;
    uint8_t buff[8]; // read ahead buffer
    uint8_t c,b,a; 
    a=0b10000000;
    b=0b00000001;
    //
    k=0;
    do {
        if(k==0) fprintf(fp_out,"    const uint8_t %s[]={ ",argv[1]);
        else {
            fprintf(fp_out,",\n");
            for(i=0;i<23+strlen(argv[1]);i++) fprintf(fp_out," ");
        }
        for(i=0;i<8;i++) buff[i]=fgetc(fp_in); // read in 8bytes
        for(i=0;i<8;i++) {
            for(j=0,c=0;j<8;j++) if(buff[j]&a>>i) c|=b<<j; // rotate
            fprintf(fp_out,"0x%02x",c);
            if(i<7) fprintf(fp_out,",");
        }
        k+=8;
    } while(k<filesize);
    fprintf(fp_out," };\n");
//
    fclose(fp_in);
    fclose(fp_out);
    return 0;
}
//
void error(int errorcode) {
	fprintf(stdout, "[E%02d]\n", errorcode);
	exit(errorcode);
}