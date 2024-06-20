/*
 * IncFile1.h
 *
 * Created: 1/31/2023 1:36:58 PM
 *  Author: DELL
 */ 
#define read(x,n) (x&(1<<n))>>n
#define set(x,n) x|=(1<<n)
#define clear(x,n) x=(x&~(1<<n))
#define toggle(x,n) x=x^(1<<n)


