unsigned long long rdtscl(void) {
  unsigned int lo, hi;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return (((unsigned long long)lo) | ((unsigned long long)hi) << 32);
}

