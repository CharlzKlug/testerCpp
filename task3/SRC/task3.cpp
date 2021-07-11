#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

#define USAGE usage (argv[0])
#define CHECKDIGIT(n) if (!std::isdigit (argument [(n)])) { return (false) ; }
#define CHECK(n, c) if (argument [(n)] != c) { return (false) ; }

int main (int argc, char **argv) {

  void usage (std::string) ;
  if (4 != argc) {
    USAGE ;
    return (8) ;
  }

  bool isCorrectArg (std::string) ;
  if (!isCorrectArg (argv [2]) ||
      !isCorrectArg (argv [3])) {
    USAGE ;
    return (8) ;
  }
  
  std::string dat1 (argv [2]);
  dat1 = dat1 + ":000" ;

  std::string dat2 (argv [3]) ;
  dat2 = dat2 + ":000" ;

  void process (std::string, std::string, std::string) ;
  process (argv[1], dat1, dat2) ;

  return (0) ;
}

bool isCorrectArg (std::string argument) {
  if (argument.length () != 19) {
    return (false) ;
  }
  CHECKDIGIT (0) ;
  CHECKDIGIT (1) ;
  CHECKDIGIT (2) ;
  CHECKDIGIT (3) ;
  CHECK (4, '-') ;
  CHECKDIGIT (5) ;
  CHECKDIGIT (6) ;
  CHECK (7, '-') ;
  CHECKDIGIT (8) ;
  CHECKDIGIT (9) ;
  CHECK (10, 'T') ;
  CHECKDIGIT (11) ;
  CHECKDIGIT (12) ;
  CHECK (13, ':') ;
  CHECKDIGIT (14) ;
  CHECKDIGIT (15) ;
  CHECK (16, ':') ;
  CHECKDIGIT (17) ;
  CHECKDIGIT (18) ;
  return (true) ;
}
  
void usage (std::string prgName) {
  // {{{
  std::cout << "Usage: " << prgName <<
    " log-file <start datetime> <end datetime>" << std::endl ;
  std::cout << " Sample: " << prgName <<
    " sample.log 1900-11-23T23:37:10 1901-01-03T07:00:00" <<
    std::endl ;
}
// }}}

