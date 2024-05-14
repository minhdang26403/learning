#include "addressbook.pb.h"
#include <iostream>

int main() {
  tutorial::Person person;
  person.set_name("yoona");
  std::cout << person.name() << '\n';

  return 0;
}
