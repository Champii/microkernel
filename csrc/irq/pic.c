/************************************************
*
* Micro Kernel
*
* - pic funcs
*
* Florian Greiner <florian.greiner@epitech.eu>
*
************************************************/

#include                  "pic.h"
#include                  "idt.h"
#include                  "irq.h"
#include                  "system.h"

void                      irq_set_mask(unsigned char irq)
{
  unsigned short          port;
  unsigned char           value;

  if (irq < 8)
    port = PIC_MASTER_DATA;
  else
  {
    port = PIC_SLAVE_DATA;
    irq -= 8;
  }

  value = inportb(port) | (1 << irq);
  outportb(port, value);
}

void                      irq_clear_mask(unsigned char irq)
{
  unsigned short          port;
  unsigned char           value;

  if (irq < 8)
    port = PIC_MASTER_DATA;
  else
  {
    port = PIC_SLAVE_DATA;
    irq -= 8;
  }

  value = inportb(port) & ~(1 << irq);
  outportb(port, value);
}

void                      acknowledge_irq()
{

}

void                      init_pic()
{
  /*Init command*/
  outportb(PIC_MASTER_COM, 0x11);
  outportb(PIC_SLAVE_COM, 0x11);

  /*idt base offset*/
  outportb(PIC_MASTER_DATA, 0x20);
  outportb(PIC_SLAVE_DATA, 0x28);

  /*Master/slave connection*/
  outportb(PIC_MASTER_DATA, 0x4);
  outportb(PIC_SLAVE_DATA, 0x2);

  /**/
  outportb(PIC_MASTER_DATA, 0x1);
  outportb(PIC_SLAVE_DATA, 0x1);

  /**/
  outportb(PIC_MASTER_DATA, 0x0);
  outportb(PIC_SLAVE_DATA, 0x0);

  irq_clear_mask(0);
  irq_clear_mask(1);

}