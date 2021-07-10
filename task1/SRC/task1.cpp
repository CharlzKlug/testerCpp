#include <iostream>
#include <string>
#include <cmath>

#define USAGE usage (program_name)

int main (int argc, char** argv) {
  void usage (std::string) ;
  std::string program_name (argv[0]) ;
  
  if (!((argc >= 3) && (argc <=4))) {
    /* Wrong argument number */
    USAGE ;
  }

  bool isDuplicates (std::string) ;
  bool isInBase (std::string, std::string) ;
  std::string base1 (argv[2]) ;
  if (isDuplicates (base1)) { USAGE ; }
  if (base1.length () < 2) { USAGE ; }
  
  switch (argc) {
  case 3 : {
    if (! isInBase (argv[1], "0123456789")) { USAGE ; }
    std::string itoBase (unsigned int, std::string) ;
    std::cout << itoBase (std::stoi (argv[1]), base1) << std::endl ;
    break ;
  }

  case 4 : {
    std::string base2 (argv[3]) ;
    if (base2.length () < 2) { USAGE ; }
    if (! isInBase (argv[1], base1)) { USAGE ; }
    if (isDuplicates (base2)) { USAGE ; }
    std::string itoBase (std::string, std::string, std::string) ;
    std::cout << itoBase (argv[1], base1, base2) << std::endl ;
    break ;
  }
  default : {}
  }
  
  return (0) ;
}

bool isInBase (std::string nb, std::string base) {
  /* Check if the number nb exists in [base] encoding */
  // {{{
  for (long unsigned int i = 0 ; i < nb.length () ; i++) {
    if (base.find (nb [i]) == std::string::npos) {
      return (false) ;
    }
  }
  return (true) ;
}
// }}}

bool isDuplicates (std::string str) {
  /* Check the base correctness */
  // {{{
  for (long unsigned int i = 0; i < str.length () ; i++) {
    if (str.find_first_of (str [i]) != str.find_last_of (str [i])) {
      return (true) ;
    }
  }
  return (false) ;
}
// }}}
    
std::string itoBase (unsigned int nb, std::string base) {
  /* Convert the number [nb] from decimal encoding to [base] */
  // {{{
  unsigned int len = base.length () ;
  std::string result ("") ;
  
  do {
    result = base [nb % len] + result ;
    nb /= len ;
  } while (nb) ;

  return (result) ;
}
// }}}

void usage (std::string name) {
  /* Print usage */
  // {{{
  std::cout << "Usage: " << name << " <number> <base>" << std::endl ;
  std::cout << "Usage: " << name << " <number> <baseSrc> <baseDst>" << std::endl ;
  exit (8) ;
}
// }}}

std::string itoBase (std::string nb, std::string baseSrc, std::string baseDst) {
  /* Convert number [nb] from [baseSrc] to [baseDst] */
  // {{{
  unsigned int tmp = 0 ;
  unsigned int base = baseSrc.length () ;
  unsigned int power = 0 ;
  
  for (int i = (nb.length () - 1) ; i >= 0 ; i--) {
    tmp += (baseSrc.find (nb[i])) * (pow (base, power)) ;
    power++ ;
  }

  return (itoBase (tmp, baseDst)) ;
}
// }}}
