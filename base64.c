#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define RED 	"\033[01;031m"
#define YELLOW  "\033[01;033m"
#define GREEN 	"\033[01;032m"
#define RESET   "\033[01;0m"
#define CLEAR 	"\033[2J\033[1;1H"

#define SIZE       5
#define FRAME      6      /* encoding BASE64 = 2^6 */
#define BASE64     64

/*const int BASE64 = 64;*/    /*error case*/

//enum e_colour{RED, YELLOW, GREEN, RESET};
typedef enum { e_RED = 1 
//	     , e_YELLOW = YELLOW /*will be int*/
//	     , e_GREEN = GREEN
//	     , e_RESET = RESET
} colour_t;

char TABLE[BASE64];  /*conversion chart, table with encoding value*/ 
const int BITNESS = sizeof(int) * 8;
//unsigned long int * m_bitArray = new unsigned long int[BITNESS];
const int n = 1; /*number of blocks is used calloc() for */
/*Dynamically allocate memory using calloc()*/
//unsigned int * m_bitArray = (unsigned int*)calloc(n, sizeof(unsigned int));
/*Dynamically allocate memory using malloc()*/
unsigned int * m_bitArray = NULL;

void initTABLE(char *table, const int tabCount);
void showTABLE(char *table, int tabCount);
void showMessage(char *m_message, int m_messCount);
void setBit(unsigned int* m_buf, int m_kthbit);
void clearBit(unsigned int * m_buf, int m_kthbit);
/*turn to ASCII table */
void charToBinary(int * m_buff, char m_character, int pos);
/*put m_message into m_buff*/
void encodingString(int *m_buff,  char * m_message, int m_count); 
void showEachBit(int * m_value, int m_kthbit);
void showlByte(int *m_value); /*show in 8bit * 4frame bit format */
void storyTeller(char * a );
void shiftBit(int *m_buff, int m_kthshift);
void copyByte(unsigned int *m_buffSource, unsigned int *m_buffTarget);
void andByte(unsigned int  *m_buffSource, unsigned int *m_buffTarget);

void divideToken(unsigned int * m_bitArray);
void convertToketToDecimal(unsigned int * m_bufftemp,
			   int position, int * resultDecimal);


/****************************
 *    D E F I N I T I O N   *  
 * **************************/

void initTABLE(char *table, const int tabCount)
{
	int half = 26;
	for (int i = 0; i < half; ++i) {
		*(table + i) = (char)(65 + i); }
	for (int i = half; i < 2*half; ++i)	{
		*(table + i) = (char)(71 + i);
	}
	for (int i = 2*half; i < 2*half + 10; i++) {
		*(table + i) = (char)(i - 4);
	}
	*(table + 62) = (char)(half + 17); /* '+' */
	*(table + 63) = (char)(half + 21); /* '/' */
}

void showTABLE(char *table, int tabCount)
{
	printf(YELLOW);
	printf("\tThat is sample substitution cipher\r\n");
	printf(RESET);
	for (int i = 0; i < tabCount; ++i){
			if ( 0 == (i+1) % 8 ) {
				printf("%3c - ", table[i]);	
				printf(YELLOW);
				printf("%2d ", i);
				printf(RESET);
				printf("\r\n");	
			} else {
				printf("%3c - ", table[i]);	
				printf(YELLOW);
				printf("%2d ", i);
				printf(RESET);
			}
	}
	printf("\r\n");
}

void showMessage(char *m_message, int m_messCount)
{
	for (int i = 0; i < m_messCount; ++i){
		printf("%c", m_message[i]);	
	}
	printf("\r\n");
}

void setBit(unsigned int* m_buf, int m_kthbit)
{
	int flag = 1;
	int pos = m_kthbit%32;
	flag = flag << pos;
	*(m_buf) = *(m_buf) | flag;
}

void clearBit(unsigned int * m_buf, int m_kthbit)
{
	int flag = 1;
	int pos = m_kthbit % 32;
	flag = ~flag;
	flag = flag << pos; 
	*(m_buf) = *(m_buf) & flag;
}

void charToBinary(int * m_buff, char m_character, int pos)
{
	unsigned int flag = 1;  /*flag = 0000.....00001 (0:31 bit)  */	
	int sizeOfArray = sizeof(int) * 8;
	sizeOfArray = 8;

	unsigned int * m_bitarraytemp =	NULL; 
	m_bitarraytemp = (unsigned int*)calloc(n, sizeof(unsigned int)); 
	copyByte(m_buff, m_bitarraytemp);   

	for (int i = sizeOfArray-1 + pos; i >=0; --i) {
		if( m_character & flag ) { 	
			setBit(m_buff, i);
		} else {
			clearBit(m_buff, i);
		}	
		flag = flag << 1; 
	}
	andByte(m_bitarraytemp, m_buff);
}

void encodingString(int *m_buff,  char * m_message, int m_count )
{
	char character = *m_message; 
	int begin = 0;
	int end = 8; //m_count;
	int local_count = 0;

	for (int i = 0; i < 4; ++i){
		character = *(m_message + i);
		int pos = i * 8;
		charToBinary(m_buff, character, pos);
	}
}

