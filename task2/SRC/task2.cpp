#include <iostream>
#include <cmath>
#include <fstream>

class Point {
  // {{{
public :
  double x ;
  double y ;
  double z ;

  Point () {
    this->x = 0 ;
    this->y = 0 ;
    this->z = 0 ;
  }
  
  Point (double x, double y, double z) {
    this->x = x ;
    this->y = y ;
    this->z = z ;
  }

  
} ;
// }}}

class Sphere {
  // {{{
public :
  Point center ;
  double radius ;

  Sphere () {
  }
  
  Sphere (Point some_point, double radius) {
    center.x = some_point.x ;
    center.y = some_point.y ;
    center.z = some_point.z ;
    this->radius = radius ;
  }
} ;

class Line {
public :
  Point pa ;
  Point pb ;

  Line () {}

  Line (Point start, Point end) {
    pa.x = start.x ;
    pa.y = start.y ;
    pa.z = start.z ;
    pb.x = end.x ;
    pb.y = end.y ;
    pb.z = end.z ;
  }
} ;
// }}}

int main (int argc, char **argv) {

  std::string myText ;

  std::ifstream myReadFile (argv[1]) ;

  if (myReadFile.is_open()) {
    while (getline (myReadFile, myText)) {
      void process (std::string) ;
      process (myText) ;
    }
  }

  myReadFile.close () ;

  return (0) ;
}

std::string remove_space (std::string txt) {
  // {{{
  std::string tmptxt ("") ;

  for (int i = 0 ; i < txt.length () ; i++) {
    if (txt [i] != ' ' ) {
      tmptxt += txt [i] ;
    }
  }

  return (tmptxt) ;
}
// }}}

void process (std::string txt) {
  // {{{
  /* Evaluate string */
  
  bool line_scanned = false ;
  bool sphere_scanned = false ;

  std::string tmptxt ("") ;
  tmptxt = remove_space (txt) ;

  /* Search center */
  double tmp ;
  std::string val ("") ;

  std::size_t found = tmptxt.find ("center:[") ;
  if (found == std::string::npos) { return ; }
  int i = found + 8 ;
  Sphere mySphere ;
  while (',' != tmptxt [i]) {
    val += tmptxt [i] ;
    i++ ;
  } ;

  mySphere.center.x = std::stod (val) ;
  val = "" ;
  i++ ;
 
  while (',' != tmptxt [i]) {
    val += tmptxt [i] ;
    i++ ;
  } ;

  mySphere.center.y = std::stod (val) ;
  val = "" ;
  i++ ;
  
  while (']' != tmptxt [i]) {
    val += tmptxt [i] ;
    i++ ;
  } ;

  mySphere.center.z = std::stod (val) ;
  val = "" ;

  found = tmptxt.find ("radius:") ;
  if (found == std::string::npos) { return ; }
  i = found + 7 ;

  while ('}' != tmptxt [i] && ',' != tmptxt [i]) {
    val += tmptxt [i] ;
    i++ ;
  }

  mySphere.radius = std::stod (val) ;
  val = "" ;

  found = tmptxt.find ("line:{[") ;
  if (found == std::string::npos) { return ; }
  i = found + 7 ;

  while (',' != tmptxt [i]) {
    val += tmptxt [i] ;
    i++ ;
  }

  Line myLine ;
  myLine.pa.x = std::stod (val) ;
  i++ ;
  val = "" ;

  while (',' != tmptxt [i]) {
    val += tmptxt [i] ;
    i++ ;
  }

  myLine.pa.y = std::stod (val) ;
  i++ ;
  val = "" ;

  while (']' != tmptxt [i]) {
    val += tmptxt [i] ;
    i++ ;
  }

  myLine.pa.z = std::stod (val) ;
  i += 3 ;
  val = "" ;
  
  while (',' != tmptxt [i]) {
    val += tmptxt [i] ;
    i++ ;
  }

  myLine.pb.x = std::stod (val) ;
  i++ ;
  val = "" ;

  while (',' != tmptxt [i]) {
    val += tmptxt [i] ;
    i++ ;
  }

  myLine.pb.y = std::stod (val) ;
  i++ ;
  val = "" ;

  while (']' != tmptxt [i]) {
    val += tmptxt [i] ;
    i++ ;
  }

  myLine.pb.z = std::stod (val) ;
  i++ ;
  val = "" ;

  void searchCollision (Line, Sphere) ;
  searchCollision (myLine, mySphere) ;
}
// }}}

void searchCollision (Line my_line, Sphere my_sphere) {
  // {{{
  double al1 = my_line.pb.x - my_line.pa.x ;
  double al2 = my_line.pb.y - my_line.pa.y ;
  double al3 = my_line.pb.z - my_line.pa.z ;
  
  double a = pow (al1, 2) +
    pow (al2, 2) +
    pow (al3, 2) ;

  if (a < 0.001) { return ; }
  
  double b = 2 * (al1 * (my_line.pa.x - my_sphere.center.x) +
		  al2 * (my_line.pa.y - my_sphere.center.y) +
		  al3 * (my_line.pa.z - my_sphere.center.z)) ;
  
  double c = - pow (my_sphere.radius, 2) + (pow (my_sphere.center.x, 2) +
					    pow (my_sphere.center.y, 2) +
					    pow (my_sphere.center.z, 2)) -
    2 * (my_sphere.center.x * my_line.pa.x + my_sphere.center.y * my_line.pa.y +
	 my_sphere.center.z * my_line.pa.z) + pow (my_line.pa.x, 2) +
    pow (my_line.pa.y, 2) + pow (my_line.pa.z, 2);
  
  double t1 = 0 ;
  double t2 = 0 ;
  int answers = 0 ;
  
  int solveQuadratic (double, double, double, double *, double *) ;
  answers = solveQuadratic (a, b, c, &t1, &t2) ;
  
  if (0 == answers) {
    printf ("Коллизий нет\n") ;
    return ;
  }
  
  double calcCoord (double, double, double) ;
  std::cout << calcCoord (my_line.pa.x, my_line.pb.x, t1) << std::endl ;
  std::cout << calcCoord (my_line.pa.y, my_line.pb.y, t1) << std::endl ;
  std::cout << calcCoord (my_line.pa.z, my_line.pb.z, t1) << std::endl ;
  
  if (1 == answers) { return ; }
  
  std::cout << calcCoord (my_line.pa.x, my_line.pb.x, t2) << std::endl ;
  std::cout << calcCoord (my_line.pa.y, my_line.pb.y, t2) << std::endl ;
  std::cout << calcCoord (my_line.pa.z, my_line.pb.z, t2) << std::endl ;
}
// }}}

double calcCoord (double x1, double x2, double t) {
  // {{{
  return ((x2 - x1) * t + x1) ;
}
// }}}

int solveQuadratic (double a, double b, double c, double * x1, double * x2) {
  // {{{
  double D = pow (b, 2) - 4 * a * c ;
  if (D < 0) {

    return (0) ;
  }

  if (D < 0.0001) {
    *x1 = (- b) / (2 * a) ;
    
    return (1) ;
  }

  *x1 = (-b - sqrt (D)) / (2*a) ;
  *x2 = (-b + sqrt (D)) / (2*a) ;
  return (2) ;
}
// }}}
