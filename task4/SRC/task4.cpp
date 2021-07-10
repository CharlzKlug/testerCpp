#include <iostream>

#define USAGE usage (argv [0])

int main (int argc, char **argv) {

  void usage (std::string) ;
  if (3 != argc) {
    USAGE ;
    return (0) ;
  }

  bool is_equal (std::string, std::string) ;
  if (is_equal (argv[1], argv[2])) {
    printf ("OK\n") ;
  } else {
    printf ("KO\n") ;
  }
  
  return (0) ;
}

void usage (std::string program_name) {
  // {{{
  printf ("Usage:\n") ;
  std::cout << " " << program_name << " [line] [mask]" << std::endl ;
}
// }}}

bool is_equal (std::string some_string, std::string mask) {
  // {{{
  if (0 == some_string.length () && 0 == mask.length ()) {
    return (true) ;
  }

  if ('*' != mask [0]) {
    if (some_string [0] == mask [0]) {
      return (is_equal (some_string.substr (1), mask.substr (1))) ;
    } else { return (false) ; }
  } else {
    if (1 == mask.length ()) {
      return (true) ;
    }

    if ('*' == mask[1]) {
      return (is_equal (some_string, mask.substr (1))) ;
    }

    for (int i = 0 ; i < some_string.length () ; i++) {
      if (is_equal (some_string.substr (i), mask.substr (1))) {
	return (true) ;
      }
    }
  }


  return (false) ;
}
// }}}
