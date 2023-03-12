#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define SIZE 36                         
// Hàm random 
unsigned char randomVI()
{
    return (unsigned char)(rand()%256);          // random 1 kí tự kiểu unsigned char
}

// Hàm đưa 1 kí tự char về 0-15 --> hex
unsigned char  char_to_hex(unsigned char ch)
{
    if(ch >= '0' && ch<= '9') return ch -'0';
    if(ch >= 'a' && ch<= 'F') return ch -'a'+10;
    if(ch >= 'A' && ch<= 'f') return ch -'A'+10;
    return -1;
}

// Hàm ghép 2 kí tự char đưa về hệ 16
unsigned char getbyte_hex()
{
    unsigned char hex;
    unsigned char c1,c2;
    scanf("%c%c",&c1,&c2);
    hex = (char_to_hex(c1)<<4)  | (char_to_hex(c2));
    return hex;
}

// Hàm lấy bit thứ n của mảng
unsigned char nbit(unsigned char arr[],int n)
{
    int  nbyte = (n-1)/8;
    int nbit = ((n-1)%8)+1;
    return (arr[nbyte] & (1<<(8-nbit))) >> (8- nbit);
}

// Hàm gán bit vào thanh ghi
void insert_bit(unsigned char arr[],int n,unsigned char value)
{
    int  nbyte = (n-1)/8;
    int nbit = ((n-1)%8)+1;
    arr[nbyte] = ((255<<(9-nbit)) & arr[nbyte]) | (value << (8-value)) | ((255 >> nbit) & arr[nbyte]);
}

// XOR & AND các bit thanh ghi và quay 1 bit
unsigned char rotate(unsigned char arr[], int size)
{
    // AND 2 bit cuối mỗi thanh ghi
    unsigned char andA = nbit(arr,91) & nbit(arr,92);
    unsigned char andB = nbit(arr,175) & nbit(arr,176);
    unsigned char andC = nbit(arr,286) & nbit(arr,287);
    
    // XOR các bit trên cùng 1 thanh ghi
    unsigned char xorA = nbit(arr,66) ^ nbit(arr,93);
    unsigned char xorB = nbit(arr,162) ^ nbit(arr,177);
    unsigned char xorC = nbit(arr,243) ^ nbit(arr,288);

    // XOR giữa các bit trên các thanh ghi khác nhau
    unsigned char sB = andA ^ nbit(arr,171) ^ xorA;
    unsigned char sC = andB ^ nbit(arr,264) ^ xorB;
    unsigned char sA = andC ^ nbit(arr,69) ^ xorC;

    unsigned char out = xorA ^ xorB ^ xorC;
    // Quay thanh ghi 1 bit ngược chiều kim đồng hồ
    for (int i = size-1; i > 0; i--)
    {
       arr[i] = (arr[i-1] << 7) | (arr[i] >> 1);
    }
    arr[0] = arr[0] >> 1;

    // Gán các bit cho đầu ba thanh ghi
    insert_bit(arr,1,sA);
    insert_bit(arr,94,sB);
    insert_bit(arr,178,sC);

    return out;
}

// Hàm trả về kí tự đã được mã hóa
unsigned char encrypt_char(unsigned char arr[])
{
    unsigned char tmp=0;
    int i=0;
    while (i != 8)
    {
        unsigned char z = rotate(arr,SIZE);
        tmp = tmp | (z << i);
        i += 1;
    }
    return tmp;
}

int main()
{
    unsigned char thanh_ghi[SIZE];               //Thanh ghi là mảng 36 byte = 288 bit
    unsigned char IV[10];                        //IV là mảng 10 byte = 80 bit
    unsigned char key[10];                       //Khóa k là mảng 10 byte = 80 bit
    srand(time(NULL));
    // Khởi tạo
    FILE *in,*out;
    in = fopen("trivium_input.txt","r");
    out = fopen("trivium_output.txt","w");
    if(in == NULL ) printf("Khong mo duoc file input.\n");
    if(out == NULL ) printf("Khong mo duoc file output.\n");
    for (int i = 0; i < SIZE; i++)
    {
       thanh_ghi[i] = 0;                                 // Gán 288 bit bằng 0
    }
    for (int i = 0; i < 10; i++)
    {
        IV[i] = randomVI();
    }
    printf("Nhap khoa k (20 ki tu hexa):\n");
    for (int i = 0; i < 10; i++)
    {
        key[i] = getbyte_hex();
    }
    for (int i = 0; i < 10; i++)
    {
        fwrite(&IV[i],1,1,out);                 // Ghi 80 bit IV vào file trivium_output 
    }
    
    for (int i = 0; i < 80; i++)
    {
       insert_bit(thanh_ghi,1+i,nbit(IV,i+1));                 // Gán 80 bit IV vào 80 bit trái nhất thanh ghi A 
       insert_bit(thanh_ghi,94+i,nbit(key,i+1));               // Gán 80 bit key vào 80 bit trái nhất thanh ghi B
    }
    // Gán 3 bit cuối thanh ghi C bằng 1
    insert_bit(thanh_ghi,286,1);
    insert_bit(thanh_ghi,287,1);
    insert_bit(thanh_ghi,288,1);

    // Pha khởi động bỏ qua 1152 lần
    for (int i = 0; i < 1152; i++)
    {
        rotate(thanh_ghi,SIZE);
    }
    unsigned char tmp;
    unsigned char entmp;
    while (fread(&tmp,1,1,in) != 0)
    {
       entmp = tmp ^ encrypt_char(thanh_ghi);
       fwrite(&entmp,1,1,out);
    }
    fclose(out);
    fclose(in);
    printf("---------------Done--------------\n");
    return 0;
}