// Test code for LED animations

#include <stdio.h>
#include <stdint.h>

#define LEDSTRIPSIZE 120

int main(void)
{
    // test for 1 cycle of Comet's Tail
    uint8_t tailLen = 6;
    int16_t i, q = 0;
    int16_t ledN = 0;
    
    // Determines where the comet's head and tail are located
    int16_t cometHeadLoc = 0;
    int16_t cometTailLoc = 0;
    //int16_t cometLength = tailLen + 1;
    
    uint16_t frames = LEDSTRIPSIZE + tailLen;

    // Forward comet flight
    for(i=0; i < frames; i++)
    {
        for(ledN = 0; ledN < cometTailLoc; ledN++)
        {
            //printf("%d: off\n", ledN);
        }
        for(q = tailLen; ledN < cometHeadLoc; ledN++, q--)
        {
            printf("%d: tail\n", ledN);
        }
        if(ledN == cometHeadLoc)
        {
            printf("%d: head\n", ledN);
        }
        
        // Advance head and tail
        cometHeadLoc = (cometHeadLoc < LEDSTRIPSIZE-1)? cometHeadLoc+1 : LEDSTRIPSIZE-1;
        cometTailLoc = (cometHeadLoc < tailLen)? 0 : (cometHeadLoc == LEDSTRIPSIZE-1)? cometTailLoc+1 : cometHeadLoc - tailLen;
        printf("cometHeadLoc: %d\tcometTailLoc: %d\n", cometHeadLoc, cometTailLoc);
        printf("-------\n");
    }
    
    printf("-------\n\n\n\n\n\n\n\n\n\n\n");
    cometHeadLoc = LEDSTRIPSIZE - 1;
    cometTailLoc = LEDSTRIPSIZE - 1;
    
    // Reverse flight
    for(i=0; i < frames; i++)
    {
        for(ledN = 0; ledN < cometHeadLoc; ledN++)
        {
            //printf("%d: off\n", ledN);
        }
        if(ledN == cometHeadLoc)
        {
            printf("%d: head\n", ledN++);
        }
        for(q = 1; ledN <= cometTailLoc; ledN++, q++)
        {
            printf("%d: tail\n", ledN);
        }
        if(ledN > cometTailLoc)
        {
            printf("%d: off\n", ledN);
        }
        
        // Advance head and tail
        cometHeadLoc = (cometHeadLoc > 0)? cometHeadLoc-1 : 0;
        cometTailLoc = ((cometTailLoc-cometHeadLoc) <= tailLen && cometHeadLoc > 0)? LEDSTRIPSIZE-1 : \
                        (cometTailLoc > 0)? cometTailLoc - 1 : 0;
                        
        printf("cometHeadLoc: %d\tcometTailLoc: %d\n", cometHeadLoc, cometTailLoc);
        printf("-------\n");
    }
}