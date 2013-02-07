// RUN: rm -rf %t
// RUN: %clang_cc1 -x objective-c++ -fmodules -fmodule-cache-path %t -I %S/Inputs %s -verify

int &global(int);
int &global2(int);

namespace N6 {
  char &f(char);
}

namespace N8 { }

namespace LookupBeforeImport {
  int &f(int);
}
void testEarly() {
  int &r = LookupBeforeImport::f(1);
}

@import namespaces_left;
@import namespaces_right;

void test() {
  int &ir1 = N1::f(1);
  int &ir2 = N2::f(1);
  int &ir3 = N3::f(1);
  int &ir4 = global(1);
  int &ir5 = ::global2(1);
  float &fr1 = N1::f(1.0f);
  float &fr2 = N2::f(1.0f);
  float &fr3 = global(1.0f);
  float &fr4 = ::global2(1.0f);
  float &fr5 = LookupBeforeImport::f(1.0f);
  double &dr1 = N2::f(1.0);
  double &dr2 = N3::f(1.0);
  double &dr3 = global(1.0);
  double &dr4 = ::global2(1.0);
  double &dr5 = LookupBeforeImport::f(1.0);
}

// Test namespaces merged without a common first declaration.
namespace N5 {
  char &f(char);
}

namespace N10 { 
  int &f(int);
}

void testMerged() {
  int &ir1 = N5::f(17);
  int &ir2 = N6::f(17);
  int &ir3 = N7::f(17);
  double &fr1 = N5::f(1.0);
  double &fr2 = N6::f(1.0);
  double &fr3 = N7::f(1.0);
  char &cr1 = N5::f('a');
  char &cr2 = N6::f('b');
}

// Test merging of declarations within namespaces that themselves were
// merged without a common first declaration.
void testMergedMerged() {
  int &ir1 = N8::f(17);
  int &ir2 = N9::f(17);
  int &ir3 = N10::f(17);
}

// Test merging when using anonymous namespaces, which does not
// actually perform any merging.
void testAnonymousNotMerged() {
  N11::consumeFoo(N11::getFoo()); // expected-error{{cannot initialize a parameter of type 'N11::<anonymous>::Foo *' with an rvalue of type 'N11::<anonymous>::Foo *'}}
  N12::consumeFoo(N12::getFoo()); // expected-error{{cannot initialize a parameter of type 'N12::<anonymous>::Foo *' with an rvalue of type 'N12::<anonymous>::Foo *'}}  
}

// namespaces-right.h: expected-note@60 {{passing argument to parameter here}}
// namespaces-right.h: expected-note@67 {{passing argument to parameter here}}