void showEachBit(int * m_value, int m_kthbit)
{
	int i = m_kthbit/32;
	int pos = m_kthbit% 32; /*which one bit*/
	unsigned int flag = 1; 
	flag = flag << pos;
	if (m_value[i] & flag ) {
		printf(" 1");	
	} else {
		printf(" 0");
	}
}

void showlByte(int *m_value)
{
	int size = sizeof(int) * 8;
	for (int i = 0; i < size; ++i){
		showEachBit(m_value, i);
	}
	printf("\r\n");
}

void shiftBit(int *m_buff, int m_kthshift)
{
		*m_buff = *m_buff << m_kthshift; 	
}

void storyTeller(char * a ) //, colour_t farbe)
{
	char * pivot = a; 
	int length = 0;
	do{
		++length;
		*pivot++;
	} while( '\0' != *pivot);
	printf(GREEN);
	for (int i = 0; i < length; ++i) {
		printf("%c", *(a+i));
	}
	printf(RESET);
}

void copyByte(unsigned int *m_buffSource, unsigned int *m_buffTarget)
{
	unsigned int flag = 1;  /*flag = 0000.....0000001*/
	for (int i = 31; i >=0; --i) {
		flag = 1;
		flag = flag << i;
		if( *m_buffSource & flag ) { 
			setBit(m_buffTarget, i);
		} else {
			clearBit(m_buffTarget, i);
		}	
	}
}

void andByte(unsigned int  *m_buffSource, unsigned int *m_buffTarget)
{
	unsigned int * m_bufftemp = NULL;
	m_bufftemp = (unsigned int*)calloc(n, sizeof(unsigned int)); /*init*/
	copyByte(m_buffTarget, m_bufftemp);
	
	for (int i = 31; i >= 0; --i){
		if ( (1 & (m_bufftemp[i/32] >> (i%32)) |
		   (  1 & m_buffSource[i/32]>> (i%32)))) {
			setBit(m_buffTarget, i);
		} else {
			clearBit(m_buffTarget, i);
		}
	}
}

void divideToken(unsigned int * m_bufftemp)
{
	int tempValue = 0;
	int position = 0;
	int tail = 0;

	while(position < 32) {
		if( 0 == ((position+1) % 6) ){
			convertToketToDecimal(m_bufftemp, position, &tempValue);
			printf("[+]...decimal = %2d", tempValue); 
			char symbol =(char) *(TABLE + tempValue);
			printf("...char = ");
			printf(GREEN);
			printf("%c\r\n", symbol);
			printf(RESET);
			++tail;	
		}
		position++;	
	} 
	tail = 6 * tail;	
	int endSymbolCount = 32 - tail;
        unsigned int * m_bufftail = NULL;
	m_bufftail = calloc(n, sizeof(unsigned int)); /*init*/
	
	if ( (1 & (m_bitArray[tail/32] >> (tail%32))) ) {
		setBit((unsigned int*)m_bufftail, 0);	/*set the first bit*/
	}
	++tail;
	if ( (1 & (m_bitArray[tail/32] >> (tail%32))) ) {
		setBit(m_bufftail, 1);  /*set the second bit*/	
	}
	
	tempValue = 0;
	convertToketToDecimal(m_bufftail, 5, &tempValue);
	printf("[+]...decimal = %2d", tempValue); 
	char symbol =(char) *(TABLE + tempValue);
	printf("...char = ");
	printf(GREEN);
	printf("%c\r\n", symbol);
	printf(RESET);

	printf("\tend of text....\t ");
	for (int i = 0; i < endSymbolCount; ++i){
		printf(GREEN);
		printf("=");
		printf(RESET);
	}
	printf("\n\n");
}

void convertToketToDecimal(unsigned int * m_bufftemp,
			   int position, int *resultDecimal)
{
	*resultDecimal = 0;
	int p = 0;
	for (int i = position; i >= (position-5); --i){
		if (1 & (m_bufftemp[i/32])>> (i%32)){
			int power = 1;
			for (int j = p; j > 0; --j){
				power *= 2;		
			}	
			*resultDecimal += power;	
		} else {
			/*empty case if bit = 0 */	
		}
		++p;
	}
}


int main()
{	
//	char message[SIZE] = {'M', 'A', 'R', 'S'}; 
//      char message[SIZE] = {'B', 'a', 'S', 'e'};
	char message[SIZE] = {'L', 'o', 'R', 'a'};
	
	int m_base = BASE64;
	initTABLE(TABLE, m_base);
	showTABLE(TABLE, m_base);	

	m_bitArray = (unsigned int*)calloc(n, sizeof(unsigned int)); /*init*/

	storyTeller("Initial message --->\t");
	showMessage( message, 5);  	

	encodingString(m_bitArray, message, 4);
	storyTeller("Message encoding with Base64\r\n");
	divideToken(m_bitArray);

	return 0;
}
