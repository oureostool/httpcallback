#pragma once

#include <string>
#include <eosiolib/memory.h>

std::string bin2hex(const unsigned char *bin, size_t len)
{
  std::string out;
 
  if (bin == NULL || len == 0)
    return out;
 
  out.resize(len*2);
  char *dest = &out[0];
  for (size_t i=0; i<len; i++) {
    dest[i*2]   = "0123456789ABCDEF"[bin[i] >> 4];
    dest[i*2+1] = "0123456789ABCDEF"[bin[i] & 0x0F];
  }
 
  return out;
}

int hexc2bin(const char hex, char *out)
{
  if (out == NULL)
    return 0;

  if (hex >= '0' && hex <= '9') {
    *out = hex - '0';
  } else if (hex >= 'A' && hex <= 'F') {
    *out = hex - 'A' + 10;
  } else if (hex >= 'a' && hex <= 'f') {
    *out = hex - 'a' + 10;
  } else {
    return 0;
  }

  return 1;
}

std::string hexs2bin(const char *hex, size_t len)
{
  std::string out;
  char   b1;
  char   b2;

  if (hex == NULL || *hex == '\0' || len <= 0)
    return std::string("");

  if (len % 2 != 0)
    return 0;
  len /= 2;

  out.resize(len);
  char *dest = &out[0];
  memset(dest, 'A', len);
  for (size_t i=0; i<len; i++) {
    if (!hexc2bin(hex[i*2], &b1) || !hexc2bin(hex[i*2+1], &b2)) {
      return std::string("");
    }
    dest[i] = (b1 << 4) | b2;
  }

  return std::move(out);
}