void process (std::string logFile, std::string start, std::string end) {
  // {{{
  std::ifstream myReadFile (logFile) ;
  std::string myText ;
  double totalVolume = 0 ;
  double currentWaterVolume = 0 ;
  double beforePeriodVolume = 0 ;

  unsigned int topUpCount = 0 ;
  unsigned int topUpFailed = 0 ;
  double pouredWaterVolume = 0 ;
  double notPouredWaterVolume = 0 ;

  unsigned int scoopCount = 0 ;
  unsigned int scoopFailed = 0 ;
  double scoopedWaterVolume = 0 ;
  double notScoopedWaterVolume = 0 ;
  
  bool checkedStart = false ;

  bool knownStart = false ;
  bool knownEnd = false ;
  
  std::string tmpString ("") ;
  std::string previousTime ("") ;
  if (myReadFile.is_open ()) {
    try {
      if (getline (myReadFile, myText)) {
	totalVolume = std::stod (myText) ;
      } 
      
      if (getline (myReadFile, myText)) {
	currentWaterVolume = std::stod (myText) ;
      }

      if (getline (myReadFile, myText)) {
	previousTime = myText.substr (0,23) ;

	if (end.compare (previousTime) < 0) {
	  goto finish ;
	}

	if (end.compare (previousTime) == 0) {
	  knownEnd = true ;
	}

	if (start.compare (previousTime) < 0) {
	  checkedStart = true ;
	}

	if (start.compare (previousTime) == 0) {
	  checkedStart = true ;
	  knownStart = true ;
	  beforePeriodVolume = currentWaterVolume ;
	}
      }

      do {
	tmpString = myText.substr (0, 23) ;

	if (end.compare (tmpString) < 0) {
	  knownEnd = true ;
	  goto finish ;
	}

	if (!checkedStart) {
	  beforePeriodVolume = currentWaterVolume ;
	  if (start.compare (previousTime) > 0 &&
	      start.compare (tmpString) < 0) {
	    checkedStart = true ;
	    knownStart = true ;
	    beforePeriodVolume = currentWaterVolume ;
	  }
	}

	bool inRange = (start.compare (myText.substr (0, 23)) <= 0 &&
			end.compare (myText.substr (0, 23)) >= 0) ;
	
	std::size_t found = myText.find ("scoop") ;
	if (found != std::string::npos) {
	  tmpString = myText.substr (found + 6, myText.length () - 2) ;
	  double scoopVolume = std::stod (tmpString) ;

	  scoopCount += (inRange ? 1 : 0) ;
	  
	  if (currentWaterVolume > scoopVolume) {
	    currentWaterVolume -= scoopVolume ;
	    scoopedWaterVolume += ( inRange ? scoopVolume : 0 ) ;
	  } else {
	    scoopFailed += (inRange ? 1 : 0) ;
	    notScoopedWaterVolume += (inRange ? scoopVolume : 0) ;
	  }
	}

	found = myText.find ("top up") ;
	if (found != std::string::npos) {
	  // {{{
	  tmpString = myText.substr (found + 7, myText.length () - 2) ;
	  double topUpVolume = std::stod (tmpString) ;

	  topUpCount += (inRange ? 1 : 0) ;
	  
	  if (currentWaterVolume + topUpVolume <= totalVolume) {
	    currentWaterVolume += topUpVolume ;
	    pouredWaterVolume += (inRange ? topUpVolume : 0) ;
	  } else {
	    topUpFailed += (inRange ? 1 : 0) ;
	    notPouredWaterVolume += (inRange ? topUpVolume : 0) ;
	  }
	}
	// }}}

	/* Full equal start of period */
	if (! checkedStart && 0 == start.compare (myText.substr (0, 23))) {
	  // {{{
	  checkedStart = true ;
	  knownStart = true ;
	}
	// }}}

	/* Full equal end of period */
	if (end.compare (myText.substr (0,23)) == 0) {
	  // {{{
	  knownEnd = true ;
	}
	// }}}
      } while (getline (myReadFile, myText)) ;
    } catch (...) {
      fprintf (stderr, "Error occured in log file!\n") ;
    }
  }
  
 finish :

  myReadFile.close () ;
  
  if (knownStart == false && knownEnd == false) {
    // {{{
    printf ("Top up tries: unknown\n") ;
    printf ("Top up errors percentage: unknown\n") ;
    printf ("Total volume of poured water: unknown\n") ;
    printf ("Total volume of not poured water: unknown\n") ;

    printf ("Scoop tries: unknown\n") ;
    printf ("Scoop errors percentage: unknown\n") ;
    printf ("Total volume of scooped water: unknown\n") ;
    printf ("Total volume of not scooped water: unknown\n") ;

    printf ("Before period water volume: unknown\n") ;
    printf ("After period water volume: unknown\n") ;
  }
  // }}}	

  if (knownStart == false && knownEnd == true) {
    // {{{
    printf ("Top up tries: unknown\n") ;
    printf ("Top up errors percentage: unknown\n") ;
    printf ("Total volume of poured water: unknown\n") ;
    printf ("Total volume of not poured water: unknown\n") ;

    printf ("Scoop tries: unknown\n") ;
    printf ("Scoop errors percentage: unknown\n") ;
    printf ("Total volume of scooped water: unknown\n") ;
    printf ("Total volume of not scooped water: unknown\n") ;

    printf ("Before period water volume: unknown\n") ;
    printf ("After period water volume: %.2f\n", currentWaterVolume) ;
  }
  // }}}

  if (knownStart == true && knownEnd == false) {
    // {{{
    printf ("Top up tries: unknown\n") ;
    printf ("Top up errors percentage: unknown\n") ;
    printf ("Total volume of poured water: unknown\n") ;
    printf ("Total volume of not poured water: unknown\n") ;

    printf ("Scoop tries: unknown\n") ;
    printf ("Scoop errors percentage: unknown\n") ;
    printf ("Total volume of scooped water: unknown\n") ;
    printf ("Total volume of not scooped water: unknown\n") ;

    printf ("Before period water volume: %.2f\n", beforePeriodVolume) ;
    printf ("After period water volume: unknown\n") ;
  }
  // }}}

  if (knownStart == true && knownEnd == true) {
    // {{{
    printf ("Top up tries: %d\n", topUpCount) ;
    printf ("Top up errors percentage: ") ;
    if (topUpCount != 0) {
      printf ("%.2f",
	      (((double) topUpFailed) / ((double) topUpCount)) * 100) ;
    } else { printf ("0") ; }
    printf ("%%\n") ;
    printf ("Total volume of poured water: %.2f\n", pouredWaterVolume) ;
    printf ("Total volume of not poured water: %.2f\n", notPouredWaterVolume) ;
    
    printf ("Scoop tries: %d\n", scoopCount) ;
    printf ("Scoop errors percentage: ") ;
    if (scoopCount != 0) {
      printf ("%.2f",
	      (((double) scoopFailed) / ((double) scoopCount)) * 100) ;
    } else { printf ("0") ; }
    printf ("%%\n") ;
    printf ("Total volume of scooped water: %.2f\n", scoopedWaterVolume) ;
    printf ("Total volume of not scooped water: %.2f\n", notScoopedWaterVolume) ;
    
    printf ("Before period water volume: %.2f\n", beforePeriodVolume) ;
    printf ("After period water volume: %.2f\n", currentWaterVolume) ;
    
  }
  // }}}
}
// }}}
