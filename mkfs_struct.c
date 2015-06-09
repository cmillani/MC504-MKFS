#include <stdint.h>

void setbit(uint8_t byte, uint8_t bit)
{
	byte |= (1 << bit);
}
void clearbit(uint8_t byte, uint8_t bit)
{
	byte &= ~(1 << bit);
}