#include <cstdint>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>

using inte = uint8_t;

typedef enum FL { set = 1, reset = 0 } FL; 

struct mp{
    inte a;
    inte b;
    inte c;
    inte d;
    inte e;
    inte h;
    inte l;
    FL cflag = reset;
    FL zflag = reset;

    std::vector<inte> memory;

    mp () : a(0), b(0), c(0), d(0), e(0), h(0), l(0),
        memory(3000) {}

    void MOV(inte& dest, inte& src) {
        dest = src;
    }

    void MVI(inte& dest, int value){
        dest = value;
    }

    void ADD(inte& r){
        a = a + r;
        
        if( a > 0xFF ) {
            cflag = set;
        }
    }

    void SUB(inte& r){
        a = a - r;
    }

    void INR(inte& r){
        r = r+1;

        if(r == 0){
            zflag = set;
        } else {
            zflag = reset;
        }
    }

    void DCR(inte& r){
        r = r-1;

        if(r == 0){
            zflag = set;
        } else {
            zflag = reset;
        }
    }

    void SUI(int value){
        a = a - value;
    } 

    void ADI(int value){
        if ( a + value > 0xFF ) {
            a = a + value & 0xFf;
            cflag = set;
        } else {
            a = a + value;
            cflag = reset;
        }
    }

    void STA(const int& memoryAddress){
        if ( cflag == set ) {
        memory[memoryAddress] = a;
        memory[memoryAddress + 1] = 0x01;
        } else {
            memory[memoryAddress] = a;
            memory[memoryAddress + 1] = 0x00;
        }
    }

    void LXI(inte& r1, inte& r2, const int& memoryAddress){
       r1 = memory[memoryAddress];
       r2 = memory[memoryAddress+1];
    }

    void LDAX(inte& r1, inte& r2){

        std::string memoryAddress = std::to_string(r1) + std::to_string(r2); 

        a = memory[std::stoi(memoryAddress)];
    }

    void INX(inte& r1, inte& r2) { 
        r2 = r2 +1;     
    }

    void ADC() {
        std::string memoryAddress = std::to_string(h) + std::to_string(l);

        if(cflag == set){ 

            if (  (a + 1 + memory[std::stoi(memoryAddress)]) > 0xFF ) {
                cflag = set;
                a = (a + 1 + memory[std::stoi(memoryAddress)]) & 0xFF;  

            } else {
                a = (a + memory[std::stoi(memoryAddress)]) ;
            }

        } else {
            a = (a + memory[std::stoi(memoryAddress)]);
        }
    }

    void SHLD(const int& memoryAddress){
        memory[memoryAddress] = h;
        memory[memoryAddress+ 1] = l;
    }

    void CMP(inte& r){
        if ( a - r ==  0 ) {
            zflag = set;
        } else {
            zflag = reset;
        }
    }

    void XRA(){
        a = 0;
        zflag = set;
    }

    void JNZ(void (*cb)(mp&)){

        if( zflag == reset){
            cb(*this);
        }
    }
};
 
int main() {
    mp p;
    
    p.LXI(p.h, p.l, 2050);
    p.MVI(p.b, 0);

    p.INR(p.b);

    std::cout<< static_cast<int>(p.b) << std::endl;
}

