#include<stdio.h>  
int euclid(int a, int b)
{
    if(b == 0) return a;
    else return euclid(b,a%b);
}
int Laymumodun(int a, int b, int n)
{
    if (b==0) return 1;
    if (b==1) return a;
    int r = Laymumodun(a,b/2,n);
    r = (r*r)%n;
    if(b%2 ==1) r = r*a;
    return r%n;
}
int main()
{   
    int a,b,c;
    scanf("%d%d%d",&a,&b,&c);
    printf("%d",Laymumodun(a,b,c));
    //printf("\n%d",euclid(a,b));
}