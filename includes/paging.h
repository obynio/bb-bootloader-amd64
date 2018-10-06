#pragma once
#include <stdint.h>

#define MEMBERS() \
   uint64_t present    : 1;\
   uint64_t rw         : 1;\
   uint64_t useless    : 5;\
   uint64_t huge_page  : 1;\
   uint64_t unused     : 2;\
   uint64_t frame      : 42;\
   uint64_t unused2    : 12;

struct pml4e
{
    uint8_t  p          :  1;
    uint8_t  rw         :  1;
    uint8_t  us         :  1;
    uint8_t  pwt        :  1;
    uint8_t  pcd        :  1;
    uint8_t  a          :  1;
    uint8_t  ign        :  1;
    uint8_t  mbz        :  2;
    uint8_t  avl        :  3;
    uint64_t pdpba      : 40;
    uint32_t available  : 11;
    uint8_t  nx         :  1;
}__attribute__((packed));

struct pdpe
{
    uint8_t  p          :  1;
    uint8_t  rw         :  1;
    uint8_t  us         :  1;
    uint8_t  pwt        :  1;
    uint8_t  pcd        :  1;
    uint8_t  a          :  1;
    uint8_t  ign        :  1;
    uint8_t  zero       :  1;
    uint8_t  mbz        :  1;
    uint8_t  avl        :  3;
    uint64_t pdpba      : 40;
    uint32_t available  : 11;
    uint8_t  nx         :  1;
}__attribute__((packed));

struct pde
{
    uint8_t  p          :  1;
    uint8_t  rw         :  1;
    uint8_t  us         :  1;
    uint8_t  pwt        :  1;
    uint8_t  pcd        :  1;
    uint8_t  a          :  1;
    uint8_t  d          :  1;
    uint8_t  one        :  1;
    uint8_t  g          :  1;
    uint8_t  avl        :  3;
    uint8_t  pat        :  1;
    uint8_t  mbz        :  8;
    uint64_t pdpba      : 31;
    uint32_t available  : 11;
    uint8_t  nx         :  1;
}__attribute__((packed));
