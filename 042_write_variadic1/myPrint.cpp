#include <cstring>
#include <iostream>
// We define a namespace so printf does not clash
namespace practice {

  // TODO: This is your base function
  void printf(const char * format) { std::cout << format; }

  // TODO: This is your 'recursive' function
  template<typename T, typename... Targs>
  void printf(const char * format, T value, Targs... args) {
    const char * p = strchr(format, '$');
    char temp[p - format + 1];
    strncpy(temp, format, p - format);
    temp[p - format] = '\0';
    std::cout << temp << value;
    printf(p + 1, args...);
  }

}  // namespace practice

int main(void) {
  // Should print: Hello World foobar 4
  practice::printf("$ $ foo$ $\n", "Hello", "World", "bar", 4);
  return EXIT_SUCCESS;
}
