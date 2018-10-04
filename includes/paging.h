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

struct pt
{
    MEMBERS()
};

struct pdt
{
    MEMBERS()
};

struct pdpt
{
    MEMBERS()
};

struct pml4e
{
    MEMBERS()
};
