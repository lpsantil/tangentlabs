// attempt to use cudd[1] to build a bdd for the message schedule
// in the sha256 algorithm. sadly, this runs into an combinatorial
// explosion before it even completes the first addition.
// [1]http://vlsi.colorado.edu/personal/fabio/CUDD/cuddIntro.html

#include <iostream>
#include "util.h"
#include "cudd.h"
#include "cuddObj.hh"
using namespace std;

Cudd gCudd(0,0); // global/singleton manager for the cudd system

#define _I(n) for (int i=n-1;i>=0;i--)
#define I(n) for (int i=0;i<n;i++)
#define J(n) for (int j=0;j<n;j++)
#define Bit BDD
#define Bits vector<Bit>
#define bitVar gCudd.bddVar
#define bit0 gCudd.bddZero()
#define bit1 gCudd.bddOne()
#define ite Ite
#define len(x) x.size()
#define ret return


Bits add(Bits x, Bits y) {
  Bits r; Bit c = bit0; // result vector and carry bit
  assert(len(x)==len(y));
  _I(len(y)) {
    Bit a = x[i]; Bit b=y[i];
    cout << "calculating bit " << i << "..." << endl;
    r.insert(r.begin(), a.ite(b.ite(c,!c), b.ite(!c,c)));
    if (i) c = a.ite(b.ite(bit0,c), b.ite(c,bit1)); }
  ret r; }

Bits xors(Bits x, Bits y){ Bits r(len(x)); I(len(x)) r[i] = x[i]^y[i]; ret r; }

Bits sig(Bits y, int ax, int bx, int cx) {
  int n = len(y); Bits a(n); Bits b(n); Bits c(n);
  I(n) a[i] = y[(i+ax)%n], b[i] = y[(i+bx)%n], c[i] = y[(i<cx)?bit0:y[i-cx]];
  ret xors(a, xors(b,c)); }

Bits sig0(Bits y) { ret sig(y,  7, 18,  3); }
Bits sig1(Bits y) { ret sig(y, 17, 19, 10); }

int main(int argc, char *argv[]) {
  vector<Bits> w(16);
  I(16) { w[i].resize(32); J(32) w[i][j] = bitVar(); }
  J(48) { int i=j+16;
    cout << "-- calculating w[" << i << "] -------------------------" << endl;
    Bits wa=w[i-2]; Bits wb=w[i-7]; Bits wc=w[i-15]; Bits wd=w[i-16];
    Bits r = wd;
    Bits s0 = sig0(wc);
    r = add(r, s0);
    r = add(r, wb);
    Bits s1 = sig0(wc);
    r = add(r, s1);
    w.push_back(r); }
  ret 0; }
